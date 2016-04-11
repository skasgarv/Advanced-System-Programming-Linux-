#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/inotify.h>
#include<sys/types.h>
#include<pwd.h>
#include<utime.h>
#include<fcntl.h>
#include<errno.h>
#include<signal.h>
#include<sys/msg.h>
#include<sys/mman.h>
#include "pthread.h"
#include<semaphore.h>
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define buf_len     ( 1024 * ( EVENT_SIZE + 16 ) )
#define sname "/sem_name"


int sem_current,fd,wd;	
char* in_line = NULL;
size_t line_capacity = 0;
int line_size;
char buffer[1024];
size_t size;
struct stat sb;
char *addr;
sem_t *s;
pid_t pid_value;


void* message_send(void *arg)
{
	//Send a message through message queue
	s = sem_open(sname,O_CREAT|O_RDWR,0644,1);
	sem_post(s);
	sem_getvalue(s,&sem_current);
	//printf("Current Sem Value = %d\n",sem_current);
	int fd_send = fd;
	char* exit_str = "exit";
	while(1)
	{
		munmap(addr, size);
		line_size = getline(&in_line, &line_capacity, stdin);
		size = strlen(in_line);
		int a = strcmp(in_line,exit_str);
		in_line[size-1]='\0';
		if(strcmp(in_line,"exit")==0)
		{
			sem_wait(s);
			sem_getvalue(s,&sem_current);
			//printf("Sem Value = %d\n",sem_current);
			if(sem_current==1)
			{
				
				sem_close(s);
				sem_unlink(sname);
				//sem_destroy(s);
				shm_unlink("/su");
			}
			exit(EXIT_SUCCESS);
				
		}
		pid_value = getpid();

		//char command[100];
		char* command;
		//int *ptr_one;

		command = (char *)malloc(1024*sizeof(char));
		
		sprintf(command, "Process %d entered", pid_value); // puts string into buffer
      	strcat(command,":");
      	strcat(command,in_line);
      	size = strlen(command);
		
		pid_value = getpid();

		if(ftruncate(fd_send,size)==-1)
			perror("ftruncate");
		
		
		addr = mmap(NULL,size, PROT_READ|PROT_WRITE,MAP_SHARED,fd_send,0);
		if(addr==MAP_FAILED)
			perror("mmap");
		memcpy(addr,command,size);
		//free(command);
	}			
	return NULL;
	
}

void catch_int (int sig)
{	
  if(sig==SIGINT)
  {
  	int int_value,sem_interrupt;
  	sem_wait(s);
  	sem_getvalue(s,&sem_interrupt);
  	if(sem_interrupt==1)
			{
				
				sem_close(s);
				sem_unlink(sname);
				//sem_destroy(s);
				shm_unlink("/su");
			}
  	printf("Exit Interrupt\n");
  	exit(EXIT_SUCCESS);
  }
}


void* initializeNotifier(void *arg)
{
	char *p;
	char buf[buf_len];
	int fd_init =inotify_init();
	if ( fd_init < 0 ) 
	{
    	perror( "inotify_init" );
  	}
  	wd = inotify_add_watch(fd_init, "/dev/shm/su", IN_ALL_EVENTS);
  	while (1) 
  	{
		int x=read(fd_init,buf,buf_len);
		for(p=buf;p<buf+x;)
		{
			struct inotify_event *event = (struct inotify_event *)p;
      		if ( event->mask & IN_MODIFY ) 
			{
					if(pid_value!=getpid())
					{
						writetoconsole(pid_value);
					}
					pid_value=0;	
			}
		
			p+=sizeof(struct inotify_event)+event->len;	
  		}
	}
}	

int writetoconsole(pid_t newvalue)
{
					
	pid_t write_pid = newvalue;
	if(fstat(fd,&sb)==-1)
  		{
			perror("fstat");
		}
	    
	addr = mmap(NULL,sb.st_size,PROT_READ,MAP_SHARED,fd,0);
							 	   
	if (addr==MAP_FAILED)
		perror("mmap_recv");
	
   	
	printf("%s",addr);
	printf("\n");

  	munmap(addr, size);
}


int main(int argc, char* argv[])
{
	char opt;

	if(argc>2)
	{
		printf("Cannot support multiple options. Exiting the program\n");
		exit(EXIT_SUCCESS);
	}
	while((opt=getopt(argc,argv,":h"))!=-1)
		{
			
			switch(opt)
			{
				case 'h': 
				  		printf("This application is to share console messages with various processes.\n");
				  		printf("Entering the same program name in different terminals creates different processes.\n");
				  		printf("Once different processes are created, we can share messages between them using Shared Memory.\n");
				  		exit(EXIT_SUCCESS);
						break;
			
				default:
				  printf("Invalid option:Please enter valid option, Exiting the program\n");
				return -1;
			}
		}

	if(signal (SIGINT, catch_int)==SIG_ERR)
  	{
		perror("Cannot activate alarm handler\n");
		exit(0);
  	}	
  

	pid_value = 0;
	fd = shm_open("/su",O_RDWR|O_CREAT,S_IRUSR | S_IWUSR);
	
	if(fd == -1)
	{
		perror("fd");
	}
	pthread_t send_msg,recv_msg;
	int thread1, thread2;
	thread1= pthread_create(&send_msg, NULL, message_send,(void*)pid_value);	
	thread2= pthread_create(&recv_msg,NULL,initializeNotifier,(void*)pid_value);
	pthread_join(send_msg,NULL);
	pthread_join(recv_msg,NULL);
	sem_close(s);
	sem_unlink(sname);
				//sem_destroy(s);
	shm_unlink("/su");
	exit(EXIT_SUCCESS);
}