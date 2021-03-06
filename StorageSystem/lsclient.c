#include "jovi.h" 

int main(int argc, char ** argv) {
	int sock;
	int read_count;
	char read_buf[MAXLINE];
	char send_line[MAXLINE];
	int i;

	struct sockaddr_un address;
	char sSocketName[MAXLINE];

	//
	memset(read_buf,0,sizeof(read_buf));
	memset(send_line,0,sizeof(send_line));
	memset(sSocketName,0,sizeof(sSocketName));

	strcat(send_line,"ls ");
	
	for (i=2;i<argc;i++)
		{
		strcat(send_line, argv[i]);
		strcat(send_line, " ");
		}

	// Get paramters: socketname and data to send from command line
	strcpy(sSocketName, argv[1]);

	// Create a socket
	if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		printf("client error: can't create socket \n");
		exit(1);
	}


	// Setup socket address to communicate to
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, sSocketName);


	// Connect to a socket
	if (connect(sock, (struct sockaddr*) &address, sizeof(address)) < 0) {
		printf("client error: can't connect to socket \n");
		exit(1);
	}


	if (write(sock, send_line, strlen(send_line)) < 0) {
		printf("writen() error");
	}

	printf("send_line is %s \n", send_line);
	sleep(2);


	read_count = read(sock, read_buf, sizeof(read_buf));
	printf("read count is %d \n", read_count);
	read_buf[read_count]=0;
	printf("received response: \n%s \n", read_buf);

}

