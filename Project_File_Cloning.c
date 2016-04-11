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

int chmod(const char *path,mode_t mode);
int utime(const char *file, const struct utimbuf *time);
extern int optind,opterr,optopt;
extern char **environ;

int file_clone(char source_file[],char dest_file[]);
void func_filewrite(char dest_file[],char source_file[]);

struct stat buf ;
struct stat *c=&buf; //pointer to buf    
int file_overwrite=0;


int main(int argc, char* argv[])
{
 char opt;
 
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


	while((opt=getopt(argc,argv,":f"))!=-1)
	{
		switch(opt)
		{
			case 'f': 
			  file_overwrite=1;
			break;
			
			default:
			  printf("Invalid option:Please enter valid option, Exiting the program\n");
			return -1;
		}

	}


 char *dest_file = argv[argc-1];
 char *source_file= argv[argc-2];
	

	if( access( source_file, F_OK ) == -1 )
	{
	    printf("File doesn't exist, please enter a valid file name. Exiting the program\n");
	    return -1;
	}
	

	lstat(dest_file,c);
	if(c->st_mode & S_IFMT)
	{
		if(S_IFDIR &c->st_mode)
		{		
			strcat(dest_file,source_file);
		}	
	}    


	  func_filewrite(dest_file,source_file);
	  

}

void func_filewrite(char dest_file[],char source_file[])
{
 if( access( dest_file, F_OK ) == -1 )
 {
	printf("Destination file %s created\n", dest_file);
 }
 else
 {
	if(file_overwrite==1)
	{
		printf("Destination File Exists,Overwriting Destination File.\n");
	}
	else
	{
		printf("No permission to write the Destination file. Exiting the program\n");
		exit(0);
	}
 } 
   
 int fd_to, fd_from;
 char buf[4096];
 ssize_t nread;
 mode_t mode;
    
 fd_from = open(source_file, O_RDONLY);
    if (fd_from < 0)	
	 exit(0);
    

 fd_to = open(dest_file, O_CREAT|O_WRONLY| O_TRUNC);    
    if (fd_to < 0)
	exit(0);
	

 lstat(source_file,c);
 mode=c->st_mode;
 chmod(dest_file,mode); 	
       
 while (nread = read(fd_from, buf, sizeof buf), nread > 0)
 {
   char *out_ptr = buf;
   ssize_t nwritten;
       do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            
           } while (nread > 0);
 }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            exit(0);
        }
        close(fd_from);
    }
	
 file_clone(source_file,dest_file);

}


int file_clone(char source_file[],char dest_file[])
{
  char destination[100];
  strcpy(destination,dest_file);  
  struct utimbuf new_times;
  struct utimbuf d;//pointer to buf
  struct stat test;    

  stat(source_file,&test);  
	
  d.actime = test.st_atime; /* keep atime unchanged */
  d.modtime = test.st_mtime;    /* set mtime to current time */
  
  if(utime(destination,&d) != 0)
  {
	printf("Error %s",strerror(errno));
  }
  printf("Cloning complete.\n");
	return 0;
  
  
}
