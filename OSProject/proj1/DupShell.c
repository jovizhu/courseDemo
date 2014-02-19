/*
 * 	File name : DupShell.c
 * 	Author: wzhu4@buffalo.edu
 *	Description:  This program will provide its own prompt to the user, read the 
 *		command from the input and execute the command. Let the shell allow arguments 
 *		to the commands. And this program also has the ability to execute the '|' 
 *	Date: 09/28/2010
 *
 * */



#include "jovi.h"

// This function parse the command line and separate the command into to char* arrays.
int parseLine(char* line, char* command_arrayA[], char * command_arrayB[]);


/*
*   Function name: main
*	Input: void
*	Output: result of the command execvp result.
*	Description: Receive the input from Standin and execvp the command , output the result 
*	to the stdout
*	Author: wzhu@buffalo.edu
*	Date: 09/28/2010
*/

int main(void) {
	int DupPipes[2];
	int pid;
	int i,flag=0;
	char commandline[999];
	char * line;
	char * commandarrayA[10];
	char * commandarrayB[10];
	int RedirectPipe = 0;

	// command line recevied from stdin
	while (1) {

		printf("Input your command: ");
		memset(commandline, 0, 999);
		fgets(commandline,sizeof(commandline),stdin);
		printf("%s \n", commandline);
		flag =0;
		// fgets has an issue is that it will get the \n from the stdin
		// this will cause the parse command failed if it not be removed
		// Remove the \n from the commandline
		for (i=0;((i<sizeof(commandline)) && ( flag == 0 ));i++)
		{
				if (commandline[i]=='\n')
				{
						flag = 1;
//							printf("I = %d \n", i);
				}
				
		}
		commandline[i-1]=0;

		if (strcmp(commandline, "exit") == 0) {
			exit(0);
		}

		// the flag for the existen of '|'
		RedirectPipe = parseLine(commandline, commandarrayA, commandarrayB);


		pipe(DupPipes);

		// fork the first process

		pid = fork();

		if (pid == 0) {

			//printf("Process A is running flag is %d \n ",RedirectPipe);
			if (RedirectPipe == 1) {
				close(DupPipes[0]);
				dup2(DupPipes[1], STDOUT_FILENO);
				close(DupPipes[1]);
				//printf("Process A is running flag is %d \n ",RedirectPipe);
			}

			execvp(commandarrayA[0], commandarrayA );
		} else if (pid > 0) {

			waitpid(pid, NULL, 0);

			if (RedirectPipe == 1) {

				//printf("Process parent or B is running flag is %d \n ",RedirectPipe);
				pid = fork();

				if (pid == 0) {
					//printf("Process B is running flag is %d \n ",RedirectPipe);
					close(DupPipes[1]);
					dup2(DupPipes[0], STDIN_FILENO);
					close(DupPipes[0]);
					execvp(commandarrayB[0], commandarrayB);
				} else if (pid > 0) {

			//			waitpid(pid,NULL,0);
					close(DupPipes[1]);
					close(DupPipes[0]);

					//printf("Process Parent is running flag is %d \n ",RedirectPipe);
					waitpid(pid, NULL, 0);
					//printf("Exit");
				} else {
						// Print for Fork B failed
					printf("DupShell: Fork Error \n");
				}
			}
		} else {
				// result for Fork A failed
			printf("DupShell: Fork Error \n");
		}

	}
}

/*
*	Function: ParseLine
*	Input: command line
*	Output: commandArrayA commanArrayB flag
*	Description: parse the commandline and separate the commandline into two parts:
			CommandArrayA and commandArrayB.
			And also return flag indicate the existence of '|'
*	Author: wzhu4@buffalo.edu
*	Date: 09/28/2010
   */
int parseLine(char * line, char * command_arrayA[], char * command_arrayB[]) {
	char * p;
	int count = 0;
	int flag = 0;

	p = NULL;
	p = strtok(line, " ");

	// Parse the commandline util meet '|' or commandline end
	while (p && strcmp(p, "|") != 0) {
		command_arrayA[count] = p;
//		printf("parseline strcmp %s \n", p);
		count++;
		p = strtok(NULL, " ");

	}
	command_arrayA[count] = NULL;

	// Parse the commandline util meet commandline end
	count = 0;
//	printf("parseline strcmp %s \n", p);
	if ((p != 0) && (strcmp(p, "|") == 0)) {
		flag = 1;
		p = strtok(NULL, " ");
		while (p != 0) {
			command_arrayB[count] = p;
			count++;
			p = strtok(NULL, " ");
		}
	}

	command_arrayB[count]=NULL;
	return flag;
}
