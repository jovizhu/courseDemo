/*
 *  	File name: Encrypt.c
 *		Author: wzhu4@buffalo.edu
 *		Description: encrypted copy of an existing file using system calls for file manipulati *				on. 
 *
 *		Date: 09/15/2010
 */

#include "jovi.h"

// Encrypt Chart A->Q ..... Z->M,   a->z ... z->p
char encryChart [2][26]= {
		{'Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M'},
		{'z','x','c','v','b','n','m','a','s','d','f','g','h','j','k','l','q','w','e','r','t','y','u','i','o','p'}
};

int EncryptFunction (int, int);

/*
 *  Function name: main
 *	Input: inputfilename outputfilename 
 *	Output:  
 *	Description:  Encrypt the inputfile by the Encrypt Chart provided before.
 *				and write the data into outputfile
 *	Author: wzhu@buffalo.edu
 *	Date: 09/15/2010
 */
int main( int argc, char * argv[])
{

		if ( argc != 3 )
		{
				printf( "Encrypt: Usage: Encrypt Inputfile Outputfile \n");
				exit(0);
		}

		int inputFile;
		int outputFile;
		// open inputfile and outputfile
		inputFile = open ( argv[1], O_RDONLY);
		outputFile = open ( argv[2], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

		// do the encrypt
		EncryptFunction ( inputFile, outputFile);

		//close the inputfile and outputfile
		close ( inputFile);
		close ( outputFile);
		
//		printf ( " Encrypt: Encrypt exit \n" );
		exit(0);

}

/*
 *  Function name: EncryptFunction 
 *	Input: File descrition of inputfile and outputfile 
 *	Output:Encrypt result in outputfile 
 *	Description: Encrypt the char in inputfile and write it to outputfile 
 *	Author: wzhu@buffalo.edu
 *	Date: 09/15/2010*/

int EncryptFunction( int InputFile, int OutputFile)
{
		char ch;
		while ( read ( InputFile, &ch, 1) ==1 )
		{
				// Capital character
				if ( ch >= 65 && ch <= 90)
				{
						ch = ch -65;
						ch = encryChart[0][ch];
				}
				// little character
				else if ( ch >= 97 && ch <= 122)
				{
						ch = ch - 97;
						ch = encryChart[1][ch]; 
				}
				// other characer will not be proceeded.
				write ( OutputFile, &ch, 1);
		}

		return 0;
}
