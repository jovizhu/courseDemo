#include "jovi.h" 

int main(int argc, char ** argv) {
	int read_count;
	char read_buf[MAXLINE];
	char send_line[MAXLINE];
	int i;

	char result_line[MAXLINE];

	struct sockaddr_un address;
	char sSocketName[MAXLINE];

	//
	memset(read_buf,0,sizeof(read_buf));
	memset(send_line,0,sizeof(send_line));
	memset(sSocketName,0,sizeof(sSocketName));
	memset(result_line,0,sizeof(result_line));

	// Get paramters: socketname and data to send from command line
	strcpy(sSocketName, "MYFS");

	int sock;
	memset(&address,0,sizeof(struct sockaddr_un));
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

	memset(send_line,0,sizeof(send_line));
	strcpy(send_line,"R ");
	strcat(send_line,argv[1]);

	//fgets(send_line, sizeof(send_line),stdin);
	//send_line[strlen(send_line)]=0;

	if (write(sock, send_line, strlen(send_line)) < 0) {
		printf("writen() error");
	}

	printf("Command %s executed\n", send_line);
	sleep(1);

	memset(read_buf,0,sizeof(read_buf));

	read_count = read(sock, read_buf, sizeof(read_buf));
	if(read_buf[0] == '0')
	{
		sscanf(read_buf+1,"%d %s",&i, result_line);
		printf("%s\n",result_line);
	}
	else if (read_buf[0] == '1')
	{
		printf("File not exist \n");
	}
	else
	{
		printf("Unknown error \n");
	}
}

