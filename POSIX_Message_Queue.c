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
#include<mqueue.h>
#include "pthread.h"
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define buf_len     ( 1024 * ( EVENT_SIZE + 16 ) ) 
char* in_line = NULL;
size_t size,line_capacity = 0;
int line_size,wd;
struct mq_attr attr,*attrp;
mqd_t mqd;
pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t con=PTHREAD_COND_INITIALIZER; 

void* message_write(void *arg)
{	
	char* quit_str = "quit";
	while(1)
	{

		line_size = getline(&in_line, &line_capacity, stdin);
		size = strlen(in_line);
		in_line[size-1]='\0';
		if(strcmp(in_line,"quit")==0)
		{
			//mq_close(mqd);
			//mq_unlink("/su");
			exit(EXIT_SUCCESS);
		}
		pthread_mutex_lock(&lock);
		if(mq_send(mqd,in_line,size,0)==-1)
		{
			perror("mq_send");
		}
		pthread_cond_signal(&con);
		pthread_mutex_unlock(&lock);		

   	}
	return NULL;

}


void* message_display(void *arg)
{
	while(1)
	{
		void *buffer;
		char* format = "User Entered:";
		int size = strlen(format);
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&con,&lock);
		buffer = malloc(attr.mq_msgsize);
		if(buffer==NULL)
		{
			perror("Buffer");
		}
		if(mq_getattr(mqd,&attr)==-1)
		{
			perror("Attr");
		}
		size_t numread;
		numread = mq_receive(mqd,buffer,attr.mq_msgsize,0);
		if(numread==-1)
		{
			perror("mq_receive");
		}
		write(STDOUT_FILENO,format,size);
		if(write(STDOUT_FILENO,buffer,numread)==-1)
		{
			perror("Write");
		}
		write(STDOUT_FILENO,"\n",1);
		free(buffer);
		pthread_mutex_unlock(&lock);
	}
}


int main(int argc, char *argv[])
{

	attrp = NULL;
	attr.mq_maxmsg=50;
	attr.mq_msgsize = 2048;
	
	mqd = mq_open("/su",O_RDWR|O_CREAT,S_IRUSR|S_IWUSR,NULL);
	if(mqd==(mqd_t)-1)
	{
		perror("mq_open");
	}	

	pthread_t send_msg,recv_msg;
	int thread1, thread2;
	thread1= pthread_create(&send_msg, NULL, message_write,NULL);	
	thread2= pthread_create(&recv_msg,NULL,message_display,NULL);
	pthread_join(send_msg,NULL);
	pthread_join(recv_msg,NULL);
	exit(EXIT_SUCCESS);
}
