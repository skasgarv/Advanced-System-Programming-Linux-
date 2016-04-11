#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<utime.h>
#include<fcntl.h>
#include<errno.h>
#include<signal.h>

FILE * fp;
int file_read(char source_file[],char dest_file[]);
int file_user_write();
void user_fileread();

int fd_to, fd_from,ret_out,ret_in,ret_output;

time_t rawtime;
struct tm* timeinfo;

char line_number=0;

char* in_line = NULL;
size_t line_capacity = 0;
int line_size;
char buffer[1024];

volatile sig_atomic_t read_flag=0;

struct stat buf ;
struct stat *c=&buf; //pointer to buf    
struct sigaction sa;

void int_catch(int sig)
{
	if(sig==SIGINT)
	{
		fclose(fp);
		close(fd_to);	
		exit(0);	
	}
}
void catch_alarm (int sig)
{	
	if(sig==SIGALRM)
	{
		read_flag=1;
	}
}

int main(int argc, char* argv[])
{
 char opt;
 mode_t mode;
 
 
 if(argc<2)
 {
	printf("Please provide the source and destination file as arguments\n");
	return -1;
 }
 else if(argc<3)
 {
	printf("No Destination file/path provided, Exiting the program.\n");
	return -1;
 } 
 else if(argc>3)
 {
	printf("Multiple files cannot be handled. Please enter only two files, Exiting the program.\n");
	return -1;
 } 


 char *dest_file = argv[argc-1];
 char *source_file= argv[argc-2];
	

	if( access( source_file, F_OK ) == -1 )
	{
	    printf("File doesn't exist, please enter a valid file name. Exiting the program\n");
	    return -1;
	}
	if( access( dest_file, F_OK ) == -1 )
 	{
		printf("Destination file %s created\n", dest_file);
		fd_to = open(dest_file, O_CREAT|O_WRONLY| O_TRUNC);    
	    	if (fd_to < 0)
		exit(0);
		lstat(source_file,c);
		mode=c->st_mode;
 		chmod(dest_file,mode); 	

 	}

	 
	fp = fopen (source_file,"r");
	if (fp < 0)
	{
		printf("Error Opening File = %s",source_file);
		exit(0);
	}
		

file_read(source_file,dest_file);
free(in_line);
}

int file_read(char source_file[],char dest_file[])
{
	
	
	if(signal (SIGALRM, catch_alarm)==SIG_ERR)
  	{
		perror("Cannot activate alarm handler\n");
		exit(0);
  	}
	if(signal(SIGINT,int_catch)==SIG_ERR)
	{
		perror("Cannot catch Signal Interrupt\n");
		exit(0);
	}	
	fd_to = open(dest_file, O_WRONLY);  
	siginterrupt(SIGALRM, 1);
	
	while(1)
	{	
		alarm(3);	
		if(read_flag==1)
		{
			user_fileread(source_file,dest_file);
			read_flag=0;
		}
		else if(read_flag==0)
		{
			file_write_user(source_file,dest_file);
			read_flag=1;
		}			
		clearerr(stdin);			
	}

fclose(fp);
close(fd_to);	
}


int file_write_user(char source_file[],char dest_file[])
{
	printf("Console Read\n");
	line_size = getline(&in_line, &line_capacity, stdin);
	
	time (&rawtime);
   	timeinfo = localtime (&rawtime);
	strftime (buffer, 100, "%a %b %d %H:%M:%S",timeinfo);
	
	if(line_size>0)
	{	
		strcat(buffer,",");
		strcat(buffer,in_line);
			
		ret_out = write (fd_to, buffer, sizeof(buffer));				
		free(in_line);
		in_line = NULL;
		strcpy(buffer,"");			
	}
}

void user_fileread(char source_file[],char dest_file[])
{
char * line = NULL;
size_t len = 0;
ssize_t read;
printf("Console Read\n");
while ((read = getline(&line, &len, fp)) != -1) 
{
	printf("%s", line);
}
pause();
}

 	
