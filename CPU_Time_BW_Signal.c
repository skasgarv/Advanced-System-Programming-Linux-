#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include<signal.h>
#include<sys/signal.h>
clock_t start_t, end_t, total_t;
double value;
int startflag = 0;
int endflag=0;
static void sig_stop(int sig)
{
	start_t = clock();
	startflag=1;
	signal(SIGTSTP,sig_stop);	
}
static void sig_start(int sig)
{
	if(startflag==1)
	{
		end_t=clock();	
		endflag=1;
	}
	
}


int main(int argc, char* argv[])
{
   
   if(signal(SIGTSTP,sig_stop)==SIG_ERR)
   {
   	printf("signal STOP cannot be catched\n");    
   }
   if(signal(SIGCONT,sig_start)==SIG_ERR)
   {
   	printf("signal CONT cannot be catched\n");
   }
   while(1)
   {
	if(startflag==1)
	{
		signal(SIGTSTP,sig_stop);
	}
	if(endflag==1)
	{	
		signal(SIGCONT,sig_start);
	}
	if(startflag && endflag ==1)
	{
		value = ((double)(end_t-start_t))/(double)CLOCKS_PER_SEC;
		printf("Total CPU Time Between Signals in Nanoseconds= %f\n",(value*1E9));	
		_exit(0);
	}
	pause();
 
    }
   return(0);
}
