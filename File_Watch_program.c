#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<sys/inotify.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<utime.h>
#include<fcntl.h>
#include<errno.h>
#include<dirent.h>
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define buf_len     ( 1024 * ( EVENT_SIZE + 16 ) )
DIR *d;
struct dirent *dir;


int main( int argc, char *argv[] ) 
{
  int x;
  int fd;
  int wd;
  char buf[buf_len];
  char *dest_path = argv[argc-1];
  char dest_file[100];
  char *p;
  struct stat s;
  if(argc<2)
  {
	printf("List of Current Working Directory Files\n");
  	d = opendir(".");
  }
  else if(argc==2)
  {	
	d=opendir(dest_path);
	
  }
  else
  {
	d=0;
	printf("Multiple paths not allowed, Enter a single destination folder to monitor\n");
	return -1;

  }

  	if (d)
  	{
    		while ((dir = readdir(d)) != NULL)
    		{
      			stat(dir->d_name,&s);
                          if(! S_ISDIR(s.st_mode))
                            printf("%s\n", dir->d_name);
    		}

  	  	closedir(d);
  	}
	else
	{
		printf("Not a valid directory, Please enter a valid directory name.\n");
		exit(0);
	}


	printf("\nEnter a file to be monitored:");
	scanf("%s",dest_file); 	
	strcat(dest_path,dest_file);
	int fdt=open(dest_path,O_RDONLY);
	if(fdt<0)
	{
		printf("File does not exist, Please Enter valid file name\n");
		return -1;
  	}
  fd = inotify_init();
  printf("Dest path is %s",dest_path);
  wd = inotify_add_watch(fd,dest_path,IN_ACCESS|IN_MODIFY | IN_CREATE | IN_DELETE );  
  if(wd==-1)
	printf("error = %s",strerror(errno));
  printf("dest file is %s\n", dest_file);	
  printf("wd = %d\n",wd);
  
  while (1) 
  {
	x=read(fd,buf,buf_len);
	for(p=buf;p<buf+x;)
	{
		struct inotify_event *event = (struct inotify_event *)p;
		if ( event->mask & IN_ACCESS )
	        {
        	  printf( "The file %s was accessed.\n", event->name );
      		}
      		else if ( event->mask & IN_DELETE )
		{
       	 	  printf( "The file %s was deleted.\n", event->name );		
        	}
      		else if ( event->mask & IN_MODIFY ) 
		{
        	  printf( "The file %s was modified.\n", event->name );
        	}
		else if ( event->mask & IN_CREATE ) 
		{
        	  printf( "The file %s was created.\n", event->name );
        	}
		
		p+=sizeof(struct inotify_event)+event->len;	
  }
}

 }
