/*
 *  SantaElfReindeer.c
 *  Description:
 *		   Implement a solution to Santa-elf-reindeer problem we discussed in class. 
 *		   Follow the solution provided in the paper by J. Trono. 
 *  Created on: Oct 24, 2010
 *      Author: wei zhu
 *      wzhu4@buffalo.edu
 */



#include "jovi.h"

#define REINDEER 9
#define ELVES 3
#define ELVESSUM 9

//Function for Larry, Moe and Curly
void *  SantaRun(void * args);
void *  ElfRun(void * args);
void *  ReindeerRun(void * args);


//define the semphore for the threads
sem_t only_elves;
sem_t Elfmutex;
sem_t Reinmutex;
sem_t ReinWait;
sem_t Sleigh;
sem_t ElfDone;
sem_t Santa;
sem_t SantaSleep;
sem_t ElfProblem;
sem_t ReinDone;

// shared integers
int rein_ct=0;
int elf_ct=0;

/*
 *  Function name: main
 *	Description: 
 *       Usage: SER
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
 
int main(int argc, char * argv[])
{
		//check the arguments number
		if ( argc !=1)
		{
				printf("Usage: SER \n");
				exit(0);
		}

		printf("Begin run\n");
		fflush(stdout);

		//initialize the semphore
		sem_init(&only_elves,0,3);
		sem_init(&Elfmutex,0,1);
		sem_init(&Reinmutex,0,1);
		sem_init(&ReinWait,0,0);
		sem_init(&Sleigh,0,0);
		sem_init(&ElfDone,0,0);
		sem_init(&Santa,0,0);
		sem_init(&SantaSleep,0,0);
		sem_init(&ElfProblem,0,0);
		sem_init(&ReinDone,0,0);
		

		// initillize the thread for santa , elf and reindeers
	    pthread_t *SantaThread;
		pthread_t *ElfThread;
		pthread_t *ReinThread;

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


        SantaThread = malloc(sizeof(pthread_t));
		ElfThread = malloc(sizeof(pthread_t)*ELVESSUM);
		ReinThread = malloc(sizeof(pthread_t)*REINDEER);

        memset(SantaThread,0,sizeof(pthread_t));
		memset(ElfThread,0,sizeof(pthread_t)*ELVESSUM);
		memset(ReinThread,0,sizeof(pthread_t)*REINDEER);

		int SantaRc;
		int ElfRc;
		int ReinRc;
		int rc;
		int i=0;

		//create the threads for Elf, Reindeer and Santa
		for(i=0;i<ELVESSUM;i++)
		{
				ElfRc=pthread_create(&ElfThread[i],&attr,ElfRun,(void *)i);
		}

		for(i=0;i<REINDEER;i++)
		{
				ReinRc=pthread_create(&ReinThread[i],&attr,ReindeerRun,(void *)i);
		}

		SantaRc = pthread_create(&SantaThread,&attr,SantaRun,NULL);
																									  /* Free attribute and wait for the other threads */
																									  pthread_attr_destroy(&attr);

		//Elf thread																							  //free  the threads for Santa, Elf and Reindeer
		for(i=0;i<ELVESSUM;i++)
		{   
          	ElfRc=pthread_join(ElfThread[i],NULL);
			if(ElfRc)
			{
					printf("ERROR: return code from pthread_join() is %d \n",ElfRc);
			}
		}   

		// Reindeer thread
    	for(i=0;i<REINDEER;i++)
    	{   
            ReinRc=pthread_join(ReinThread[i],NULL);
			if(ReinRc)
			{
					printf("ERROR: return code from pthread_join() is %d \n", ReinRc);
			}
    	}   

		// Santa thread
    	SantaRc = pthread_join(SantaThread,NULL);

    	if (SantaRc)
		{
	       printf("ERROR; return code from pthread_join() is %d\n", rc);
	       exit(-1);
		}

		printf("End run \n");

		//Free resources for the semphores
		sem_destroy(&only_elves);
		sem_destroy(&Elfmutex);
	    sem_destroy(&Reinmutex);
        sem_destroy(&ReinWait);
	    sem_destroy(&Sleigh);
		sem_destroy(&ElfDone);
		sem_destroy(&Santa);
		sem_destroy(&SantaSleep);
	    sem_destroy(&ElfProblem);
	    sem_destroy(&ReinDone);

}

