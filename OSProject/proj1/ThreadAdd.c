/*
    * 	File name : DupShell.c 
	* 	Author: wzhu4@buffalo.edu 
	*	Description: This application create an n-way concurrency using n threads. Each thread will wait for a random time and generate a random and synchronize with the main controller thread. The main thread then accumulates (sums up) all the returned values and prints them out. You may appropriate synchronization primitive to control the threads. (Barrier primitive?)

	*	Date: 09/28/2010
	*
  */

#include "jovi.h"


//This function is called by each thread and generate the random number
void * generate_number(void * args);

/*
 *  Function name: main
 *	Input: Thread Number 
 *	Output: Sum of the number which created by the thread.
 *	Description: Create the threads and sum of the random number
 *	Author: wzhu4@buffalo.edu
 *	Date: 09/28/2010
 */

int main( int argc, char * argv[])
{
		if (argc != 2)
		{
				printf("Usage: ThreadAdd Number");
				exit(0);
		}
		
		int threadnum=atoi(argv[1]);
		if (threadnum > 20)
		{
				printf("The maximum number of thread is 20");
				exit(0);
		}

		pthread_t *threadpoint;


		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


		threadpoint = malloc(sizeof(pthread_t)*threadnum);
		memset(threadpoint,0,sizeof(pthread_t)*threadnum);

		int i;
		int j;
		int rc;
		int Sum;

		Sum =0;
		void * status;

		

		for ( i=0; i<threadnum; i++)
		{
				rc = pthread_create(&threadpoint[i],&attr,generate_number,NULL);
		}

		/* Free attribute and wait for the other threads */
		pthread_attr_destroy(&attr);


	   pthread_attr_destroy(&attr);

	   // Ref to https://computing.llnl.gov/tutorials/pthreads/
	   // POSIX Threads Programming
	   for(j=0; j<threadnum; j++) {
	        rc = pthread_join(threadpoint[j], &status);
		  if (rc) {
					     printf("ERROR; return code from pthread_join() is %d\n", rc);
					     exit(-1);
						 }
	    Sum = Sum + (int)status;
	   }
		printf("The SUm is %d \n", Sum);
}

/*
 *  Function name: main
 *	Input: void 
 *	Output: random number 
 *	Description:  Create a random number and return it to the main function 
 *	Author: wzhu4@buffalo.edu
 *	Date: 09/28/2010
 */
void * generate_number(void * args)
{
		//int my_Number = 1+(int)(10.0*(rand()/(RAND_MAX + 1.0)));
		int my_Number = rand() % 10;
		printf(" IN the thread, the random number is %d \n ", my_Number);
		usleep(my_Number*1000);
		pthread_exit((void *) my_Number);
}
