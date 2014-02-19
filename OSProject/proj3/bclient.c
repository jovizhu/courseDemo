#include "jovi.h" 

int main(int argc, char ** argv) {
	int sock;
	int read_count;
	char read_buf[MAXLINE];
	char send_line[MAXLINE];

	struct sockaddr_un address;
	char sSocketName[80];

	//
	memset(read_buf,0,sizeof(read_buf));
	memset(send_line,0,sizeof(send_line));
	memset(sSocketName,0,sizeof(sSocketName));

	// Get paramters: socketname and data to send from command line
	strcpy(sSocketName, argv[1]);
	strcpy(send_line, argv[2]);

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
	sleep(2);

	read_count = read(sock, read_buf, sizeof(read_buf));
	read_buf[read_count]=0;
	printf("received response: %s \n", read_buf);

}