/*
 *  Function name: SantaRun
 *	Description: 
 *  	Function for Thread Santa
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
void * SantaRun(void * args)
{
		int i = 0;
		int j = 0;
		while(i<8)
		{
		printf("Santa      : I am sleeping \n");
		fflush(stdout);
		sem_wait(&SantaSleep);

		if(rein_ct == REINDEER)
		{
				printf("Santa      : I was waked up by Reindeer \n");
				fflush(stdout);
				sem_wait(&Reinmutex);
				rein_ct = 0;
				sem_post(&Reinmutex);
				
				for(j=0;j<REINDEER-1;j++)
				{
						sem_post(&ReinWait);
				}

				for(j=0;j<REINDEER;j++)
				{
						sem_post(&Sleigh);
				}
				printf("Santa      : Deliver all toys and return \n");
				fflush(stdout);

				printf("Santa      : Reindeer, You can bakc to vacation now\n");
				fflush(stdout);
				for(j=0;j<REINDEER;j++)
				{
						sem_post(&ReinDone);
				}
		}
		else
		{
				printf("Santa      : I was waked up by Elf \n");
				fflush(stdout);
				for (j=0;j<ELVES-1;j++)
						sem_post(&Santa);

				sem_wait(&Elfmutex);
				elf_ct=0;
				sem_post(&Elfmutex);

				for(j=0;j<ELVES;j++)
				{
						printf("Santa      : Solve Elf Problem \n");
						fflush(stdout);
						sem_post(&ElfProblem);
						sem_post(&ElfDone);
				}

		}

		// if you want it to run forerver ,comment the following line
		i++;
		}
		pthread_exit(NULL);

}

/*
 *  Function name: ReindeerRun
 *	Description: 
 *  	Function for Thread Reindeer
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
void *  ReindeerRun(void * args)
{
		int i=0;
		int num = (int)args;
		printf("Reindeer[%d]: I am Reineer # %d \n", num,num);
		fflush(stdout);
		while(i<2)
		{
				sleep(1);
				printf("Reindeer[%d]: Wow, it is Christmas, back to Santa\n",num);
				fflush(stdout);
				sem_wait(&Reinmutex);
				rein_ct++;
				if(rein_ct == REINDEER)
				{
						sem_post(&Reinmutex);
						printf("Reindeer[%d]: Hey, Wakeup, Santa\n", num);
						fflush(stdout);
						sem_post(&SantaSleep);
				}
				else
				{
						sem_post(&Reinmutex);
						printf("Reindeer[%d]: Oh, Wait outsied Santa's house\n",num);
						fflush(stdout);
						sem_wait(&ReinWait);
				}
				
				sem_wait(&Sleigh);
				printf("Reindeer[%d]: Fly off to deliver the toys\n",num);
				fflush(stdout);
				sem_wait(&ReinDone);

				printf("Reindeer[%d]: I will back for long vacation on the beaches\n",num);
				fflush(stdout);
				// if you want it to run forerver ,comment the following line
				i++;
		}
		pthread_exit(NULL);
}

/*
 *  Function name: ElfRun
 *	Description: 
 *  	Function for Thread Elf
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
void *  ElfRun(void * args)
{
		int i=0;
		int num=(int)args;
		while(i<2)
		{

				sleep(1);
				printf("Elf[%d]     : Oh, it is so difficult, neet to vist Santa\n",num);
				fflush(stdout);
				sem_wait(&only_elves);


				sem_wait(&Elfmutex);
				elf_ct++;
				if(elf_ct == ELVES)
				{
						sem_post(&Elfmutex);
						printf("Elf[%d]     : Hey, Wake up, Santa\n", num);
						fflush(stdout);
						sem_post(&SantaSleep);
				}
				else
				{
						sem_post(&Elfmutex);
						printf("Elf[%d]     : Wait outsied Santa's house\n",num);
						fflush(stdout);
						sem_wait(&Santa);
				}

				printf("Elf[%d]     : Ask a question and wait to be solved\n",num);
				fflush(stdout);
				sem_wait(&ElfProblem);

				sem_wait(&ElfDone);
				printf("Elf[%d]     : My Problem was solved\n",num);
				fflush(stdout);

				printf("Elf[%d]     : I will back to do the toys\n",num);
				fflush(stdout);
				sem_post(&only_elves);
				// if you want it to run forerver ,comment the following line
				i++;
		}

		pthread_exit(NULL);
}

