/*
 *  LarryCurlyMoe2.c
 *  Description:
 *			Design, implement a solution for this LCM problem , which represent Larry, Curly, 
 *			and Moe. Use semaphores as the synchronization mechanism.  And add the semphore BulbePlanter 
 *			in this program.
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */

#include "jovi.h"


#define MAXHOLE 5


// Function for Larry, Moe and Curly
void *  LarryRun(void * args);
void *  MoeRun(void * args);
void *  CurlyRun(void * args);


//define the semphore for the threads
sem_t DigHole;
sem_t Shovel;
sem_t Moe2go;
sem_t Curty2go;
sem_t BulbPlanter;

/*
 *  Function name: main
 *	Description: 
 *  Usage: LCM2 <maxunfilled number> 
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
 
int main(int argc, char * argv[])
{
		// check the argument number
		if ( argc !=2)
		{
				printf("Usage: LCM2 maxunfilled \n");
				exit(0);
		}

		int maxunfilled = atoi(argv[1]);
		printf("Maximun number of unfilled holes: %d \n", maxunfilled);

		printf("Begin run\n");

		//init the semphore
		sem_init(&DigHole,0,maxunfilled);
		sem_init(&Shovel,0,1);
		sem_init(&Moe2go,0,0);
		sem_init(&Curty2go,0,0);
		sem_init(&BulbPlanter,0,1);

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

		//creat the threads for Larry, Moe and Curly
		LarryRc = pthread_create(&threadpoint[0],&attr,LarryRun,NULL);
		MoeRc = pthread_create(&threadpoint[1],&attr,MoeRun,NULL);
		CurlyRc = pthread_create(&threadpoint[2],&attr,CurlyRun,NULL);
																									  /* Free attribute and wait for the other threads */
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
		//free resources for the semphore
		sem_destroy(&DigHole);
		sem_destroy(&Shovel);
		sem_destroy(&Moe2go);
		sem_destroy(&Curty2go);
		sem_destroy(&BulbPlanter);

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

				sem_wait(&BulbPlanter);

				printf("Moe plants a bulb in a hole #%d \n", i);
				i++;

				sem_post(&BulbPlanter);

				sem_post(&Curty2go);

		}
		pthread_exit(NULL);
}

/*
 *  Function name: CurlyRun
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
				
				sem_wait(&BulbPlanter);
				printf("Curly add the fertilizer to hole #%d \n",i);
				sem_post(&BulbPlanter);


				sem_wait(&Shovel);
				printf("Curly fills a planted hole #%d \n", i);
				i++;
				sem_post(&Shovel);

				sem_post(&DigHole);
		}

		pthread_exit(NULL);
}

