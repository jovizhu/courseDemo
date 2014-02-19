/*
   * Program: PipeEncrypt.c
   * 
   * Requires: jovi.h
   * 
   * Description:
   * 
   *     This Program forks two processes.
   *	One reads from source file and write into the pipe.
   *	The other read from the pipe and write to the destination file after encryption.
   *  
   * To execute:
   * 
   *     From command line type> PipeEncrypt <inputFile> <outputFile>
   * 
   * Written by: Wei Zhu (Jovi) 
   * Date : 09/26/2010
  */



#include "jovi.h"

// Encrypt Chart
char encryChart [2][26] = {
		{'Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M'},
		{'z','x','c','v','b','n','m','a','s','d','f','g','h','j','k','l','q','w','e','r','t','y','u','i','o','p'}
};

/*
 *  Function name: main
 *	Input: Inputfilename Outputfilename
 *	Output: Encrypt result of inputfile 
 *	Description: read from the inputfile and send to outputfile  by pipes.
 				Encrypt the data and restore it into the outputfile
 *	Author: wzhu@buffalo.edu
 *	Date: 09/28/2010
 */
int main( int argc, char * argv[])
{
		int encryptFilePipes[2];
		char readch;
		char writech;
		int inputFile;
		int outputFile;
		pid_t ReadPid;
		pid_t WritePid;
		
		if ( argc !=3 )
		{
				printf ( "PipeEncrypt: Usage: PipeEncrypt Inputfile Outputfile \n " );
		}

		// create the pipes
		if(pipe(encryptFilePipes)==0)
		{
				ReadPid = fork();

				if ( ReadPid ==0)
				{
						close ( encryptFilePipes[1]);
						outputFile = open ( argv[2], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
						// read from the pipe and write it to the output file
						while (read(encryptFilePipes[0], &writech,1) == 1 )
						{
								write ( outputFile, &writech, 1);

						}
				}
				else if ( ReadPid > 0 )
				{
						WritePid = fork();
						if ( WritePid == 0)
						{
								close ( encryptFilePipes[0]);
								inputFile = open ( argv[1], O_RDONLY);

								// Read from the inputfile, encrypt the date and write it to the pipe
								while ( read ( inputFile, &readch, 1) == 1 )
								{
										if ( readch >=65 && readch <=90)
										{
											readch = readch -65;
											readch = encryChart[0][readch];
										}
										else if ( readch>= 97 && readch <= 122)
										{
											readch = readch -97;
											readch = encryChart[1][readch];
										}
										write ( encryptFilePipes[1],&readch,1);
								}
						}
						else if ( WritePid > 0 )
						{
							waitpid (WritePid, NULL,0);
							close (inputFile);
							close (outputFile);

					//		printf(" PipeEncrypt: PipeENcrypt exit \n ");
						}
						else 
						{
								printf("PipeEncrypt: Fork Error \n ");
						}
			}
				else
				{
						printf("PipeEncrypt: Fork Error \n ");
				}

		}
}


