/*
   * Program: dserver.c
   * 
   * Requires: jovi.h
   * 
   * Description:
   * 
   * To execute:
   * 
   * Written by: Wei Zhu (Jovi) 
   * Date : 11/16/2010
  */



#include "jovi.h"

/*
 *  Function name: main
 *	Author: wzhu4@buffalo.edu
 *
 *	Date: 11/28/2010
 */

int main( int argc, char * argv[])
{
	pid_t child_pid;
	int i;

	char * command_array[MAXLINE];
	int command_count;
	int errno;
	char read_line[MAXLINE];
	char temp_read_line[MAXLINE];
	int bytes_read;
	int read_line_len;
	char send_line[MAXLINE];
	char sSocketName[MAXLINE];
	int listen_fd;
	int mmap_fd;
	uint8_t * map;
	int track_num, sector_num;
	struct sockaddr_un server_address, client_address;
	int file_size;
	char result_line[257];
	char mmapstorefile[MAXLINE];
	char filepath[MAXLINE];


		
   	int result; 

	if ( argc !=5 )
	{
		printf ( "Usage: dserver unixsocket track_num sector_num mmapfile\n " );
		exit (1);
	}
	// if argc != 1
	track_num=atoi(argv[2]);
	sector_num=atoi(argv[3]);



	file_size=track_num*sector_num*sizeof(uint8_t)*256;

	memset(mmapstorefile,0,sizeof(mmapstorefile));
	memset(filepath,0,sizeof(filepath));

	strcpy(filepath,"./");
	strcpy(mmapstorefile,argv[4]);
	strcat(filepath,mmapstorefile);
	printf("DERVER:MMAP fiel was set to file %s\n", filepath);


    // char *map  /*If you work on string, char* will be needed. */

    /* Open a file for writing.
     *  - Creating the file if it doesn't exist.
     *  - Truncating it to 0 size if it already exists. (not really needed)
     *
     * Note: "O_WRONLY" mode is not sufficient when mmaping.
     */
	
    mmap_fd = open(filepath, O_RDWR | O_CREAT , (mode_t)0600);
    if (mmap_fd == -1) {
	printf("DESERVER:Error opening file for writing");
	exit(EXIT_FAILURE);
    }

    /* Stretch the file size to the size of the (mmapped) array of ints
     */
    result = lseek(mmap_fd, file_size-1, SEEK_SET);
    if (result == -1) 
	{
		close(mmap_fd);
		printf("DSERVER:Error calling lseek() to 'stretch' the file");
		exit(EXIT_FAILURE);
    }
	printf("DSERVER:Mmap creaed\n");
    
    /* Something needs to be written at the end of the file to
     * have the file actually have the new size.
     * Just writing an empty string at the current file position will do.
     *
     * Note:
     *  - The current position in the file is at the end of the stretched 
     *    file due to the call to lseek().
     *  - An empty string is actually a single '\0' character, so a zero-byte
     *    will be written at the last byte of the file.
     */
    result = write(mmap_fd, "", 1);
    if (result != 1) {
	close(mmap_fd);
	printf("DSERVER:Error writing last byte of the file");
	exit(EXIT_FAILURE);
    }

    /* Now the file is ready to be mmapped.
     */
    map = mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, mmap_fd, 0);

    if (map == MAP_FAILED) 
	{
		close(mmap_fd);
		perror("DSERVER:Error mmapping the file");
		exit(EXIT_FAILURE);
    }

	strcpy(sSocketName, argv[1]);

	printf("DSERVER:Listening on UNIX socket ");
	printf("\"%s\" \n\n",sSocketName);


	// Create a unix socket
	if ((listen_fd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		printf("DSERVER:Client error can't create socket");
		exit(1);
	}

	// Setup socket attribute
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, sSocketName);

	// bind server_address to the passive socket
	if (bind(listen_fd, (struct sockaddr *) &server_address, sizeof(server_address))< 0) {
	printf("bind() error \n");
	}

	// start listening for incoming connection requests
	// LISTENQ is set to 5
	if (listen(listen_fd, LISTENQ) < 0) {
		printf("listen() failed");
	}
		
		
	//infinite loop to accept connection

	while (1) {


		socklen_t client_len = sizeof(client_address);
		int conn_fd;

		printf("DSERVER:Awaiting connection...\n");

		if ((conn_fd = accept(listen_fd, (struct sockaddr *) &client_address,
				&client_len)) < 0) {
			if (conn_fd == 0) {
				printf("Errno is Zero \n");
				continue;
			} else if ( conn_fd < 0){
				printf("accept() failed \n");
			}
		}
		
		printf("DSERVER:Acquired client. \n");
		     
		if ((child_pid = fork()) < 0) {
			printf("fork() failed \n");
		}
		
		if (child_pid == 0) { 
			/* child process*/
			while(1)
			{
				memset(read_line,0,sizeof(read_line));
				bytes_read = read(conn_fd, read_line, MAXLINE);
				if (bytes_read < 0){
						printf("read() failed \n");
				}
				if ( bytes_read == 0 ){
						printf("DSERVER:Read Len is zero\n");
					exit(0);
				}
				

				memset(result_line,0,sizeof(result_line));
				memset(temp_read_line,0,sizeof(temp_read_line));
				memset(command_array,0,sizeof(command_array));

				close(listen_fd); /* close the listening socket */

				read_line[bytes_read]=0;
				printf("DSERVER:Received Request: ");
				printf("\"%s\" \n", read_line);

				read_line_len=strlen(read_line);
				strcpy(temp_read_line,read_line);
			
				command_count=parseLine(temp_read_line, command_array);
				command_array[command_count]=NULL;
			
			
				if( (strcmp(command_array[0],"I")==0) && (command_count == 1))
				{
					sprintf(result_line,"%d %d",track_num,sector_num);
					printf("DSERVER:Result line is %s \n",result_line);
					write(conn_fd,result_line,strlen(result_line));
				}
				else if( (strcmp(command_array[0],"R")==0 ) && ( command_count == 3 ) )
				{
					printf("DSERVER:Entred command R \n");
					int track_index =atoi( command_array[1]);
					int sector_index = atoi(command_array[2]);

					printf("DSERVER:Index is %d  %d\n", track_index,sector_index);

					if( (track_index<track_num) && ( sector_index < sector_num))
					{
						result_line[0]='1';

						for (i=0;i<256;i++)
						{
							result_line[i+1]=map[(track_index*sector_num+sector_index)*256+i];
						}

						printf("DSERVER:Result line is %s \n", result_line);
						write(conn_fd,result_line,257);
					}
					else
					{
						result_line[0]='0';
						write(conn_fd,result_line,1);
					}
				}
				else if ( (strcmp(command_array[0],"W" ) == 0 ) )
				{
					printf("DSERVER:Enter command W\n");
					int track_index = atoi(command_array[1]);
					int sector_index = atoi(command_array[2]);
					int read_length = atoi(command_array[3]);

					if( (track_index < track_num) && ( sector_index<sector_num))
					{
						result_line[0]='1';
						int offset = strlen(command_array[1])+strlen(command_array[2])+strlen(command_array[3])+5;

						for (i=0;i<256 &&( i< read_length);i++)
						{
							printf("%c ",read_line[i+offset]);
							map[(track_index*sector_num+sector_index)*256+i]=(uint8_t)read_line[i+offset];
						}

						printf("\nDSERVER:write is OK\n");
					}
					else
					{
						result_line[0]='0';
					}

					write(conn_fd,result_line,1);
					
				}
				/*
				else if ( ( strcmp(command_array[0],"W") == 0 ) && ( command_count == 3))
				{
					// Format the block with zero
					int track_index = atoi(command_array[1]);
					int sector_index = atoi ( command_array[2]);

					if (( track_index < track_num) && ( sector_index < sector_num))
					{
					result_line[0]='1';

					for(i=0;i<256;i++)
					{
						map[track_index*sector_num+sector_index*256+i]=0;
					}
					printf("write is ok\n");
					}
					else
					{
						result_line[0]='0';
					}
					write(conn_fd,result_line,1);
						
				}*/
				else
				{
					printf("DSERVER:Unsupport Command \n");
					result_line[0]='0';
					write(conn_fd, result_line,1);
				}

			}
			// child process while(1);
			close(conn_fd);
		}
		// if child_pid = 0
		else if( child_pid > 0)
		{
			waitpid(child_pid,NULL);
			close (conn_fd);
			printf("DSERVER:connection closed \n");
		}
		else
		{
			printf("fork() Error \n ");
		}

	}// main while(1)

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



int MMapFree(int * map, int fd, int file_size)
{

    /* Don't forget to free the mmapped memory
     */
    if (munmap(map, file_size) == -1) {
	perror("Error un-mmapping the file");
	/* Decide here whether to close(fd) and exit() or not. Depends... */
    }

    /* Un-mmaping doesn't close the file, so we still need to do that.
     */
    close(fd);

}

