#include "jovi.h"



int main(int argc, char** argv) {


	int listen_fd, conn_fd;
	pid_t child_pid;
	struct sockaddr_un client_address, server_address;
	char read_line[MAXLINE];
	char sSocketName[80];
	int errno;
	int client_len;
	int bytes_read;
	char temp;
	int i;
	int read_line_len;

	if(argc < 2)
	{
		printf("Usage: bserver SocketName \n");
		exit(0);
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
	if (bind(listen_fd, (struct sockaddr *) &server_address, sizeof(server_address))
			< 0) {
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
		printf("Awaiting connection...\n");
		if ((conn_fd = accept(listen_fd, (struct sockaddr *) &client_address,
				&client_len)) < 0) {
			if (errno == 0) {
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

			for (i=0;i<read_line_len/2;i++)
			{
				temp=read_line[i];
				read_line[i]=read_line[read_line_len-i-1];
				read_line[read_line_len-i-1]=temp;
			}

			printf("Sending request: \"%s\" \n", read_line);
			if ((write(conn_fd, read_line, bytes_read)) < 0) {
				printf("writen() failed");
			}
			exit(0);
		}

		close(conn_fd);
		printf("Connection closed");
	}

	exit(0);
}
