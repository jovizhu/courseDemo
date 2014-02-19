/*
 *  LarryCurlyMoe1.c
 *  Description:
 *			Design, implement a solution for this LCM problem , which represent Larry, Curly, 
 *			and Moe. Use semaphores as the synchronization mechanism.  
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */



#include "jovi.h"

//Function for Larry, Moe and Curly
void *  LarryRun(void * args);
void *  MoeRun(void * args);
void *  CurlyRun(void * args);

//define the semphore for the threads
sem_t DigHole;
sem_t Shovel;
sem_t Moe2go;
sem_t Curty2go;

/*
 *  Function name: main
 *	Description: 
 *  Usage: LCM1 <maxunfilled number> 
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
int main(int argc, char * argv[])
{
		//check the arguments number
		if ( argc !=2)
		{
				printf("Usage: LCM1 maxunfilled \n");
				exit(0);
		}

		int maxunfilled = atoi(argv[1]);
		printf("Maximun number of unfilled holes: %d \n", maxunfilled);

		printf("Begin run\n");

		//initialize the semphore
		sem_init(&DigHole,0,maxunfilled);
		sem_init(&Shovel,0,1);
		sem_init(&Moe2go,0,0);
		sem_init(&Curty2go,0,0);

	    pthread_t *threadpoint;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


        threadpoint = malloc(sizeof(pthread_t)*3);
        memset(threadpoint,0,sizeof(pthread_t)*3);

		int LarryRc;
		int MoeRc;
		int CurlyRc;
		int rc;
		int i=0;

		//create the threads for Larry, Moe and Curly
		LarryRc = pthread_create(&threadpoint[0],&attr,LarryRun,NULL);
		MoeRc = pthread_create(&threadpoint[1],&attr,MoeRun,NULL);
		CurlyRc = pthread_create(&threadpoint[2],&attr,CurlyRun,NULL);
																									  /* Free attribute and wait for the other threads */
																									  pthread_attr_destroy(&attr);
		pthread_attr_destroy(&attr);

 		for(i=0; i<3; i++)
		{
			rc = pthread_join(threadpoint[i], NULL);
		    if (rc)
			{
		       printf("ERROR; return code from pthread_join() is %d\n", rc);
		       exit(-1);
			}
		}


		printf("End run \n");

		//Free resources for the semphores
		sem_destroy(&DigHole);
		sem_destroy(&Shovel);
		sem_destroy(&Moe2go);
		sem_destroy(&Curty2go);

}

/*
 *  Function name: LarryRun
 *	Description: 
 *  	Function for Thread Larry
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
void * LarryRun(void * args)
{
		int i = 0;
		while(i<100)
		{
		sem_wait(&DigHole);

		sem_wait(&Shovel);
		printf("Larry digs another hole # %d \n", i);
		i++;
		sem_post(&Shovel);

		sem_post(&Moe2go);

		}
		pthread_exit(NULL);

}

/*
 *  Function name: MoeRun
 *	Description: 
 *  	Function for Thread Moe
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
void *  MoeRun(void * args)
{
		int i=0;
		while(i<100)
		{
				sem_wait(&Moe2go);
				printf("Moe plants a seed in a hole # %d \n", i);
				i++;
				sem_post(&Curty2go);

		}
		pthread_exit(NULL);
}

/*
 *  Function name: LarryRun
 *	Description: 
 *  	Function for Thread Curly
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
void *  CurlyRun(void * args)
{
		int i=0;
		while(i<100)
		{
				sem_wait(&Curty2go);

				sem_wait(&Shovel);
				printf("Curty fills a planted hole #%d \n", i);
				i++;
				sem_post(&Shovel);

				sem_post(&DigHole);
		}

		pthread_exit(NULL);
}

