/* 
 * 	File name : TimeEncrypt.c 
 * 	Author: wzhu4@buffalo.edu 
 *	Description:    This program will calculate the tree types of encryption and calculate the *			time of them.
 *	Date: 09/20/2010 
 * 
 */
#include "jovi.h"

/*
 *  Function name: main
 *	Input: inputfilename outputfilename
 *	Output: printf the time of each program 
 *	Description: Fork 3 progress and calculate each runtime.
 *	Author: wzhu@buffalo.edu
 *	Date: 09/20/2010
 */
int main(int argc, char * argv [])
{
	int pid;
	int pid_2;
	int pid_3;

	// time 1
	double time_seconds;
	struct timeval time_seconds_start;
	struct timeval time_seconds_end;

	// time 2
	double time_seconds2;
	struct timeval time_seconds2_start;
	struct timeval time_seconds2_end;

	// time 3
	double time_seconds3;
	struct timeval time_seconds3_start;
	struct timeval time_seconds3_end;

	if ( argc != 3)
	{
			printf(" Usage: TimeEncrypt inputfile outpufile \n");
	}

	gettimeofday(&time_seconds_start,NULL);

	//printf("Time 1 start: %d :\n", time_seconds_start.tv_usec);

	pid = fork();


	if ( pid == 0)
	{
			//printf(" Running Encrypt \n ");
			//sleep(1);
			execl("./Encrypt", "./Encrypt",argv[1],argv[2],(char * ) 0);
	}
	else if ( pid > 0 )
	{
		waitpid ( pid, NULL, 0);
		gettimeofday(&time_seconds_end,NULL);
		// time 1
		time_seconds =( time_seconds_end.tv_sec+(time_seconds_end.tv_usec/1000000.0)) -( time_seconds_start.tv_sec+(time_seconds_start.tv_usec/1000000.0));


		gettimeofday(&time_seconds2_start, NULL);
//		printf("Time 2 start: %d :\n", time_seconds2_start.tv_usec);

		pid_2=fork();

		if ( pid_2 == 0 )
		{
			//	printf( " Running ForkEncrypt \n " );
			//	sleep(2);
				execl ("./ForkEncrypt", "./ForkEncrypt", argv[1], argv[2],(char *) 0);
		}
		else if ( pid_2 >0 )
		{
				waitpid ( pid_2, NULL, 0);
				gettimeofday(&time_seconds2_end,NULL);
//				printf("Time 2 end: %d :\n", time_seconds2_end.tv_usec);
				// the seconde time
				time_seconds2 =( time_seconds2_end.tv_sec+(time_seconds2_end.tv_usec/1000000.0)) -( time_seconds2_start.tv_sec+(time_seconds2_start.tv_usec/1000000.0));

				gettimeofday(&time_seconds3_start ,NULL);

				pid_3= fork();
				if ( pid_3 == 0 )
				{
						//printf ( " Running PipeEncrypt \n " );
						//sleep(3);
						execl ( "./PipeEncrypt", "./PipeEncrypt", argv[1],argv[2], (char * ) 0);
				}
				else if ( pid_3 > 0 )
				{
						waitpid ( pid_3, NULL, 0);
						gettimeofday(&time_seconds3_end, NULL);
//						printf("Time 3 end: %d :\n", time_seconds3_end.tv_usec);
						// the time 3
						time_seconds3 =( time_seconds3_end.tv_sec+(time_seconds3_end.tv_usec/1000000.0)) -( time_seconds3_start.tv_sec+(time_seconds3_start.tv_usec/1000000.0));

						// print result
						printf( "************************************************\n");
						printf(" Encrypt time is %f seconds \n \n ", time_seconds);
						printf(" ForkEncrypt time is %f seconds \n \n ", time_seconds2);
						printf(" PipeEncrypt time is %f seconds \n \n ", time_seconds3);

				}
				else
				{
						printf(" Fork Error child 3\n ");
				}

		}
		else 
		{
				printf(" Fork Error child 2\n ");
		}
	}
	else 
	{
			printf("Fork Error child 1  %d \n", getpid());
	}
}




