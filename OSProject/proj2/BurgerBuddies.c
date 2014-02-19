/*
 * 		BurgerBuddies.c
 *
 *      Description:     Design, implement a solution for the BurgerBuddies problem specified below.
 *  	Created on: Oct 24, 2010
 *      Author: wei zhu
 *      		wzhu4@buffalo.edu
 */



#include "jovi.h"


#define CUSTOMERNUM 100

// Function for Cooker, Casher and Customer
void *  CookerRun(void * args);
void *  CasherRun(void * args);
void *  CustomerRun(void * args);


//define the semphore for the threads
sem_t Customer2Casher;
sem_t Burger;
sem_t Rack;


/*
 *  Function name: main
 *	Description: 
 *  Usage: BBC <cooker num>  <casher num> <racksize num>
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
 int main(int argc, char * argv[])
{
		// check the argument number

		if ( argc !=4)
		{
				printf("Usage: BBC cooker casher racksize \n");
				exit(0);
		}

		int cookernum = atoi(argv[1]);
		int cashernum = atoi(argv[2]);
		int racksize  = atoi(argv[3]);

		printf("cookers[%d],  cahsers[%d], customers[%d],   racksize[%d] \n",cookernum,cashernum,CUSTOMERNUM,racksize);
		fflush(stdout);

		printf("Begin run\n");
		fflush(stdout);

		//init the semphore
		sem_init(&Customer2Casher,0,0);
		sem_init(&Burger,0,0);
		sem_init(&Rack,0,racksize);

	    pthread_t *CasherThread;
		pthread_t *CustomerThread;
		pthread_t *CookerThread;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


        CustomerThread = malloc(sizeof(pthread_t)*CUSTOMERNUM);
        memset(CustomerThread,0,sizeof(pthread_t)*CUSTOMERNUM);

		CasherThread = malloc(sizeof(pthread_t)*cashernum);
		memset(CasherThread,0,sizeof(pthread_t)*cashernum);

		CookerThread = malloc(sizeof(pthread_t)*cookernum);
		memset(CookerThread,0,sizeof(pthread_t)*cookernum);

		int CookerRc;
		int CasherRc;
		int CustomerRc;
		int i=0;

		//creat the threads for Cooker, Casher and Customer
		for(i=0;i<cookernum;i++)
		{
			CookerRc = pthread_create(&CookerThread[i],&attr,CookerRun,(void *)i);
		}
		for(i=0;i<cashernum;i++)
		{
			CasherRc = pthread_create(&CasherThread[i],&attr,CasherRun,(void *)i);
		}
		for(i=0;i<CUSTOMERNUM;i++)
		{
			CustomerRc = pthread_create(&CustomerThread[i],&attr,CustomerRun,(void *)i);
		}
																									  /* Free attribute and wait for the other threads */
		pthread_attr_destroy(&attr);

 		for(i=0; i<cookernum; i++)
		{
			CookerRc = pthread_join(CookerThread[i], NULL);
		    if (CookerRc)
			{
		       printf("ERROR; return code from pthread_join() is %d\n", CookerRc);
		       exit(-1);
			}
		}

		for(i=0; i<cashernum; i++)
		{   
		    CasherRc = pthread_join(CasherThread[i], NULL);
			if (CasherRc)
			{   
		       printf("ERROR; return code from pthread_join() is %d\n", CasherRc);
		       exit(-1);
		    }   
		} 

		for(i=0; i<CUSTOMERNUM; i++)
		{   
		    CustomerRc = pthread_join(CustomerThread[i], NULL);
		    if (CustomerRc)
		    {   
		       printf("ERROR; return code from pthread_join() is %d\n", CustomerRc);
		       exit(-1);
		    }   
		}
		
		printf("End run \n");
		//free resources for the semphore
		sem_destroy(&Burger);
		sem_destroy(&Rack);
		sem_destroy(&Customer2Casher);

}

/*
 *  Function name: CustomerRun
 *	Description: 
 *  	Function for Thread Customer
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
void * CustomerRun(void * args)
{
		int num = (int) args;
		sleep(num);

		printf("Custom[%d]: Customer #%d come in\n", num,num);
		fflush(stdout);

		sem_post(&Customer2Casher);

		pthread_exit(NULL);

}

/*
 *  Function name: CasherRun
 *	Description: 
 *  	Function for Thread Casher
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
void *  CasherRun(void * args)
{
		int i=0;
		int num=(int) args;

		while(i<100)
		{
				//sleep(1);
				//printf("Casher[%d]: Casher #%d is sleeping\n",num,num);
				fflush(stdout);
				sem_wait(&Customer2Casher);

				printf("Casher[%d]: Accept an order\n",num);
				fflush(stdout);
				//i++;

				sem_wait(&Burger);
				printf("Casher[%d]: takes a burger to customer\n",num);
				fflush(stdout);
				sem_post(&Rack);

		}
		pthread_exit(NULL);
}

/*
 *  Function name: CookerRun
 *	Description: 
 *  	Function for Thread Cooker
 *  Created on: Oct 24, 2010
 *  Author: wei zhu
 *          wzhu4@buffalo.edu
 */
void *  CookerRun(void * args)
{
		int i=0;
		int num=(int)args;

		while(i<100)
		{
				sleep(2);
				sem_wait(&Rack);
				printf("Cooker[%d]: Make a burger\n",num);
				fflush(stdout);
				//i++;
				sem_post(&Burger);
		}

		pthread_exit(NULL);
}

