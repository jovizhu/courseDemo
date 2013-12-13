#include "UBTorrent.h"

fd_set read_set, RST;
int PeerFdList[MAXPEERNUM];
struct PeerObject PeerObjectList[MAXPEERNUM];
char My_ip[MAXLINE];
char My_port[MAXLINE];
char Torrent_file[MAXLINE];

int main(int argc, char * argv[]) {
	// init the connection list for the ubtorrent;
	memset(PeerFdList, 0, sizeof(PeerFdList));
	memset(PeerObjectList, 0, sizeof(PeerObjectList));
	memset(Torrent_file, 0, sizeof(Torrent_file));
	//uint8_t FILECONTENT[100][262144];
	int FILECONTENTFLAG[100];

	//memset(FILECONTENT, 0, sizeof(FILECONTENT));
	memset(FILECONTENTFLAG, 0, sizeof(FILECONTENTFLAG));

	// For defaul TCP listening
	uint32_t local_ip_address;
	uint16_t local_tcp_port;

	// parse the Input
	if (argc == 2) {
		strcpy(Torrent_file, argv[1]);
		local_tcp_port = DEFAULT_PORT;
	} else if (argc == 3) {
		strcpy(Torrent_file, argv[1]);
		local_tcp_port = atoi(argv[2]);
	} else {
		printf("Usage: ./UBtorrent <Torrent_file> [port]\n");
		exit(0);
	}

	int Torrent_file_end;
	int Torrent_file_flag;
	Torrent_file_flag = 0;
	Torrent_file_end = strlen(Torrent_file) - 1;
	if (Torrent_file_end < 7)
		Torrent_file_flag = 1;
	if (Torrent_file[Torrent_file_end] != 't' || Torrent_file[Torrent_file_end
			- 1] != 'n' || Torrent_file[Torrent_file_end - 2] != 'e'
			|| Torrent_file[Torrent_file_end - 3] != 'r'
			|| Torrent_file[Torrent_file_end - 4] != 'r'
			|| Torrent_file[Torrent_file_end - 5] != 'o'
			|| Torrent_file[Torrent_file_end - 6] != 't'
			|| Torrent_file[Torrent_file_end - 7] != '.') {
		Torrent_file_flag = 1;
	}
	if (Torrent_file_flag == 1) {
		printf("Usage: ./UBtorrent <Torrent_file> [port]\n");
		printf("Detect: input .torrent name error\n");
		printf("Please input a correct .torrent file\n");
		exit(0);
	}

	if ((local_ip_address = Getlocalip()) == 0) {
		perror("Getlocalip()");
		exit(1);
	}
	printf("local ip is %x \n", local_ip_address);

	int i;
	int loopnum;
	int file_fd;
	uint8_t filecontent[MAXLINE];

	//Init some important struct for the ubtorrent
	struct MetaStruct * MetaInfo;
	MetaInfo = malloc(sizeof(struct MetaStruct));
	memset(MetaInfo, 0, sizeof(MetaInfo));
	MetaInfo->port = local_tcp_port;

	struct AnnounceRes * announceres;
	announceres = malloc(sizeof(struct AnnounceRes));
	memset(announceres, 0, sizeof(announceres));
	announceres->peernum = 0;
	announceres->LocalIP = local_ip_address;
	announceres->LocalPort = local_tcp_port;

	// Get the METAINFO
	GetMetainfo(Torrent_file, MetaInfo);

	printf("Please input user commands\n");
	printf("For exit the program, please type \"quit\"\n");
	fflush(stdout);

	//******************************************************************************
	//******************************************************************************
	//******************************************************************************
	//***********************************************************
	// in the following,Set Up the  tcp server listening;
	int listen_fd;
	const int on = 1;
	struct sockaddr_in server_address;
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket() error");
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
		printf("setsockopt() failed");
	}

	// bind server_address to the socket
	if (bind(listen_fd, (struct sockaddr *) &server_address,
			sizeof(server_address)) < 0) {
		printf("bind() failed");
	}

	// actually turn the socket into a passive socket
	if (listen(listen_fd, LISTENQ) < 0) {
		printf("listen() failed");
	}

	// setting things up for select
	int maxfd;
	int num_ready_fds;
	FD_ZERO(&read_set);
	maxfd = listen_fd;
	FD_SET(listen_fd, &read_set);
	maxfd = max(STDIN_FILENO, maxfd);
	FD_SET(STDIN_FILENO,&read_set);

	// for TCP Listening
	struct sockaddr_in client_address;
	socklen_t client_SA_len;
	int tcp_fd;
	uint16_t remoteport;
	uint8_t peermessagecontent[MAXLINE];
	// End TCP Listening

	// for New Tcp Listening
	int n = 0;
	// End New TCP Listening

	// for STDIN
	char commandline[MAXLINE];
	int flag = 0;
	// End STDIN


	while (1) {
		RST = read_set;
		// set the num_ready_fds
		if ((num_ready_fds = select(maxfd + 1, &RST, NULL, NULL, NULL)) < 0) {
			/*
			 * there are two cases where select fails: an actual error has occured
			 * or we're interrupted by SIGCHLD. If the later happens, we just go back
			 * to selecting
			 */
			if (errno == EINTR) {
				continue;
			} else {
				printf("select() failed");
			}
		}

		//**********************************************************************
		// New TCP SERVER LISTENING

		for (loopnum = 0; loopnum < MY_FD_SETSIZE; loopnum++) { /* check all clients for data */
			if ((tcp_fd = PeerFdList[loopnum]) > 0) {
				if (FD_ISSET(tcp_fd , &RST)) {
					sleep(1);
					memset(peermessagecontent, 0, sizeof(peermessagecontent));
					printf("Communication from other Peers # %d \n", loopnum);
					if ((n = read(tcp_fd, peermessagecontent, MAXLINE)) == 0) {
						/* connection closed by client */
						close(tcp_fd);
						FD_CLR(tcp_fd, &read_set);
						PeerFdList[loopnum] = -1;
						printf("Connection # %d has been closed \n", loopnum);
					} else if (n < 0) {
						printf("system error \n");
					} else {
						while (n > 0) {
							sleep(1);
							printf("\nMessage from Other Peer is %s \n",
									peermessagecontent);
							fflush(stdout);

							if (peermessagecontent[0] == 19) {
								printf("*********************\nThis Send Hand Shake Message\n********************\n");

								if (PeerObjectList[loopnum].MsgHandShakeFlag
										== 0) {

									SendHandShakeMessage2(tcp_fd, announceres,
											MetaInfo);
									PeerObjectList[loopnum].MsgHandShakeFlag
											= 1;

								} else {
									printf("Send Bit Field Messge \n");
									SendBitFieldMessage2(tcp_fd, announceres,
											MetaInfo);
								}

								if (MetaInfo->fileflag > 0) {
									printf("Send Unchock Message %d \n",
											MetaInfo->fileflag);
									SendUnchockMessage2(tcp_fd, announceres,
											MetaInfo);
									//sleep(2);
								}

								printf("The Left Message Length not proceed is %d\n",n);
								n = n - 68;
								memcpy(peermessagecontent, peermessagecontent
										+ 68, n);

								continue;



							} else if (peermessagecontent[4] == 5) {
								//bit field message
								uint32_t msglength;
								memcpy(&msglength, peermessagecontent, 4);
								msglength = ntohl(msglength);


								printf(
										"*********************\nThis is BitFiled message \n\n********* %d *******\n",
										loopnum);

								printf("BitField message length is %d\n",
																	msglength);
								PeerObjectList[loopnum].bitfiled
										= peermessagecontent[5];
								//sleep(1);
/*
								printf("Bit Field Message ***************\n");
								for (i = 0; i < 20; i++) {

									printf("%d:%d\n", i, peermessagecontent[i]);
								}*/

								if (PeerObjectList[loopnum].MsgBitFieldFlag
										== 0)
									SendBitFieldMessage2(tcp_fd, announceres,
											MetaInfo);

								printf("The Left Message Length not proceed is %d\n",n);
								n = n - msglength - 4;
								memcpy(peermessagecontent, peermessagecontent
										+ msglength + 4, n);
								continue;
							} else if ((peermessagecontent[4] == 2)) {
								//interest message

								printf(
										"*********************\nThis is Interest message \n\n****** %d **********\n",
										loopnum);
								//sleep(1);
								uint8_t temp_uint8 = 1;
								PeerObjectList[loopnum].interestflag
										= temp_uint8;

								if (PeerObjectList[loopnum].MsgChockFlag == 0) {
									SendUnchockMessage2(tcp_fd, announceres,
											MetaInfo);
									PeerObjectList[loopnum].MsgChockFlag = 1;
								}

								printf("The Left Message Length not proceed is %d\n",n);
								n = n - 5;
								memcpy(peermessagecontent, peermessagecontent
										+ 5, n);
								continue;

							} else if ((peermessagecontent[4] == 1)) {
								//unchock message

								printf(
										"*********************\nThis is Unchock message \n\n****************\n");
								//sleep(1);

								if (PeerObjectList[loopnum].interestflag == 0) {
									sendInterestMessage2(tcp_fd, announceres,
											MetaInfo);
									PeerObjectList[loopnum].interestflag = 1;
								}

								//sleep(1);
								sendRequestMessage2(tcp_fd, announceres,
										MetaInfo, 0);

								printf("The Left Message Length not proceed is %d\n",n);
								n = n - 5;
								memcpy(peermessagecontent, peermessagecontent
										+ 5, n);
								continue;

							} else if ((peermessagecontent[4] == 3)) {
								//UnInterest message

								printf(
										"*******************\nThis is UnInterest message \n****************\n");
								//sleep(1);

								PeerObjectList[loopnum].interestflag = 0;

								printf("The Left Message Length not proceed is %d\n",n);
								n = n - 5;

								memcpy(peermessagecontent, peermessagecontent
										+ 5, n);
								continue;

							} else if ((peermessagecontent[4] == 0)) {

								//chock message
								printf(
										"*********************\nThis is chock message \n****************\n");
								//sleep(1);

								PeerObjectList[loopnum].chockflag = 1;

								printf("The Left Message Length not proceed is %d\n",n);
								n = n - 5;
								memcpy(peermessagecontent, peermessagecontent
										+ 5, n);
								continue;
							} else if ((peermessagecontent[4] == 7)) {
								//piece message

								printf(
										"*********************\nTHis is piece message \n****************\n");

								if (PeerObjectList[loopnum].time_up == 0) {
									if (gettimeofday(
											&PeerObjectList[loopnum].up, NULL)
											< 0) {
										perror("gettimeofday()");
									}
									PeerObjectList[loopnum].time_up = 1;
								}



								uint32_t filelen;
								uint32_t offset;
								uint32_t piece_index;
								memcpy(&filelen, peermessagecontent, 4);
								filelen = ntohl(filelen);

								memcpy(&piece_index, peermessagecontent + 5, 4);
								piece_index = ntohl(piece_index);

								memcpy(&offset, peermessagecontent + 9, 4);
								offset = ntohl(offset);

								printf("Offset is %ld \n", offset);
								filelen = filelen - 9;
								//file_fd = file_fd + offset; // ?
								printf("The file length in Hex is %ld \n", filelen);



								file_fd = open(MetaInfo->filename,
																 O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

								if(lseek(file_fd,offset+piece_index*MetaInfo->piecelength,SEEK_SET))
								{
									printf("LSEEK ERROR \n");
								}


								 write(file_fd, &peermessagecontent[13], n - 13);


								//copy the file content to the memory
								/*char* Currentpointer = &FILECONTENT[piece_index][0];
								memcpy(Currentpointer, &peermessagecontent[13],
										n - 13);*/



								FILECONTENTFLAG[piece_index] = 1;
								//Currentpointer = Currentpointer + n - 13;

								memset(filecontent, 0, sizeof(filecontent));
								uint32_t templen = n - 13;

								int n_piece;

								while (templen < (filelen - MAXLINE)) {
									sleep(1);
									n_piece
											= read(tcp_fd, filecontent, MAXLINE);
									templen = templen + n_piece;
									printf("The Receiving Length is %d  Sum of Length is %ld \n",
											n_piece, templen);
									fflush(stdout);

									 write(file_fd, filecontent, n_piece);

									//memcpy(Currentpointer, filecontent, n_piece);
									//Currentpointer = Currentpointer + n_piece;

									memset(filecontent, 0, sizeof(filecontent));
								}

								//sleep(1);
								n_piece = read(tcp_fd, filecontent, filelen
										- templen);
								templen = templen + n_piece;
								printf("The Receiving Length is %d  Sum of Length is %02x \n",
										n_piece, templen);
								fflush(stdout);

								 write(file_fd, filecontent, n_piece);
								 close(file_fd);

								//memcpy(Currentpointer, filecontent, n_piece);
								//Currentpointer = Currentpointer + n_piece;

								memset(filecontent, 0, sizeof(filecontent));

								PeerObjectList[loopnum].downloaded
										= PeerObjectList[loopnum].downloaded
												+ templen;

								printf(
										"Check Whether need to be close the connection and finishing downloading \n");

								int sum = 0;

								for (i = 0; i < MetaInfo->blocknum; i++) {
									sum = sum + FILECONTENTFLAG[i];
								}

								if (sum == MetaInfo->blocknum) {
									/*//Begin to write the file;
									file_fd = open(MetaInfo->filename, O_RDWR
											| O_CREAT, S_IRUSR | S_IWUSR);

									for (i = 0; i < (MetaInfo->blocknum - 1); i++) {
										write(file_fd, &FILECONTENT[i][0],
												MetaInfo->piecelength);
										printf(
												"Write the piece %d to the file, the length is \n",
												i,MetaInfo->piecelength);
									}

									printf(
											"Write the last pieces %d to the file\nThe Writing Length is %ld",
											i, (MetaInfo->filesize
													% MetaInfo->piecelength));

									write(file_fd, &FILECONTENT[i][0],
											(MetaInfo->filesize
													% MetaInfo->piecelength));*/


									//downloaded ended and close the connection
									close(tcp_fd);
									FD_CLR(tcp_fd, &read_set);
									PeerFdList[loopnum] = -1;
									printf(
											"Downloading Ended and Connection # %d has been closed \n",
											loopnum);
								}

								for (i = 0; i < MetaInfo->blocknum; i++) {
									if (FILECONTENTFLAG[i] == 0) {
										sendRequestMessage2(tcp_fd,
												announceres, MetaInfo, i);
										printf(
												"Request message for piece %d \n",
												i);
										break;
									}
								}


								printf("The Left Message Length is %d\n I will not proceed the message following with this long piece message\n The request need to be resend",n);
								printf("The Left Message Length not proceed is %d\n",n);
								n = 0;
								// I do not want accept any message following the piece message
								// Pls resend it.
								continue;

							} else if ((peermessagecontent[4] == 6)) {

								printf("THis is the request message \n"
									"****************\n"
									" need to reply the piece message\n"
									" %d \n", n);

								/*	for (i = 0; i < 50; i++) {

								 printf("%d:%d\n", i, peermessagecontent[i]);
								 }*/
								//sleep(2);

								if (PeerObjectList[loopnum].time_down == 0) {
									if (gettimeofday(
											&PeerObjectList[loopnum].down, NULL)
											< 0) {
										perror("gettimeofday()");
									}
									PeerObjectList[loopnum].time_down = 1;
								}

								char messagecontent[17];

								uint32_t index;
								uint32_t begin;
								uint32_t length;
								uint32_t readstart;
								uint32_t templen;
								int maxline;
								int messagestart = 0;
								int messagelength = n;

								uint8_t sendcontent[MAXLINE];
								uint32_t result;

								memset(&sendcontent, 0, MAXLINE);
								memset(messagecontent, 0, 17);
								memcpy(messagecontent, peermessagecontent
										+ messagestart, 17);
								memcpy(&index, messagecontent + 5, 4);
								memcpy(&begin, messagecontent + 9, 4);
								memcpy(&length, messagecontent + 13, 4);

								index = ntohl(index);
								begin = ntohl(begin);
								length = ntohl(length);

								templen = 0;
								printf("index is %ld \n begin is %ld \n length is %ld\n",index,begin,length);
								readstart = MetaInfo->piecelength * index
										+ begin;
								maxline = MAXLINE;

								printf("the reading index is %d \n", index);
								printf("the reading begin is %d \n", begin);
								printf("the reading start is %ld \n",
										readstart);

								templen = readstart;
								int n_request =0;

								file_fd = open(MetaInfo->filename, O_RDONLY);
								while (templen > maxline) {
									memset(filecontent, 0, sizeof(filecontent));
									n_request = read(file_fd, filecontent,
											MAXLINE);
									templen = templen - n_request;
									printf(
											"Reading length is %d  Sum of length is %02x \n",
											n_request, templen);
									fflush(stdout);
									memset(filecontent, 0, sizeof(filecontent));
								}

								n_request = read(file_fd, filecontent, templen);

								memset(filecontent, 0, sizeof(filecontent));

								templen = 0;
								printf("Read from %02x\n", readstart);
								printf("Total need to Sending  is %02x\n", length);

								uint32_t head = length + 9;
								uint32_t index_net = htonl(index);
								uint32_t begin_net = htonl(begin);
								uint32_t length_net = htonl(head);

								memcpy(&sendcontent, &length_net, 4);
								sendcontent[4] = 7;
								memcpy(&sendcontent[5], &index_net, 4);
								memcpy(&sendcontent[9], &begin_net, 4);
								/*for (i = 0; i < 20; i++) {

									printf("%d:%d\n", i, sendcontent[i]);
									fflush(stdout);
								}*/

								// send the piece message header
								result = write(tcp_fd, &sendcontent, 13);
								printf("The Message Header is Sending and the Length is %ld \n", result);

								// send the message
								templen = length;
								uint32_t u_int32;

								while (templen > MAXLINE) {
									u_int32 = read(file_fd, &sendcontent,
											MAXLINE);
									templen = templen - u_int32;
									printf(
											"Reading length is %ld  the left is %02x \n",
											u_int32, templen);
									fflush(stdout);
									//sleep(1);
									result = write(tcp_fd, &sendcontent,
											u_int32);
									printf("The Sending Length is %ld \n",
											result);
									memset(&sendcontent, 0, MAXLINE);
								}

								u_int32 = read(file_fd, &sendcontent, templen);
								templen = templen - u_int32;
								printf(
										"Reading length is %ld  the left is %02x \n",
										u_int32, templen);
								fflush(stdout);
								sleep(1);
								result = write(tcp_fd, &sendcontent, u_int32);
								printf("The Sending Length is %ld \n", result);

								close(file_fd);

								PeerObjectList[loopnum].uploaded
										= PeerObjectList[loopnum].uploaded
												+ templen;

								printf("One Request Processing End\n");

								n = n - 17;
								memcpy(peermessagecontent, peermessagecontent
										+ 17, n);
								printf("The Left Message Length not proceed is %d\n",n);
								continue;
							}

							// will not be run because the ending is continue
							printf("the length n is %d, and no matching \n", n);
							n = 0;
						} // end of while(n>0)

					}
					if (--num_ready_fds <= 0)
						break; /* no more readable descriptors */
				}
			}
		}
		// end New tCP SERVER LISTENING


		//**********************************************************************
		// TCP SERVER LISTENING
		// if listen_fd has something to say
		if (FD_ISSET(listen_fd, &RST)) {
			printf("TCP connecton accepted  555\n");
			client_SA_len = sizeof(client_address);
			if ((tcp_fd = accept(listen_fd,
					(struct sockaddr *) &client_address, &client_SA_len)) < 0) {
				if (errno == EINTR) {
					continue; /* restart accepting new connections */
				} else { /* otherwise, some error has occured */
					printf("accept() failed");
				}
			}

			// Comments for the following
			for (loopnum = 0; loopnum < MY_FD_SETSIZE; loopnum++) {
				if (PeerFdList[loopnum] <= 0) {
					printf("Add into new listen loop \n");
					PeerFdList[loopnum] = tcp_fd; //save descriptor
					char * ipaddress = inet_ntoa(client_address.sin_addr);
					remoteport = ntohs(client_address.sin_port);
					AddtoPeerObjectList(&PeerObjectList[loopnum], ipaddress,
							remoteport);
					printf("ipaddress %s \n", PeerObjectList[loopnum].ipaddress);

					printf("remotport %ld \n",
							PeerObjectList[loopnum].remotport);
					break;
				}
				printf("Run to the next PeerFdList \n");

			}

			if (loopnum == MY_FD_SETSIZE) {
				printf("too many clients \n");
				continue;
			}

			FD_SET(tcp_fd, &read_set); /* add new descriptor to set */
			if (tcp_fd >= maxfd)
				maxfd = tcp_fd; /* for select */

			if (--num_ready_fds <= 0)
				continue;
		} /* if listen_fd is ready */

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

			if (strcmp(commandline, "quit") == 0) {
				AnnounceToTracker(MetaInfo, announceres, 1);
				printf("Chatty exited, OH OH OH OH YOU YOU YOU\n");
				exit(0);
			}

			else if (strcmp(commandline, "metainfo") == 0) {
				Print_MetaInfo(MetaInfo);
			}

			else if (strcmp(commandline, "announce") == 0) {

				//Announce Function
				AnnounceToTracker(MetaInfo, announceres, 0);
				Print_Trackerinfo(announceres);

				for (i = 0; i < announceres->peernum; i++) {

					int jumpflag = 0;
					int peernum = SendHandShakeMessage(announceres, MetaInfo);

					if (peernum < 0) {
						printf("peer number is less than zero ,break \n");
						break;
					}

					for (loopnum = 0; (peernum >= 0) && (loopnum
							< MY_FD_SETSIZE) && (jumpflag == 0); loopnum++) {
						if (PeerFdList[loopnum] <= 0) {
							printf("Add into new listenFD loop %d \n", loopnum);
							PeerFdList[loopnum]
									= announceres->PeerList[peernum].conn_fd; //save descriptor


							struct in_addr address_struct;
							address_struct.s_addr = htonl(
									announceres->PeerList[peernum].ipaddress);

							char * ipaddress = inet_ntoa(address_struct);

							remoteport = announceres->PeerList[peernum].port;

							AddtoPeerObjectList(&PeerObjectList[loopnum],
									ipaddress, remoteport);

							printf("ip address %s \n",
									PeerObjectList[loopnum].ipaddress);

							printf("remote port %ld \n",
									PeerObjectList[loopnum].remotport);

							PeerObjectList[loopnum].MsgHandShakeFlag = 1;

							PeerObjectList[loopnum].downloaded = 0;
							PeerObjectList[loopnum].uploaded = 0;

							tcp_fd = announceres->PeerList[peernum].conn_fd;

							jumpflag = 1;
							break;
						}
						printf("Jump to the next loopnum \n");
					}

					if (tcp_fd <= 0)
						break;

					if (loopnum == MY_FD_SETSIZE) {
						printf("too many clients \n");
						continue;
					}

					FD_SET(tcp_fd, &read_set); /* add new descriptor to set */
					if (tcp_fd >= maxfd)
						maxfd = tcp_fd; /* for select */

					//SendBitFieldMessage2(tcp_fd, announceres, MetaInfo);

					//


					if (MetaInfo->fileflag > 0) {
						//SendUnchockMessage2(tcp_fd,announceres, MetaInfo);
					} else {
						sendInterestMessage2(tcp_fd, announceres, MetaInfo);
					}
					// Update the PeerObject List;

				}
				//

			}

			else if (strcmp(commandline, "status") == 0) {
				//status Function
				if (Print_Status(MetaInfo) < 0) {
					printf("Error: the file size doesn't match\n");
					printf("Please delete the file and restart the program\n");
					exit(1);
				}
			}

			else if (strcmp(commandline, "trackerinfo") == 0) {
				//trackerinfo Function
				Print_Trackerinfo(announceres);
			}

			else if (strcmp(commandline, "show") == 0) {
				//show Function
				struct timeval current;
				memset(&current, 0, sizeof(current));
				if (gettimeofday(&current, NULL) < 0) {
					perror("gettimeofday()");
				}

				printf(
						"ID | IP address      | Status | Down/s   | Up/s     | Bitfield         |\n");
				printf(
						"------------------------------------------------------------------------\n");
				for (loopnum = 0; loopnum < MY_FD_SETSIZE; loopnum++) {
					if (PeerFdList[loopnum] > 0) {
						int time_down = current.tv_sec
								- PeerObjectList[loopnum].down.tv_sec;
						int time_up = current.tv_sec
								- PeerObjectList[loopnum].up.tv_sec;
						int Show_downloaded =
								PeerObjectList[loopnum].downloaded;
						int Show_uploaded = PeerObjectList[loopnum].uploaded;

						printf("%3d|", loopnum);
						printf(" %15s |", PeerObjectList[loopnum].ipaddress);
						printf(" %d%d", PeerObjectList[loopnum].chockflag,
								PeerObjectList[loopnum].interestflag);
						printf("01   |"); // always interest and unchoke
						printf(" %8d |", Show_uploaded / time_down);
						printf(" %8d |", Show_downloaded / time_up);
						printf(" %d\n", PeerObjectList[loopnum].bitfiled);

					}
				}
			} else {
				printf("Unknown Command.\n");
			}
		}
	}
	exit(0);

	return 0;
}
