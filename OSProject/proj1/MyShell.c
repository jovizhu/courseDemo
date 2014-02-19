/*
 * 	File nameMyShell.c
 * 	Author: wzhu4@buffalo.edu 
 *	Descritpin:  This program will provide its own prompt to the user, read the 
 *		command from the input and execute the command. Let the shell allow arguments 
 *		to the commands.  
 *	Date: 09/27/2010
 */
#include "jovi.h"

//This function parse the command line and separate the command into to arrays.
int parseLine(char* line, char* command_array[]);


/*
 *   Function name: main
 *	 Input: void
 *	 Output: result of the command execvp result.
 *	 Description: Receive the input from Standin and execvp the command , output the result 
 *	 to the stdout
 *	 Author: wzhu@buffalo.edu
 *	 Date: 09/27/2010
*/
int main(int argc, char * argv[])
{
		int pid;
		int flag, i;
		char commandline [999];
		char * commandarray[100];
		int commandcount;
		memset ( commandline, 0, 50);

	while ( 1 )
		{
			printf("Input your command: ");
			memset(commandline,0,999);
			// get command from stdin
			fgets(commandline,sizeof(commandline),stdin);
			flag =0;
			// fgets has an issue is that it will get the \n from the stdin
			// this will cause the parse command failed if it not be removed
			// Remove the \n from the commandline
			for (i=0;((i<sizeof(commandline)) && ( flag == 0 ));i++)
				 {
					if (commandline[i]=='\n')
						{
							flag = 1;
						 }
				 }
			 commandline[i-1]=0;

			 //this is not necessary, but to cofirm that the commandline does not out of range
			 commandline[999] = '\0';

			 // if input exit, exit
				if ( strcmp ( commandline, "exit") ==0 )
				{
						exit(0);
				}


	    commandcount =parseLine ( commandline, commandarray);

		// Important, without it, execvp will not run correctly
		commandarray[commandcount]=NULL;

		pid = fork();

		if ( pid ==0 )
		{
			if (execvp(commandarray[0],commandarray) == -1)
			{
				printf ("MyShell: Command Error");
				exit (0);
			}
		}
		else if ( pid >0 )
		{
				waitpid( pid, NULL, 0);

		}


}
}

/*
 *	Function: ParseLine
 *	Input: command line
 *	Output: commandArray commandcount
 *	Description: parse the commandline and store the command into command_arry
 *	Author: wzhu4@buffalo.edu
 *	Date: 09/28/2010   */
int parseLine(char * line, char * command_array[]) {
		char * p;
		p=NULL;
		int count = 0;
		p = strtok(line, " ");
		// parse the command until the end or meet a '|'
		while (p && strcmp(p,"|") != 0) {
				command_array[count] = p;
				printf("split command is %s \n", command_array[count]);
				count++;
				p = strtok(NULL," ");
		}
		// return the command count
		return count;
}
