#include "ChattyMe.h"
#include "network_util.h"
#include "color.h"

fd_set read_set, RST;
int ConnectFdList[MY_FD_SETSIZE] = { -1, -1, -1, -1, -1 };
struct ConnectObject ConnectObjectList[MY_FD_SETSIZE];
void Info_Print(uint16_t, uint16_t);
int lookup_host(const char *host, char * ippointer, char * hostpointer);
int DoConnect(char * destination, char * tcpport);

int main(int argc, char * argv[]) {

	uint16_t local_udp_port = LOCAL_UDP_PORT;
	uint16_t local_tcp_port = LOCAL_TCP_PORT;
	int loopnum = 0; // used for loop;

	textcolor(BRIGHT, YELLOW, BLACK);
	printf("//    Program: ChattyMe (version release 0.3)\n"
	
		"//\n"
		"//    Welcome to use ChattyMe!\n"
		"//    This is a project \n"
		"//\n"
		"//    If you found any bug, please contact:\n");
	textnormal();
	printf("\n\n");

	/*
	 * This is for the argc verify.
	 */
	if (argc == 1) {
		textcolor(BRIGHT, RED, BLACK);
		printf("Using Default TCP-Port %d, and UDP-Port %d ", local_tcp_port,
				local_udp_port);
		textnormal();
		printf("\nChattyMe > ");
		fflush(stdout);
	} // end if of arg==1
	else if (argc == 2) {
		local_tcp_port = atoi(argv[1]);
		textcolor(BRIGHT, RED, BLACK);
		printf("Using  TCP-Port %d, and  Default UDP-Port %d ", local_tcp_port,
				local_udp_port);
		textnormal();
		printf("\nChattyMe > ");
		fflush(stdout);
	} // end if arg == 2
	else if (argc == 3) {
		local_tcp_port = atoi(argv[1]);
		local_udp_port = atoi(argv[2]);
		textcolor(BRIGHT, RED, BLACK);
		printf("Using  TCP-Port %d, and UDP-Port %d ", local_tcp_port,
				local_udp_port);
		textnormal();
		printf("\nChattyMe > ");
		fflush(stdout);
	} // end if arg ==3
	else {
		textcolor(BRIGHT, RED, BLACK);
		printf("Usage: ChattyMe [<TCP-Port> <UDP-Port>] ");
		textnormal();
		printf("\n");
		exit(1);
	} // end if else


	//******************************************************************************
	// in the following, will be udp server and tcp server;
	int listen_fd;
	const int on = 1;
	struct sockaddr_in server_address;
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket() error\n");
	}

	// fill out the server address structure
	memset((void *) &server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(local_tcp_port);

	/*
	 * This is to prevent the case where the parent server was killed and
	 * restarted later while the children are still handling requests using the
	 * same tcp port number. In this situation, 'setsockopt' allows the server
	 * to bind the passive socket to the port
	 */

	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		printf("setsockopt() failed\n");
	}

	// bind server_address to the socket
	if (bind(listen_fd, (struct sockaddr *) &server_address,
			sizeof(server_address)) < 0) {
		printf("bind() failed\n");
	}

	// actually turn the socket into a passive socket
	if (listen(listen_fd, LISTENQ) < 0) {
		printf("listen() failed\n");
	}

	//***************************************************************************
	// IN the following , it is the udp turn
	int udp_fd;
	if ((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket() failed\n");
	}

	memset((void *) &server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(local_udp_port);

	if (bind(udp_fd, (struct sockaddr *) &server_address,
			sizeof(server_address)) < 0) {
		printf("bind() failed\n");
	}

	//********************************************************************8
	// no zombies around, please
	if ((simpler_sigaction(SIGCHLD, sig_child_handler)) == SIG_ERR) {
		printf("simpler_sigaction() failed\n");
	}

	// setting things up for select
	int maxfd;
	int num_ready_fds;
	FD_ZERO(&read_set);
	maxfd = max(listen_fd, udp_fd);
	FD_SET(listen_fd, &read_set);
	FD_SET(udp_fd, &read_set);
	maxfd = max(STDIN_FILENO,maxfd);
	FD_SET(STDIN_FILENO,&read_set);

	// our favourite infinite loop


	// for TCP Listening
	struct sockaddr_in client_address;
	int client_SA_len;
	char hostname[MAXLINE];
	int tcp_fd;
	uint16_t localport, remotport;
	// End TCP Listening

	// for New Tcp Listening
	char tcpmessagecontent[MAXLINE];
	struct TCPMessageFormat TCPMessage;
	int n = 0;
	char length[4];

	// for New TCP Listening

	// for UDP Listening
	int upd_fd;
	struct UDPmessageFormat * upd_message;
	char udpmessagecontent[MAXLINE];
	int pid;
	char hostname_udp[MAXLINE];
	int port_udp;

	//for UDP Listening

	// for STDIN
	char commandline[MAXLINE];
	char * commandArray[MAXLINE];
	int flag = 0;
	int commandcount;
	int sock_fd;
	// for STDIN


	while (1) {
		RST = read_set;
		// set the num_ready_fds
		if ((num_ready_fds = select(maxfd + 1, &RST, NULL, NULL, NULL)) < 0) {
			/*
			 * there are two cases where select fails: an actual error has occured
			 * or we're interrupted by SIGCH       res.  If hints is not NULL it points to an addrinfo structure whose ai_family,LD. If the later happens, we just go back
			 * to selecting
			 */
			if (errno == EINTR) {
				continue;
			} else {
				printf("select() failed\n");
			}
		}

		//**********************************************************************
		// TCP SERVER LISTENING
		// if listen_fd has something to say
		if (FD_ISSET(listen_fd, &RST)) {
			client_SA_len = sizeof(client_address);
			if ((tcp_fd = accept(listen_fd,
					(struct sockaddr *) &client_address, &client_SA_len)) < 0) {
				if (errno == EINTR) {
					continue; /* restart accepting new connections */
				} else { /* otherwise, some error has occured */
					printf("accept() failed\n");
				}
			}

			for (loopnum = 0; loopnum < MY_FD_SETSIZE; loopnum++) {
				if (ConnectFdList[loopnum] < 0) {
					ConnectFdList[loopnum] = tcp_fd; /* save descriptor */
					char * ipaddress = inet_ntoa(client_address.sin_addr);
					remotport = ntohs(client_address.sin_port);
					localport = local_tcp_port;
					getnameinfo((struct sockaddr *) &client_address,
							sizeof(client_address), hostname, sizeof(hostname),
							0, 0, 0);
					AddtoConnectObjectList(loopnum, ipaddress, hostname,
							localport, remotport);

					textcolor(BRIGHT, MAGENTA, BLACK);
					printf("\n// Connection to (%s:%d) established",
							ConnectObjectList[loopnum].hostname,
							ConnectObjectList[loopnum].remotport);
					textnormal();
					printf("\nChatty Me > ");
					fflush(stdout);
					/*
					 printf("ipaddress %s \n",
					 ConnectObjectList[loopnum].ipaddress);
					 printf("hostname %s \n",
					 ConnectObjectList[loopnum].hostname);
					 printf("remotport %ld \n",
					 ConnectObjectList[loopnum].remotport);
					 printf("localport %ld \n",
					 ConnectObjectList[loopnum].localport);
					 */

					break;
				}
			}
			if (loopnum == MY_FD_SETSIZE) {
				textcolor(BRIGHT, RED, BLACK);
				printf("\n// Maximum TCP connections have been reached! ");
				textnormal();
				printf("\nChattyMe > ");
				fflush(stdout);
				continue;
			}

			FD_SET(tcp_fd, &read_set); /* add new descriptor to set */
			if (tcp_fd >= maxfd)
				maxfd = tcp_fd; /* for select */

			if (--num_ready_fds <= 0)
				continue;
		} /* if listen_fd is ready */

		//**********************************************************************
		// New TCP SERVER LISTENING
		for (loopnum = 0; loopnum < MY_FD_SETSIZE; loopnum++) { /* check all clients for data */
			if ((tcp_fd = ConnectFdList[loopnum]) >= 0) {
				if (FD_ISSET(tcp_fd , &RST)) {
					memset(tcpmessagecontent, 0, MAXLINE);
					sleep(2);
					if ((n = read(tcp_fd, tcpmessagecontent, MAXLINE)) == 0) {
						/* connection closed by client */
						close(tcp_fd);
						FD_CLR(tcp_fd, &read_set);
						ConnectFdList[loopnum] = -1;

						textcolor(BRIGHT, MAGENTA, BLACK);
						printf("\n// connection # %d was closed by peer",
								loopnum);
						textnormal();
						printf("\nChattyMe > ");
						fflush(stdout);

					} else if (n < 0) {
						printf("system error \n");
					} else {
						uint32_t tcp_rev_pid;
						memcpy(&tcp_rev_pid, &tcpmessagecontent[4], 4);
						tcp_rev_pid = ntohl(tcp_rev_pid);

						textcolor(BRIGHT, MAGENTA, BLACK);
						printf("\n// Received mesg:      \"%s\"\n",
								&tcpmessagecontent[8]);
						printf("// Protocol:           TCP\n");
						printf("// Connection number:  %d\n", loopnum);
						printf("// Sender's PID:       %d", tcp_rev_pid);
						textnormal();
						printf("\nChattyMe > ");

						fflush(stdout);
					}
					if (--num_ready_fds <= 0)
						break; /* no more readable descriptors */
				}
			}
		}
		// end New tCP SERVER LISTENING

		// Listen to UDP
		// if udp_fd is ready
		if (FD_ISSET(udp_fd, &RST)) {
			sleep(1);
			client_SA_len = sizeof(client_address);

			n = recvfrom(udp_fd, udpmessagecontent, MAXLINE, 0,
					(struct sockaddr *) &client_address, &client_SA_len);
			if (n < 0) {
				printf("recvfrom failed\n");
			}

			char * ipaddress_udp = inet_ntoa(client_address.sin_addr);
			port_udp = ntohs(client_address.sin_port);
			getnameinfo((struct sockaddr *) &client_address,
					sizeof(client_address), hostname_udp, sizeof(hostname_udp),
					0, 0, 0);

			//upd_message = (struct UDPmessageFormat *) udpmessagecontent;

			//pid = fork();
			//if (pid == 0) {
			uint32_t udppid;
			memcpy(&udppid, &udpmessagecontent[0], 4);
			udppid = ntohl(udppid);

			textcolor(BRIGHT, MAGENTA, BLACK);
			printf("\n// Received mesg:     \"%s\"\n", &udpmessagecontent[4]);
			printf("// Protocal:          UDP\n");
			printf("// Sender's IP/name:  (%s,%s)\n", ipaddress_udp,
					hostname_udp);
			printf("// Sender's port:     %d\n", port_udp);
			printf("// Sender's PID:      %d", udppid);
			textnormal();
			printf("\nChattyMe > ");
			fflush(stdout);

			//exit(0);
			//}

			continue;
		} /*if udp_fd is ready */

		// Listen to STDIN
		if (FD_ISSET(STDIN_FILENO, &RST)) {
			fgets(commandline, sizeof(commandline), stdin);

			// Remove the \n from the commandline
			flag = 0;
			for (loopnum = 0; ((loopnum < sizeof(commandline)) && (flag == 0)); loopnum++) {
				if (commandline[loopnum] == '\n') {
					flag = 1;
					//printf("I = %d \n", i);
				}

			}
			commandline[loopnum - 1] = 0;

			if (loopnum == 1) {
				printf("ChattyMe > ");
				fflush(stdout);
				continue;
			}

			commandcount = Parse_Input(commandline, commandArray);

			if (strcmp(commandline, "quit") == 0 || strcmp(commandline, "exit")
					== 0) {
				textcolor(BRIGHT, YELLOW, BLACK);
				printf("Thanks for using ChattyMe!");
				textnormal();
				printf("\n");
				exit(0);}


			else if (strcmp(commandline, "show") == 0) {
				ShowFunction();
			}

			else if ((commandcount == 1) && (strcmp(commandArray[0], "info")
					== 0)) {
				Info_Print(local_tcp_port, local_udp_port);
				continue;
			}

			else if ((commandcount == 3) && (strcmp(commandArray[0], "connect")
					== 0)) {
				int socknum = DoConnect(commandArray[1], commandArray[2]);

				if (socknum < 0) {
					textcolor(BRIGHT, YELLOW, BLACK);
					printf("\n Name or service not known!");
					textnormal();
					printf("\nChattyMe > ");
					fflush(stdout);
					continue;
				}

				sock_fd = ConnectFdList[socknum];
				maxfd = max(sock_fd, maxfd) + 1;
				FD_SET(sock_fd, &read_set);
				continue;
			}

			else if ((commandcount == 2) && (strcmp(commandArray[0],
					"disconnect") == 0)) {
				int closefdnum = atoi(commandArray[1]);

				if (RemoveFromConnectObjectList(closefdnum) < 0) {
					textcolor(BRIGHT, YELLOW, BLACK);
					printf("No such active connection!");
					textnormal();
					printf("\nChattyMe > ");
					fflush(stdout);

					continue;
				}

				textcolor(BRIGHT, MAGENTA, BLACK);
				printf("// Connection number %d is closed", closefdnum);
				textnormal();
				printf("\nChattyMe > ");
				fflush(stdout);

				continue;
			}

			//This is the implementation of sending out TCP messages.
			else if ((strcmp(commandArray[0], "send") == 0)) {
				int sendfdnum = atoi(commandArray[1]);

				if (sendfdnum < 0 || sendfdnum >= MY_FD_SETSIZE) {
					textcolor(BRIGHT, YELLOW, BLACK);
					printf("Input ERROR");
					textnormal();
					printf("\nChattyMe > ");
					fflush(stdout);
					continue;
				}

				if (ConnectFdList[sendfdnum] < 0) {
					textcolor(BRIGHT, YELLOW, BLACK);
					printf("No such TCP connection!");
					textnormal();
					printf("\nChattyMe > ");
					fflush(stdout);
					continue;
				}

				sock_fd = ConnectFdList[sendfdnum];

				char message[MAXLINE];
				memset(message, 0, sizeof(message));
				char * temp;

				uint32_t tcpsendpid = getpid();
				tcpsendpid = htonl(tcpsendpid);

				temp = &message[8];
				for (loopnum = 2; loopnum < MAXLINE && commandArray[loopnum]
						!= NULL; loopnum++) {
					strcat(temp, commandArray[loopnum]);
					strcat(temp, " ");
				}
				temp[strlen(temp) - 1] = '\0';

				uint32_t payload = strlen(temp) + 4;
				uint32_t payloadbig = htonl(payload);

				memcpy(&message[0], &payloadbig, 4);
				memcpy(&message[4], &tcpsendpid, 4);
				memcpy(&message[8], temp, strlen(temp));

				if (write(sock_fd, message, payload + 4) < 0) {
					printf("writen() error\n");
				} else {
					textcolor(BRIGHT, MAGENTA, BLACK);
					printf("// TCP message: \"%s\" \n", temp);
					printf("// Send to Conn#: %d", sendfdnum);
					textnormal();
					printf("\nChattyMe > ");
					fflush(stdout);
				}

				continue;
			}

			//This is the implementation of sending out UDP messages.
			else if (strcmp(commandArray[0], "sendto") == 0) {

				if (commandcount < 4) {
					textcolor(BRIGHT, YELLOW, BLACK);
					printf("SYNTAX:\n");
					printf("         sendto <ip-address> <udp-port> <message>");
					textnormal();
					printf("\nChattyMe > ");
					fflush(stdout);
					continue;
				}

				char udpsendmessage[MAXLINE];
				memset(udpsendmessage, 0, sizeof(udpsendmessage));
				char upd_send_server_ip[MAXLINE];
				char *udp_temp = &udpsendmessage[4];
				struct sockaddr_in udp_send_server_address;
				int udp_send_sock_fd = 0, udp_send_pton_ret;
				char udp_send_ipaddress[MAXLINE];
				char upd_send_hostname[MAXLINE];
				char hostname_upd[MAXLINE];

				uint16_t udp_send_server_port;
				uint32_t udp_send_pid = getpid();
				udp_send_pid = htonl(udp_send_pid);

				for (loopnum = 3; loopnum < MAXLINE && commandArray[loopnum]
						!= NULL; loopnum++) {
					strcat(udp_temp, commandArray[loopnum]);
					strcat(udp_temp, " ");
				}

				udp_temp[strlen(udp_temp) - 1] = '\0';

				udp_send_server_port = atoi(commandArray[2]);

				strncpy(upd_send_server_ip, commandArray[1], strlen(
						commandArray[1]));

				memcpy(&udpsendmessage[0], &udp_send_pid, 4);
				memcpy(&udpsendmessage[4], udp_temp, strlen(udp_temp));
				memset((void *) &udp_send_server_address, 0,
						sizeof(udp_send_server_address));

				udp_send_server_address.sin_family = AF_INET;
				udp_send_server_address.sin_port = htons(udp_send_server_port);
				udp_send_pton_ret = inet_pton(AF_INET, upd_send_server_ip,
						&udp_send_server_address.sin_addr);
				if (udp_send_pton_ret <= 0) {
					lookup_host(upd_send_server_ip, udp_send_ipaddress,
							upd_send_hostname);
					strcpy(upd_send_server_ip, udp_send_ipaddress);
					upd_send_server_ip[MAXLINE] = '\0';

					memset((void *) &udp_send_server_address, 0,
							sizeof(udp_send_server_address));
					udp_send_server_address.sin_family = AF_INET;
					udp_send_server_address.sin_port = htons(
							udp_send_server_port);
					udp_send_pton_ret = inet_pton(AF_INET, upd_send_server_ip,
							&udp_send_server_address.sin_addr);
					//printf("%d \n", udp_send_server_port);
					if (udp_send_pton_ret <= 0) {
						printf("inet_pton() error\n");
					}
				}

				// create a UDP socket
				if ((udp_send_sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {

					printf("socket() error \n");
				}

				if (sendto(udp_send_sock_fd, udpsendmessage,
						sizeof(udpsendmessage), 0,
						(struct sockaddr *) &udp_send_server_address,
						sizeof(udp_send_server_address)) < 0) {
					printf("sendto() failed");
				}

				getnameinfo((struct sockaddr *) &udp_send_server_address,
						sizeof(udp_send_server_address), hostname_upd,
						sizeof(hostname_upd), 0, 0, 0);

				textcolor(BRIGHT, MAGENTA, BLACK);
				printf("// UDP message : \"%s\"\n", &udpsendmessage[4]);
				printf("// sent to     : (%s, %s)\n", hostname_upd,
						upd_send_server_ip);
				printf("// port        : %d", udp_send_server_port);
				textnormal();
				printf("\nChattyMe > ");
				fflush(stdout);

			} else {
				textcolor(BRIGHT, YELLOW, BLACK);
				printf("Unknown commands!\n");
				printf("Please read README carefully!");
				textnormal();
				printf("\nChattyMe > ");
				fflush(stdout);
			}
		}
	}
}

int AddtoConnectObjectList(int loopnum, char * ipaddress, char * hostname,
		int localport, int remotprot) {
	ConnectObjectList[loopnum].localport = localport;
	ConnectObjectList[loopnum].remotport = remotprot;
	strcpy(ConnectObjectList[loopnum].hostname, hostname);
	ConnectObjectList[loopnum].hostname[MAXLINE] = '\0';
	strcpy(ConnectObjectList[loopnum].ipaddress, ipaddress);
	ConnectObjectList[loopnum].ipaddress[MAXLINE] = '\0';

	return 0;
}

int ShowFunction() {
	int i = 0;
	int flag = 0;

	for (i = 0; i < MY_FD_SETSIZE; i++) {
		if (ConnectFdList[i] > 0)
			flag = 1;
	}

	if (flag == 0) {
		textcolor(BRIGHT, CYAN, BLACK);
		printf("No active TCP connections to show");
		textnormal();
		printf("\nChattyMe > ");
		fflush(stdout);
		return 0;
	}

	textcolor(BRIGHT, CYAN, BLACK);
	printf(
			" cID |   IP address  |          Host name           | Local | Remote \n");
	printf(
			"---------------------------------------------------------------------");
	textnormal();
	printf("\n");

	for (i = 0; i < MY_FD_SETSIZE; i++) {
		if (ConnectFdList[i] > 0) {
			textcolor(BRIGHT, CYAN, BLACK);
			printf("  %d  ", i); // print Connection id
			printf("|");
			printf("%15s", ConnectObjectList[i].ipaddress); // print ip address
			printf("|");
			printf("%30s", ConnectObjectList[i].hostname); // hostname
			printf("|");
			printf("%7d", ConnectObjectList[i].localport); //local port
			printf("|");
			printf("%8d", ConnectObjectList[i].remotport); // remote port
			textnormal();
			printf("\n"); // end of line;
		}
	}

	printf("ChattyMe > ");
	fflush(stdout);

	return 0;
}

/*
 ****************************************************************************************
 Function Name: Parse_Input
 Source: copied from Dr.Hung Ngo's Project Example.
 ***************************************************************************************\
*/
int Parse_Input(char* line, char* command_array[]) {

	char * p;
	char * temp;
	int count = 0;
	p = strtok(line, " ");

	while (p != 0) {
		command_array[count] = p;
		count++;
		p = strtok(NULL, " ");
	}

	command_array[count] = NULL;
	return count;
}

/*
 ****************************************************************************************
 Function Name: Info_Print
 Source:  Print out the IP address, hostname, TCP port, UDP port and process ID.
 Code copied from Dr Hung Ngo's Example and make some modification
 Date Created: 09/26/2010
 ***************************************************************************************8\
*/

void Info_Print(uint16_t tcp_port, uint16_t udp_port) {
	//printf("Info-Print Enter: \n");
	struct sockaddr_in server_address;
	struct sockaddr_in local_address;
	int sock_fd = 0, pton_ret, sock_name_no, host_name_no;
	char server_ip[MAXLINE] = DEFAULT_DNS_IP;
	char local_name[MAXLINE] = DEFAULT_LOCAL_NAME;
	uint16_t server_port = DEFAULT_DNS_PORT;
	socklen_t len;

	//printf("Define End: \n");

	// get port & address from input, if any
	//parse_input(argc, argv, &server_port, server_ip);
	//note("Server (IP, port) = (%s, %d)", server_ip, server_port);

	// fill up server address structure
	//memset((void *) &local_address, 0, sizeof(local_address));
	memset((void *) &server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	pton_ret = inet_pton(AF_INET, server_ip, &server_address.sin_addr);

	//printf(" Init End \n");

	if (pton_ret <= 0)
		printf("inet_pton() failed");

	// create a UDP socket
	//printf("Create UDP socket: \n");
	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("socket() failed");
	}

	if (connect(sock_fd, (struct sockaddr *) &server_address,
			sizeof(server_address)) == -1) {
		printf("connect() failed");
		exit(0);
	}

	//printf("get SockName : \n");
	len = sizeof(local_address);
	sock_name_no = getsockname(sock_fd, (struct sockaddr *) &local_address,
			&len);

	if (sock_name_no < 0) {
		printf("getsockname() error");
	}

	textcolor(BRIGHT, CYAN, BLACK);
	printf(
			"   IP address  |          Host name           | udp port | tcp port | PID \n");
	printf(
			"--------------------------------------------------------------------------");
	textnormal();
	printf("\n");

	textcolor(BRIGHT, CYAN, BLACK);
	// print local address
	printf("%15s", inet_ntoa(local_address.sin_addr));
	printf("|");
	// print local hostname
	host_name_no = gethostname(local_name, MAXLINE);
	if (host_name_no < 0)
		printf("gethostname() error");
	printf("%30s", local_name);
	printf("|");
	printf("%10d", udp_port);
	printf("|");
	printf("%10d", tcp_port);
	printf("|");
	printf("%5d", getpid());
	textnormal();
	printf("\nChattyMe > ");

	fflush(stdout);

}

int DoConnect(char * destination, char * tcpport) {

	int sock_fd, pton_ret;
	int loopnum;
	struct sockaddr_in server_address;
	struct sockaddr_in local_addr;
	uint16_t server_port = atoi(tcpport);
	char server_ip[MAXLINE];
	char ipaddress[MAXLINE];
	char hostname[MAXLINE];
	char servername[MAXLINE];
	strcpy(server_ip, destination);
	struct addrinfo hints, *res0;
	struct hostent *hptr;
	int remotport, localport;

	server_ip[MAXLINE] = '\0';
	// get port & address from input, if any
	//parse_input(argc, argv, &server_port, server_ip);
	//note("Server (IP, port) = (%s, %d)", server_ip, server_port);

	// create the 'active' socket to send requests to
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("socket() error");

	// fill out the server_address structure.
	memset((void *) &server_address, 0, sizeof(server_address));
	memset((void *) &hints, 0, sizeof(hints));

	memset((void *) &local_addr, 0, sizeof(local_addr));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);

	pton_ret = inet_pton(AF_INET, server_ip, &server_address.sin_addr);

	if (pton_ret <= 0) {
		if (lookup_host(destination, ipaddress, hostname) < 0) {
			printf("lookup_host() error");
			return -1;
		}
		lookup_host(destination, ipaddress, hostname);
		strcpy(server_ip, ipaddress);
		server_ip[MAXLINE] = '\0';

		memset((void *) &server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(server_port);
		pton_ret = inet_pton(AF_INET, server_ip, &server_address.sin_addr);
		//printf("%d \n", server_port);
		if (pton_ret <= 0) {
			printf("inet_pton() error");
			return -1;
		}
	} else {
		// gethostbyaddr
		// empty
	}

	// tries to establish a tcp connection to the server
	if (connect(sock_fd, (struct sockaddr *) &server_address,
			sizeof(server_address)) < 0) {
		printf("connect() error");
		return -1;
	}
	int length = sizeof(local_addr);
	getsockname(sock_fd, (struct sockaddr *) &local_addr, &length);
	//printf("local socket number is %d \n", ntohs(local_addr.sin_port));
	localport = ntohs(local_addr.sin_port);

	length = sizeof(server_address);
	getnameinfo((struct sockaddr *) &server_address, sizeof(server_address),
			hostname, sizeof(hostname), servername, sizeof(servername), 0);
	server_port = ntohs(server_address.sin_port);

	textcolor(BRIGHT, MAGENTA, BLACK);
	printf("// Connection to (%s:%s) established", hostname, servername);
	textnormal();
	printf("\nChattyMe > ");
	fflush(stdout);

	for (loopnum = 0; loopnum < MY_FD_SETSIZE; loopnum++) {
		if (ConnectFdList[loopnum] < 0) {
			ConnectFdList[loopnum] = sock_fd; /* save descriptor */
			char * ipaddress = inet_ntoa(server_address.sin_addr);
			remotport = ntohs(server_address.sin_port);
			//localport = 0;
			AddtoConnectObjectList(loopnum, ipaddress, hostname, localport,
					remotport);
			//					printf("ipaddress %s \n",ConnectObjectList[loopnum].ipaddress);
			//					printf("hostname %s \n",ConnectObjectList[loopnum].hostname);
			//					printf("remotport %ld \n",ConnectObjectList[loopnum].remotport);
			//					printf("localport %ld \n",ConnectObjectList[loopnum].localport);

			break;
		}
	}
	if (loopnum == MY_FD_SETSIZE) {
		textcolor(BRIGHT, RED, BLACK);
		printf("\n// Maximum TCP connections have been reached \n");
		textnormal();
		printf("\nChattyMe > ");
		fflush(stdout);
		return 0;
	}

	//FD_SET(sock_fd, &read_set); /* add new descriptor to set */

	return loopnum;
	//exit(0);
}

int lookup_host(const char *host, char * ippointer, char * hostpointer) {
	struct addrinfo hints, *res;
	int errcode;
	char addrstr[100];
	void *ptr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;
	errcode = getaddrinfo(host, NULL, &hints, &res);
	if (errcode != 0) {
		perror("getaddrinfo");
		return -1;
	}

	//printf("Host: %s\n", host);
	while (res) {
		inet_ntop(res->ai_family, res->ai_addr->sa_data, addrstr, 100);

		switch (res->ai_family) {
		case AF_INET:
			ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
			strcpy(hostpointer, res->ai_canonname);
			break;
		case AF_INET6:
			//	          ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
			break;
		}
		inet_ntop(res->ai_family, ptr, addrstr, 100);
		//printf("IPv%d address: %s (%s)\n", res->ai_family == PF_INET6 ? 6 : 4,
		//		addrstr, res->ai_canonname);
		res = res->ai_next;
	}

	//  return 0 ;
	strcpy(ippointer, addrstr);
	// strcpy(hostpointer,res->ai_canonname);

	return 0;
}

int RemoveFromConnectObjectList(int closefdnum) {

	if (closefdnum < 0 || closefdnum >= MY_FD_SETSIZE) {
		return -1;
	}

	int sockfd = ConnectFdList[closefdnum];

	if (close(sockfd) < 0) {
		return -1;
	}

	ConnectObjectList[closefdnum].localport = 0;
	ConnectObjectList[closefdnum].remotport = 0;

	memset(ConnectObjectList[closefdnum].hostname, 0,
			sizeof(ConnectObjectList[closefdnum].hostname));
	memset(ConnectObjectList[closefdnum].ipaddress, 0,
			sizeof(ConnectObjectList[closefdnum].ipaddress));
	FD_CLR(sockfd, &read_set);
	ConnectFdList[closefdnum] = -1;
	return 0;
}

int SendTCPMessage(int sendfdnum, char * message) {
	int sock_fd = ConnectFdList[sendfdnum];
	struct TCPMessageFormat * tcpmessage;
	tcpmessage = malloc(sizeof(struct TCPMessageFormat));
	tcpmessage->length = htons(4 + strlen(message));
	tcpmessage->pid = htons(getpid());
	tcpmessage->message = message;
	//tcpmessage->message[MAXLINE] = '\0';
	if (write(sock_fd, (char *) tcpmessage, tcpmessage->length + 8) < 0) {
		printf("writen() error");
		return -1;
	}
	return 0;
}
