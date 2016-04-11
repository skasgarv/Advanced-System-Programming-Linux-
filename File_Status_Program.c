#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<pwd.h>

extern int optind,opterr,optopt;
extern char **environ;
struct stat *fileStat;

void help_info();
void func_filetype(char file[],struct stat *fileStat);
void func_ownerdetails(char file[],struct stat *fileStat);
void func_writefile(char file[],struct stat *fileStat);
void func_sizeinfo(char file[],struct stat *fileStat);
 
    
int main(int argc, char* argv[])
{
struct stat fileStat ;
struct stat *c=&fileStat;    

    
int file_type=0;
int size_info=0;
int owner_details=0;
int write_file=0;
char opt;
char* file = malloc(100);
strcpy(file,argv[argc-1]);
 

if(argc<2)  	
help_info();
else
	{
		while((opt=getopt(argc,argv,":toe"))!=-1)
		{
			
			switch(opt)
			{
				case 't': 
				  file_type=1;
				  size_info=1;					
				break;
			
				case 'o': 
				  owner_details=1;
				  size_info=1;	
				break;
				
				case 'e': 
				  write_file=1;			
				break;
				
				default:
				  printf("Invalid option:Please enter valid option, Exiting the program\n");
				  free(file);
				return -1;
			}

		}


	char *file = argv[optind];
	if( access( file, F_OK ) == -1 )
	{
	    printf("File doesn't exist, please enter a valid file name. Exiting the program\n");
	    return -1;
	    free(file);
	}
 

                 
		
	if (write_file==1)
	{
		if((file_type==1) && (owner_details ==1))
		{
			func_filetype(file,c);
			func_ownerdetails(file,c);
			func_writefile(file,c);			
			func_sizeinfo(file,c);
		}	
		else if ((file_type==1) && (owner_details==0))
		{
			func_filetype(file,c);
			func_writefile(file,c);
			func_sizeinfo(file,c);				
		}
		else if ((file_type==0) && (owner_details==1))
		{
			func_ownerdetails(file,c);
			func_writefile(file,c);
			func_sizeinfo(file,c);
		}
		else
		{
			func_writefile(file,c);
			func_sizeinfo(file,c);			
				
		}
	}
	
	else
	{
		if((file_type==1) && (owner_details ==1))
		{
			func_filetype(file,c);
			func_ownerdetails(file,c);
			func_sizeinfo(file,c);
		}	
		else if ((file_type==1) && (owner_details==0))
		{
			func_filetype(file,c);
			func_sizeinfo(file,c);				
		}
		else if ((file_type==0) && (owner_details==1))
		{
			func_ownerdetails(file,c);
			func_sizeinfo(file,c);
		}
		else
		{
			func_sizeinfo(file,c);
				
		}
	}
		
	
	}
return 0;
  

    printf("File doesn't exist, please enter a valid file name. Exiting the program\n");
    return -1;
    free(file);

}
//End of main


//Functions of the program
void help_info()
{
	printf("Help Information:");
	printf("program_name -command_options -file_name\n");
	printf("\t -t: provides the file type of the entered file_name\n");
	printf("\t -o: provides the owner details of the entered file_name\n");
	printf("\t -e: writes the environment to the entered file_name\n");
}


void func_filetype(char file[],struct stat *sb)
{
 printf("Type of the file:");
 lstat(file,sb);

           switch (sb->st_mode & S_IFMT) 
	   {
           	case S_IFBLK:  printf("Block Device\n");            break;
           	case S_IFCHR:  printf("Character Device\n");        break;
           	case S_IFDIR:  printf("Directory\n");               break;
           	case S_IFIFO:  printf("FIFO/pipe\n");               break;
           	case S_IFLNK:  printf("Symlink\n");                 break;
           	case S_IFREG:  printf("Regular File\n");            break;
           	case S_IFSOCK: printf("Socket\n");                  break;
           	default:       printf("Unknown?\n");                break;
           }
}


void func_ownerdetails(char file[],struct stat *sb)
{
 	struct passwd  *pwd;
 	lstat(file,sb);

		if ((pwd = getpwuid(sb->st_uid)) != NULL)
	        	printf("Owner name of the file: %s\n", pwd->pw_name);
		else
 	       		printf("Owner name not found, Owner ID: %d\n", sb->st_uid);
}



void func_sizeinfo(char file[],struct stat *sb)
{
	lstat(file,sb);
	printf("File size:%lld bytes\n",(long long) sb->st_size);
}



void func_writefile(char file[],struct stat *sb)
{
	lstat(file,sb);
	if (sb->st_mode & S_IWUSR)
	{	
		FILE *file_name;
		file_name =fopen(file,"a");
		int i=0;
		for (i=0; environ[i]!=NULL;i++)
			fprintf(file_name,"%s\n",environ[i]);		
		fclose(file_name);
	}
	else
	{
		printf("No permission to write file. Exiting the program\n");
		exit(0);
		
	}
}

