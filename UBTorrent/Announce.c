
#include "Announce.h"

void AnnounceToTracker(MetaStruct * Metainfo, AnnouceRes * announceres, int exit_flag) {
	char temp[255];
	char destination[255];
	char tcpport[255];
	char receivedmessage1[8096];
	char receivedmessage2[8096];
	char receivedmessage3[8096];
	char receivedmessage4[8096];
	char receivedmessage5[8096];
	char receivedmessage6[8096];
	int bytes_read;
	int i;
	int k;
/*
	memset(temp, 0, sizeof(temp));
	memset(destination, 0, sizeof(destination));
	memset(tcpport, 0, sizeof(tcpport));
	memset(receivedmessage1, 0, sizeof(receivedmessage1));
	memset(receivedmessage2, 0, sizeof(receivedmessage2));
	memset(receivedmessage3, 0, sizeof(receivedmessage3));
	memset(receivedmessage4, 0, sizeof(receivedmessage4));
	memset(receivedmessage5, 0, sizeof(receivedmessage5));
	memset(receivedmessage6, 0, sizeof(receivedmessage6));
*/

	strcpy(temp, Metainfo->announceURL + 7);
	printf(" temp %s \n", temp);
	char * portstart = strchr(temp, '\:');
	char * portend = strchr(temp, '\/');

	printf(" portstart is %s \n", portstart);
	printf(" portend is  %s \n", portend);

	strncpy(destination, temp, portstart - temp);
	destination[portstart - temp] = '\0';
	printf("destination is %s \n", destination);
	strncpy(tcpport, portstart + 1, portend - portstart - 1);
	tcpport[portend - portstart - 1] = '\0';
	printf("tcpport is %s \n", tcpport);

	int sock_fd, pton_ret;
	struct sockaddr_in server_address;
	uint16_t server_port = atoi(tcpport);

	printf("Server port is %d \n", server_port);
	char server_ip[255];

	strcpy(server_ip, destination);
	printf("Server Ip is %s \n", server_ip);
	struct addrinfo hints;


	server_ip[255] = '\0';
	// get port & address from input, if any
	//parse_input(argc, argv, &server_port, server_ip);
	//note("Server (IP, port) = (%s, %d)", server_ip, server_port);

	// create the 'active' socket to send requests to
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("socket() error");

	// fill out the server_address structure.
	memset((void *) &server_address, 0, sizeof(server_address));
	memset((void *) &hints, 0, sizeof(hints));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);

	pton_ret = inet_pton(AF_INET, server_ip, &server_address.sin_addr);

	// tries to establish a tcp connection to the server
	if (connect(sock_fd, (struct sockaddr *) &server_address,
			sizeof(server_address)) < 0) {
		printf("connect() error");
	}

	//char * annoucemessage="GET /announce?info_hash=_tWL%26%BD%C4%BDkEn%FD%7E1%2CJ3%40s%1B&peer_id=M4-4-2--5ffd511f4079&port=6881&key=585b8345&uploaded=1&downloaded=0&left=0&compact=1&event=started HTTP/1.1\r\n\r\n";

	char announcemessage[8096];

	memset(announcemessage, 0, sizeof(announcemessage));
	strcpy(announcemessage, "GET /announce?info_hash=");

	int cpy_loc = strlen("GET /announce?info_hash=");
	printf("Pre INho Hash ANNOUNCE MESSAGE IS %s, length is %d \n ",
			announcemessage, strlen(announcemessage));

	printf("Info Hash is : ");
	/*for (i = 0; i < 20; i++) {
	 announcemessage[cpy_loc + i] = Metainfo->infohash[i];
	 printf("%2x", (uint8_t) announcemessage[cpy_loc + i]);
	 }*/
	printf("\n");
	printf("print by string is %s \n", Metainfo->infohash);
	printf("\n");

	cpy_loc = cpy_loc + EncodeHashtable(&announcemessage[cpy_loc],
			Metainfo->infohash);

	printf(" After Info Hash ANNOUNCE MESSAGE IS %s \n ", announcemessage);

	memcpy(&announcemessage[cpy_loc], "&peer_id=", 9);
	cpy_loc = cpy_loc + 9;
	printf("pre my ID ANNOUNCE MESSAGE IS %s \n ", announcemessage);
	/*

	 printf("my id is: ");
	 for (i = 0; i < 20; i++) {
	 announcemessage[cpy_loc + i] = Metainfo->myid[i];
	 printf("%2x", (uint8_t) announcemessage[cpy_loc + i]);
	 }
	 printf("\n");
	 */

	cpy_loc = cpy_loc + EncodeHashtable(&announcemessage[cpy_loc],
			Metainfo->myid);

	printf("after my id ANNOUNCE MESSAGE IS %s \n ", announcemessage);

	memcpy(&announcemessage[cpy_loc], "&port=", 6);
	cpy_loc = cpy_loc + 6;
	printf("befor port ANNOUNCE MESSAGE IS %s \n ", announcemessage);

	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%d", Metainfo->port);

	memcpy(&announcemessage[cpy_loc], temp, strlen(temp));
	cpy_loc = cpy_loc + strlen(temp);
	printf("after port ANNOUNCE MESSAGE IS %s \n ", announcemessage);


	if (Metainfo->fileflag > 0 && exit_flag == 1) {
		printf("I was a seeder, but I'm quit\n");
		memcpy(
				&announcemessage[cpy_loc],
				"&key=585b8345&uploaded=1&downloaded=0&left=0&compact=1&event=stopped HTTP/1.1\r\n\r\n",
				strlen(
						"&key=585b8345&uploaded=1&downloaded=0&left=0&compact=1&event=stopped HTTP/1.1\r\n\r\n"));
		printf("Annoucne Message is : %s \n", announcemessage);
		if (write(sock_fd, (char *) announcemessage, strlen(announcemessage)) < 0) {
			printf("writen() error");
		}
		printf("Announce End\n");
		exit(0);
	}

	if (Metainfo->fileflag <= 0 && exit_flag == 1) {
		printf("I was a downloader, but I'm quit\n");
		memcpy(
				&announcemessage[cpy_loc],
				"&key=585b8345&uploaded=1&downloaded=1&left=0&compact=1&event=stopped HTTP/1.1\r\n\r\n",
				strlen(
						"&key=585b8345&uploaded=1&downloaded=1&left=0&compact=1&event=stopped HTTP/1.1\r\n\r\n"));
		printf("Annoucne Message is : %s \n", announcemessage);
		if (write(sock_fd, (char *) announcemessage, strlen(announcemessage)) < 0) {
			printf("writen() error");
		}
		printf("Announce End\n");
		exit(0);
	}


	if (Metainfo->fileflag > 0) {
		printf("Work as a seeder \n");
		memcpy(
				&announcemessage[cpy_loc],
				"&key=585b8345&uploaded=1&downloaded=0&left=0&compact=1&event=started HTTP/1.1\r\n\r\n",
				strlen(
						"&key=585b8345&uploaded=1&downloaded=0&left=0&compact=1&event=started HTTP/1.1\r\n\r\n"));
	} else {
		printf("Work as a downloader \n");
		memcpy(&announcemessage[cpy_loc],
				"&key=585b8345&uploaded=1&downloaded=0&left=", strlen(
						"&key=585b8345&uploaded=1&downloaded=0&left="));
		cpy_loc = cpy_loc + strlen(
				"&key=585b8345&uploaded=1&downloaded=0&left=");

		printf("file size create\n");
		memset(temp, 0, sizeof(temp));
		sprintf(temp, "%d", Metainfo->filesize);
		memcpy(&announcemessage[cpy_loc], temp, strlen(temp));
		cpy_loc = cpy_loc + strlen(temp);
		printf("ANNOUNCE MESSAGE IS %s \n ", announcemessage);

		memcpy(&announcemessage[cpy_loc],
				"&compact=1&event=started HTTP/1.1\r\n\r\n", strlen(
						"&compact=1&event=started HTTP/1.1\r\n\r\n"));
		cpy_loc = cpy_loc + strlen("&compact=1&event=started HTTP/1.1\r\n\r\n");
	}

	printf("Annoucne Message is : %s \n", announcemessage);
	if (write(sock_fd, (char *) announcemessage, strlen(announcemessage)) < 0) {
		printf("writen() error");
	}

	printf("Message is %s \n", announcemessage);

	sleep(2);

	bytes_read = readline(sock_fd, receivedmessage1, sizeof(receivedmessage1));

	printf(" received message 1 is %s \n", receivedmessage1);

	bytes_read = readline(sock_fd, receivedmessage2, sizeof(receivedmessage2));

	printf(" received message 2 is %s \n", receivedmessage2);

	bytes_read = readline(sock_fd, receivedmessage3, sizeof(receivedmessage3));

	printf(" received message 3 is %s \n", receivedmessage3);

	bytes_read = readline(sock_fd, receivedmessage4, sizeof(receivedmessage4));

	printf(" received message 4 is %s \n", receivedmessage4);
	sleep(2);

	bytes_read = readline(sock_fd, receivedmessage5, sizeof(receivedmessage5));

	printf(" received message 5 is %s \n, bytes read is %d \n",
			receivedmessage5, bytes_read);

	for (i = 0; i < 200; i++) {
		printf("%c", receivedmessage5[i]);
	}
	int ptr = 0;
	for (k = 0; k <= bytes_read; k++) {
		//printf("%d\  %d  \n", k, seed[k]);
		if ((receivedmessage5[k] == 112) && (receivedmessage5[k + 1] == 101)
				&& (receivedmessage5[k + 2] == 101) && (receivedmessage5[k + 3]
				== 114) && (receivedmessage5[k + 4] == 115)) {
			printf("\n k position is %c %d\n", receivedmessage5[k + 5], k);
			if (receivedmessage5[k + 7] == '\:') {
				ptr = k + 8;
				printf("8 \n");
			} else if (receivedmessage5[k + 8] == '\:') {
				ptr = k + 9;
				printf("9 \n");
			} else if (receivedmessage5[k + 9] == '\:') {
				ptr = k + 10;
				printf("10 \n");
			} else {
				ptr = k + 7;
			}
		}

	}

	printf(" peer num is %d \n", announceres->peernum);
	int peernum = announceres->peernum;

	while (ptr < bytes_read - 2) {
		memcpy(&(announceres->PeerList[peernum].ipaddress),
				&receivedmessage5[ptr], 4);
		announceres->PeerList[peernum].ipaddress =ntohl(announceres->PeerList[peernum].ipaddress);

		memcpy(&(announceres->PeerList[peernum].port), &receivedmessage5[ptr
				+ 4], 2);
		announceres->PeerList[peernum].port = ntohs(
				announceres->PeerList[peernum].port);



		printf("\n IP port is %ld \n \n", announceres->PeerList[peernum].port);
		announceres->PeerList[peernum].handshakeflag=0;
		announceres->PeerList[peernum].conn_fd=0;
		peernum++;
		ptr = ptr + 6;
		printf(" peer num is %d \n", peernum);
	}
	announceres->peernum = peernum;

	bytes_read = readline(sock_fd, receivedmessage6, sizeof(receivedmessage6));
	printf(" \n message 6  is  %s \n", receivedmessage6);

	be_node *n = be_decoden(receivedmessage5, sizeof(receivedmessage5));
	GetAnnouceInfo(n, 0, announceres);
	be_free(n);

	printf(" \n Annoucne End \n");
}

