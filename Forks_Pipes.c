#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{
	char *input = argv[argc-1];
	if(argc<2)
	{
		printf("Supply pattern for 'grep' command.\n");
	}
	else if(argc>2)
	{
		printf("Too many arguments, Pass in a single argument.\n");
	}
	
	int pfd[2]; /* Pipe file descriptors */
	if (pipe(pfd) == -1) /* Create pipe */
		perror("pipe");

	switch (fork()) 
	{
		case -1:
			perror("fork");

		case 0: 
			if (close(pfd[0]) == -1) /* Read end is unused */
				perror("close 1");

			if (pfd[1] != STDOUT_FILENO) 
			{ 
				if (dup2(pfd[1], STDOUT_FILENO) == -1)
					perror("dup2 1");
				if (close(pfd[1]) == -1)
					perror("close 2");
			}
			
			execlp("/bin/ls", "ls", (char *) NULL); /* Writes to pipe */
			

		default: 
			break;
	}

	switch (fork()) 
	{
		case -1:
			perror("fork");

		case 0: 
			if (close(pfd[1]) == -1) /* Write end is unused */
				perror("close 3");
			if (pfd[0] != STDIN_FILENO) 
			{ 
				if (dup2(pfd[0], STDIN_FILENO) == -1)
					perror("dup2 2");
				if (close(pfd[0]) == -1)
					perror("close 4");
			}
			
			execlp("grep", "grep", input, (char *) NULL); /* Reads from pipe */
			

		default: 
		break;
	}



if (close(pfd[0]) == -1)
perror("close 5");
if (close(pfd[1]) == -1)
perror("close 6");
if (wait(NULL) == -1)
perror("wait 1");
if (wait(NULL) == -1)
perror("wait 2");
exit(EXIT_SUCCESS);
}
