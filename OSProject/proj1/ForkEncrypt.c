/*
* Program: ForkEncrypt.c
* 
* Requires: jovi.h
* 
* Description:
* 
*     "ForkEncrypt" spawn a new process using fork system call. Then use execl to execute "Encrypt" program. The source and destination file names presented as command-line arguments should be passed to execl as system call arguments. The main process waits for completion of copy operation using wait system call.
*  
* To execute:
* 
*     From command line type> ForkEncrypt <inputFile> <outputFile>
* 
* Author: Wei Zhu (Jovi)  wzhu4@buffalo.edu
* Date : 09/14/2010
*/
#include "jovi.h"

/*
 *   Function name: main
 *	Input: inputiflename outputfilename 
 *	Output: result of the command "Encrypt inputfile outputfile" result.
 *	Description: Fork a process and execute the command "Encrypt inputfile outputfile" 
 *	Author: wzhu@buffalo.edu
 *	Date: 09/14/2010
 */
int main( int argc, char * argv [] )
{
		int pid;

		if ( argc != 3 )
		{
				printf("ForkEncrypt: Usage: ForkEncrypt inputFile outputFile \n");
				exit(0);
		}

		// fork the process
		pid = fork();

		if( pid == 0 )
		{
				//printf("ForkEncrypt: Child process %d is running \n",getpid());
				execl("./Encrypt", "./Encrypt",argv[1], argv[2],(char *)0);
				printf("ForkEncrypt: Execl Error \n");

		}
		else if ( pid > 0)
		{
				wait(NULL);
			//	printf("ForkEncrypt: Parent process %d is Running \n", getpid());
			//	printf("ForkEncrypt: Encrypt %s  %s  has been completed \n",argv[1],argv[2]);
		}
		else
		{
				printf("ForkEncrypt: Fork Error \n");
		}

}

		