void GetAnnouceInfo(be_node *node, ssize_t indent, AnnouceRes * announceres) {
	size_t i;

	printf("strart \n");
	indent = abs(indent);

	switch (node->type) {
	case BE_DICT:
		for (i = 0; node->val.d[i].val; ++i) {
			printf("%s : ", node->val.d[i].key);
			if (strcmp(node->val.d[i].key, "complete") == 0) {
				announceres->complete = node->val.d[i].val->val.i;
				printf("complete is : %lld \n", announceres->complete);

			} else if (strcmp(node->val.d[i].key, "incomplete") == 0) {
				announceres->incomplete = node->val.d[i].val->val.i;
				printf("incomplete is : %lld \n", announceres->incomplete);
			} else if (strcmp(node->val.d[i].key, "downloaded") == 0) {
				announceres->downloaded = node->val.d[i].val->val.i;
				printf("downloaded is : %lld \n", announceres->downloaded);
			} else if (strcmp(node->val.d[i].key, "interval") == 0) {
				announceres->interval = node->val.d[i].val->val.i;
				printf("interval is : %lld \n", announceres->interval);
			} else if (strcmp(node->val.d[i].key, "min interval") == 0) {
				announceres->mininterval = node->val.d[i].val->val.i;
				printf("mininterval is : %lld \n", announceres->mininterval);
			}
			GetMetaFileInfo(node->val.d[i].val, -(indent + 1), announceres);
		}
		break;
	default:
		break;
	}
}

