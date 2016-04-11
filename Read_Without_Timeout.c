#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t count = 1;
int alarm_act=0;
char* in_line = NULL;
size_t line_capacity = 0;
int line_size;

void do_execution();


void catch_alarm (int sig)
{	
  if(count<3)
  {	  
	printf("Time Expired, Timeout(%d)\n",count);
	printf("Please Enter Some text within 3 seconds:\n");	
 	alarm(3);
 	count = count+1;
  }
  else
  {	
	alarm(0);
	printf("3 Timeouts Encountered, Exiting the program.\n");
	exit(0);
  }
}

void do_execution()
{
	printf("Please Enter Some text within 3 seconds:\n");	
	line_size = getline(&in_line, &line_capacity, stdin);	
	printf("You Wrote:%s\n", in_line);
	
}

int
main (void)
{
  if(signal (SIGALRM, catch_alarm)==SIG_ERR)
  {
	perror("Cannot activate alarm handler\n");
	exit(0);
  }	
  while (1)
	{
			alarm (3);
			do_execution();
	}
  free(in_line);
  return EXIT_SUCCESS;
}
