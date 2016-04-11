#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */
#include <time.h>
#include <sys/signal.h>
double value;
pid_t childpid; 
int retval;     
int status;     
int cldcont=0, cldstop=0,start_flag=1,end_flag=0;;
clock_t start_val, end_val, total_val;
clock_t total;
void sig_handler(int signum, siginfo_t *siginfo, void *ucontext)
{
if(cldcont ==1 && cldstop==0)
	    {
		printf("Child has not been stopped\n");
		exit(EXIT_FAILURE);
	    }
if(signum==SIGCHLD)
{		
	if(siginfo->si_code == CLD_CONTINUED)	
	{	
		printf("Child continued by SIGCONT\n");
			
	}
	else if(siginfo->si_code == CLD_DUMPED)
	{		
		printf("Child abnormal termination (core dump)\n");
		end_flag=1;
	}	
	else if(siginfo->si_code == CLD_EXITED)	
	{	
		printf(" Child exited\n");
		end_flag=1;	
	}	
	else if(siginfo->si_code == CLD_KILLED)	
	{
		printf("Child abnormal termination (no core dump)\n");
		end_flag=1;	
	}	
	else if(siginfo->si_code == CLD_STOPPED)	
	{	
		cldstop=1;	
		printf("Child stopped\n");
	}	
	else if(siginfo->si_code == CLD_TRAPPED)	
		printf("Traced child stopped\n");
}
}

int main(int argc, char* argv[])
{
struct sigaction s;
s.sa_sigaction = sig_handler;
sigemptyset(&s.sa_mask);
s.sa_flags = SA_RESTART|SA_SIGINFO;
	

double value;
struct timeval start_value, end_value;
    childpid = fork();
    start_flag=1;
    
    if (childpid >= 0) 
    {
	if (childpid == 0) 
        {
	    start_val = clock(); 	 	        	
	    while(1);					
        }
        else 
        {
	    
            if(sigaction(SIGCHLD, &s, NULL)==-1)
	    {
		perror("signal");
		exit(EXIT_FAILURE);
            }
	    wait(&status);
	    end_val = clock();				
	    
	    
	    if(start_flag==1 && end_flag==1)
	    {
		
		clock_t diff = end_val - start_val;
		long double temp = (long double) diff/sysconf(_SC_CLK_TCK);
		printf("Total time taken by child: %i seconds and %i microseconds.\n", (int)temp,(int)	((temp*1E6)-((int)temp*1E6)));				
	    	exit(0);	    
	    }	
        }
    }
    else 
    {
        perror("fork"); 
        exit(0); 
    }
}