int EncodeHashtable(char * destination, uint8_t * source) {
	int loc = 0;
	int i;
	char temp[10];
	printf("Enter\n");
	for (i = 0; i < 20; i++) {

		if ((source[i] == 46) || (source[i] == 45) || (source[i] == 95)
				|| (source[i] == 126) || ((source[i] >= 48)
				&& (source[i] <= 57)) || ((source[i] >= 65)
				&& (source[i] <= 90)) || ((source[i] >= 97) && (source[i]
				<= 122))) {

			destination[loc] = source[i];
			loc = loc + 1;
			//printf("%d\n", loc);
		} else {

			memset(temp, 0, sizeof(temp));

			sprintf(temp, "%02x", source[i]);

			destination[loc] = 37;

			memcpy(&destination[loc + 1], temp, strlen(temp));
			loc = loc + strlen(temp) + 1;
			//printf("%d\n", loc);
		}
	}

	//printf("%d\n", loc);
	return loc;
}



void Print_Trackerinfo(AnnouceRes * announceres) {
	struct in_addr Print_address;
	char * Print_ipaddress;
	size_t i;

	printf(" complete | downloaded | incomplete | interval | min interval |\n");
	printf("---------------------------------------------------------------\n");
	printf(" %8lld | %10lld | %10lld | %8lld | %12lld |\n",
			announceres->complete,
			announceres->downloaded,
			announceres->incomplete,
			announceres->interval,
			announceres->mininterval);
	printf("---------------------------------------------------------------\n");
	printf("++++ Peer List :\n");
	printf(" IP               | Port       \n");
	for (i = 0; i < announceres->peernum; i++) {
		memset(&Print_address, 0, sizeof(Print_address));
		Print_address.s_addr = htonl(
				announceres->PeerList[i].ipaddress);
		Print_ipaddress = inet_ntoa(Print_address);
		printf(" %17s| %5ld\n",
				Print_ipaddress,
				announceres->PeerList[i].port);
	}

}
