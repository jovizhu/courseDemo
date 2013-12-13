/*
   * Program: lsserver.c
   * 
   * Requires: jovi.h
   * 
   * Description:
   * 
   *  
   * To execute:
   * 
   * 
   * Written by: Wei Zhu (Jovi) 
   * Date : 11/16/2010
  */



#include "jovi.h"


/*
 *  Function name: main
 *	Author: wzhu@buffalo.edu
 *
 *	Date: 09/28/2010
 */
int main( int argc, char * argv[])
{
		int ls_pipes[2];
		pid_t child_pid;
	
		char * command_array[MAXLINE];
		int command_count;
		int errno;
		char read_line[MAXLINE];
		int bytes_read;
		int read_line_len;
		char send_line[MAXLINE];
		char sSocketName[MAXLINE];
		int listen_fd, conn_fd;
		struct sockaddr_un server_address, client_address;

		if ( argc !=2 )
		{
				printf ( "Usage: lsserver unixsocket \n " );
		}

		strcpy(sSocketName, argv[1]);

		printf("Basic server listening on UNIX socker \n");
		printf("\"%s\" \n\n",sSocketName);

		// Create a unix socket
		if ((listen_fd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
			printf("client error: can't create socket");
			exit(1);
			}

		// Setup socket attribute
		server_address.sun_family = AF_UNIX;
		strcpy(server_address.sun_path, sSocketName);

		// bind server_address to the passive socket
		if (bind(listen_fd, (struct sockaddr *) &server_address, sizeof(server_address))< 0) {
		printf("bind()");
		}

		// start listening for incoming connection requests
		// LISTENQ is set to 5
		if (listen(listen_fd, LISTENQ) < 0) {
		printf("listen() failed");
		}
		
		
	//infinite loop to accept connection
	while (1) {

		socklen_t client_len = sizeof(client_address);

		if(pipe(ls_pipes)<0)
		{
				printf(" pipe()error");
		}

		printf("Awaiting connection...\n");

		if ((conn_fd = accept(listen_fd, (struct sockaddr *) &client_address,
				&client_len)) < 0) {
			if (errno == 0) {
					printf("Errno is Zero \n");
					sleep(3);
				continue;
			} else {
				printf("accept() failed");
			}
		}
		
		printf("Acquired client. \n");

		if ((child_pid = fork()) < 0) {
			printf("fork() failed");
		}
		
		if (child_pid == 0) { /* child process              */

			close(listen_fd); /* close the listening socket */
			memset(read_line,0,sizeof(read_line));
			memset(command_array,0,sizeof(command_array));

			bytes_read = read(conn_fd, read_line, MAXLINE);

			if (bytes_read < 0)
				printf("read() failed");
			if (bytes_read == 0)
				return 0;

			read_line[bytes_read]=0;
			printf("Received Request: ");
			printf("\"%s\" \n", read_line);

			//converse the received string
			read_line_len=strlen(read_line);
			
			// close the pipes and dup to stdout to pipes1
			close ( ls_pipes[0]);

			dup2(ls_pipes[1], STDOUT_FILENO);

			close ( ls_pipes[1]);
			close (conn_fd);
			

			command_count=parseLine(read_line, command_array);
			command_array[command_count]=NULL;
			//printf("%s \n", command_array[0]);
			//printf("%s \n", command_array[1]);

			execvp(command_array[0], command_array );
			exit(0);
			
			}
		else if ( child_pid > 0 )
			{

				wait(5);
				printf("sending response \n");
				read ( ls_pipes[0], send_line, sizeof(send_line)) ;
				write (conn_fd,send_line,sizeof(send_line));
				printf("send_line is %s \n", send_line);
				memset(send_line,0,sizeof(send_line));
				close (conn_fd);
				printf("connection closed \n");
			}
		else
			{
						printf("Error \n ");
			}
		memset(ls_pipes,0,sizeof(ls_pipes));

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
			//	printf("split command is %s \n", command_array[count]);
				count++;
				p = strtok(NULL," ");
		}
		// return the command count
		return count;
}

