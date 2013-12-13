#include "jovi.h" 


int main(int argc, char ** argv) {
	int read_count;
	char read_buf[MAXLINE];
	char send_line[MAXLINE];
	char ResponseLineFromDisk[MAXLINE];
	int i;

	struct sockaddr_un address;
	char sSocketName[MAXLINE];
	int track_num, sector_num;
	int track_index,sector_index;
	int loopnum;
	int randomnum;
	char tempstr[MAXLINE];

	//
	printf("initial the parameter\n");
	memset(read_buf,0,sizeof(read_buf));
	memset(send_line,0,sizeof(send_line));
	memset(sSocketName,0,sizeof(sSocketName));
	memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));

	strcat(send_line,"I");
	
	// Get paramters: socketname and data to send from command line
	if( argc !=3 )
	{
		printf("Usage: dclient MYFS LoopNum\n");
		exit(1);
	}

	strcpy(sSocketName, argv[1]);
	loopnum = atoi(argv[2]);
	printf("Loop Number is set to %d\n",loopnum);

	int sock;
	memset(&address,0,sizeof(struct sockaddr_un));
	// Create a socket
	printf("created socket\n");
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


	 if(write(sock,"I",1) < 0 )
	 {
	    printf("write() error\n");
	    exit(1);
	 }

	  memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
	  //sleep(1);
	  if(read(sock, ResponseLineFromDisk,sizeof(ResponseLineFromDisk)) < 0)
	  {
	     printf("read() error\n");
	     exit(1);
	  }
	 
	  printf("Received:\"%s\"",ResponseLineFromDisk);
	  sscanf(ResponseLineFromDisk,"%d %d",&track_num,&sector_num);
	  printf("Disk server has %d tracks / %d sectors per track\n", track_num, sector_num);


	 while(loopnum > 0)
	 {

		printf("loop: %d \n" , loopnum);
		memset(send_line,0,sizeof(send_line));
		memset(tempstr,0,sizeof(tempstr));

		randomnum = rand() % 2;

		if ( randomnum == 0)
		{
				strcpy(send_line, "W ");
		}
		else if (randomnum == 1)
		{
			strcpy(send_line,"R ");
		}
		else
		{
			printf("rand() error\n");
			continue;
		}

		track_index = rand() % track_num;
		sector_index = rand() % sector_num;


		sprintf(tempstr,"%d %d",track_index,sector_index);
		strcat(send_line, tempstr);

		printf("command line is %s\n", send_line);


		if( randomnum == 0 )
		{
			strcat(send_line," 256 abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstu1");
		}


		//printf("command line is %c\n", send_line[0]);

		if (write(sock, send_line, strlen(send_line)) < 0) {
			printf("writen() error");
		}

		sleep(1);

		memset(read_buf,0,sizeof(read_buf));

		read_count = read(sock, read_buf, sizeof(read_buf));
		printf("read result is %c\n", read_buf[0]);
		printf("-----------------------------\n%s\n", &read_buf[1]);
		loopnum--;
	}
}

