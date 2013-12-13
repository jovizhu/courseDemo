#include "MessageSend.h"


int SendUnchokMessage(AnnouceRes * announceres, MetaStruct * metainfo) {

	printf("This is Unchock Message \n");

	struct sockaddr_in server_address;
	int sock_fd;
	int pton_ret;
	char message[2];
	int i;

	printf("announceres->peernum is %d \n", announceres->peernum);

	for (i = 0; i < announceres->peernum; i++) {

		if (announceres->LocalIP == announceres->PeerList[i].ipaddress) {
			printf("Local ip continue \n");
			continue;
		}

		if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			printf("socket() error");

		// fill out the server_address structure.
		memset((void *) &server_address, 0, sizeof(server_address));
		memset(message, 0, sizeof(message));

		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(announceres->PeerList[i].port);

		printf("send message to %d \n", announceres->PeerList[i].port);

		struct in_addr address_struct;
		address_struct.s_addr = htonl(announceres->PeerList[i].ipaddress);

		pton_ret = inet_pton(AF_INET, inet_ntoa(address_struct),
				&server_address.sin_addr);

		printf("send message to %s \n", inet_ntoa(server_address.sin_addr));

		if (pton_ret <= 0) {
			printf("inet_pton() error");
		}

		// tries to establish a tcp connection to the server
		if (connect(sock_fd, (struct sockaddr *) &server_address,
				sizeof(server_address)) < 0) {
			printf("connect() error");
		}

		message[0] = 1;
		message[1] = 1;
		write(sock_fd, (char *) message, 2);

		printf("Unchock Message Sended is  %s \n", message);
	}

	printf("This is Unchock Message  End\n");
	return 1;
}

int sendChockMessage(AnnouceRes * announceres, MetaStruct * metainfo) {

	printf("This is Chock Message \n");

	struct sockaddr_in server_address;
	int sock_fd;
	int pton_ret;
	char message[2];
	int i;

	printf("announceres->peernum is %d \n", announceres->peernum);

	for (i = 0; i < announceres->peernum; i++) {

		if (announceres->LocalIP == announceres->PeerList[i].ipaddress) {
			printf("Local ip continue \n");
			continue;
		}

		if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			printf("socket() error");

		// fill out the server_address structure.
		memset((void *) &server_address, 0, sizeof(server_address));
		memset(message, 0, sizeof(message));

		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(announceres->PeerList[i].port);

		printf("send message to %d \n", announceres->PeerList[i].port);

		struct in_addr address_struct;
		address_struct.s_addr = htonl(announceres->PeerList[i].ipaddress);

		pton_ret = inet_pton(AF_INET, inet_ntoa(address_struct),
				&server_address.sin_addr);

		printf("send message to %s \n", inet_ntoa(server_address.sin_addr));

		if (pton_ret <= 0) {
			printf("inet_pton() error");
		}

		// tries to establish a tcp connection to the server
		if (connect(sock_fd, (struct sockaddr *) &server_address,
				sizeof(server_address)) < 0) {
			printf("connect() error");
		}

		message[0] = 1;
		message[1] = 0;
		write(sock_fd, (char *) message, 2);

		printf("Chock Message Sended is  %s \n", message);
	}

	printf("This is Chock Message  End\n");

	return 1;

}

int sendInterestMessage(AnnouceRes * announceres, MetaStruct * metainfo) {
	printf("This is Interest Message \n");

	struct sockaddr_in server_address;
	int sock_fd;
	int pton_ret;
	char message[2];
	int i;

	printf("announceres->peernum is %d \n", announceres->peernum);

	for (i = 0; i < announceres->peernum; i++) {

		if (announceres->LocalIP == announceres->PeerList[i].ipaddress) {
			printf("Local ip continue \n");
			continue;
		}

		if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			printf("socket() error");

		// fill out the server_address structure.
		memset((void *) &server_address, 0, sizeof(server_address));
		memset(message, 0, sizeof(message));

		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(announceres->PeerList[i].port);

		printf("send message to %d \n", announceres->PeerList[i].port);

		struct in_addr address_struct;
		address_struct.s_addr = htonl(announceres->PeerList[i].ipaddress);

		pton_ret = inet_pton(AF_INET, inet_ntoa(address_struct),
				&server_address.sin_addr);

		printf("send message to %s \n", inet_ntoa(server_address.sin_addr));

		if (pton_ret <= 0) {
			printf("inet_pton() error");
		}

		// tries to establish a tcp connection to the server
		if (connect(sock_fd, (struct sockaddr *) &server_address,
				sizeof(server_address)) < 0) {
			printf("connect() error");
		}

		message[0] = 1;
		message[1] = 2;
		write(sock_fd, (char *) message, 2);

		printf("Interest Message Sended is  %s \n", message);
	}

	printf("This is interest Message  End\n");

	return 1;

	printf("This is Interest Message End\n");

}

int sendUnInterestMessage(AnnouceRes * announceres, MetaStruct * metainfo) {
	printf("This is Uninterest Message \n");

	struct sockaddr_in server_address;
	int sock_fd;
	int pton_ret;
	char message[2];
	int i;

	printf("announceres->peernum is %d \n", announceres->peernum);

	for (i = 0; i < announceres->peernum; i++) {

		if (announceres->LocalIP == announceres->PeerList[i].ipaddress) {
			printf("Local ip continue \n");
			continue;
		}

		if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			printf("socket() error");

		// fill out the server_address structure.
		memset((void *) &server_address, 0, sizeof(server_address));
		memset(message, 0, sizeof(message));

		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(announceres->PeerList[i].port);

		printf("send message to %d \n", announceres->PeerList[i].port);

		struct in_addr address_struct;
		address_struct.s_addr = htonl(announceres->PeerList[i].ipaddress);

		pton_ret = inet_pton(AF_INET, inet_ntoa(address_struct),
				&server_address.sin_addr);

		printf("send message to %s \n", inet_ntoa(server_address.sin_addr));

		if (pton_ret <= 0) {
			printf("inet_pton() error");
		}

		// tries to establish a tcp connection to the server
		if (connect(sock_fd, (struct sockaddr *) &server_address,
				sizeof(server_address)) < 0) {
			printf("connect() error");
		}

		message[0] = 1;
		message[1] = 3;
		write(sock_fd, (char *) message, 2);

		printf("Uninterest Message Sended is  %s \n", message);
	}

	printf("This is Uninterest Message  End\n");

	return 1;

}

int sendHaveMessage(AnnouceRes * announceres, MetaStruct * metainfo) {
	printf("This is have Message \n");

	struct sockaddr_in server_address;
	int sock_fd;
	int pton_ret;
	char message[2];
	int i;

	printf("announceres->peernum is %d \n", announceres->peernum);

	for (i = 0; i < announceres->peernum; i++) {

		if (announceres->LocalIP == announceres->PeerList[i].ipaddress) {
			printf("Local ip continue \n");
			continue;
		}

		if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			printf("socket() error");

		// fill out the server_address structure.
		memset((void *) &server_address, 0, sizeof(server_address));
		memset(message, 0, sizeof(message));

		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(announceres->PeerList[i].port);

		printf("send message to %d \n", announceres->PeerList[i].port);

		struct in_addr address_struct;
		address_struct.s_addr = htonl(announceres->PeerList[i].ipaddress);

		pton_ret = inet_pton(AF_INET, inet_ntoa(address_struct),
				&server_address.sin_addr);

		printf("send message to %s \n", inet_ntoa(server_address.sin_addr));

		if (pton_ret <= 0) {
			printf("inet_pton() error");
		}

		// tries to establish a tcp connection to the server
		if (connect(sock_fd, (struct sockaddr *) &server_address,
				sizeof(server_address)) < 0) {
			printf("connect() error");
		}

		message[0] = 1;
		message[1] = 4;
		write(sock_fd, (char *) message, 2);

		printf("Have Message Sended is  %s \n", message);
	}

	printf("This is have Message  End\n");

	return 1;
}

int SendHandShakeMessage(AnnouceRes * announceres, MetaStruct * metainfo) {

	struct sockaddr_in server_address;
	int sock_fd;
	int pton_ret;
	uint8_t message[8096];
	char readmessage[8096];
	int i;
	int j;

	printf("announceres->peernum is %d \n", announceres->peernum);

	for (i = 0; i < announceres->peernum; i++) {

		if ((announceres->LocalIP == announceres->PeerList[i].ipaddress) &&
				(announceres->LocalPort == announceres->PeerList[i].port)) {
			printf("Local ip & port continue  %d\n", i);
			continue;
		}
		else if ( announceres->PeerList[i].handshakeflag > 0)
		{
			printf("Already handshaked %d \n",i);
			continue;
		}

		if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			printf("socket() error");

		// fill out the server_address structure.
		memset((void *) &server_address, 0, sizeof(server_address));
		memset(message, 0, sizeof(message));

		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(announceres->PeerList[i].port);

		printf("send message to %d \n", announceres->PeerList[i].port);

		struct in_addr address_struct;
		address_struct.s_addr = htonl(announceres->PeerList[i].ipaddress);

		pton_ret = inet_pton(AF_INET, inet_ntoa(address_struct),
				&server_address.sin_addr);

		printf("send message to %s \n", inet_ntoa(server_address.sin_addr));

		if (pton_ret <= 0) {
			printf("inet_pton() error");
		}

		// tries to establish a tcp connection to the server
		if (connect(sock_fd, (struct sockaddr *) &server_address,
				sizeof(server_address)) < 0) {
			printf("connect() error");
		}

		printf("hand shake end 1 %s \n", message);
		message[0] = 19;
		memcpy(&message[1], "BitTorrent protocol", 19);
		memcpy(&message[20 + 8], metainfo->infohash, 20);
		printf("hand shake end 2 %s \n", message);
		for (j = 0; j < 20; j++) {
			printf("%02x", message[28 + j]);
		}

		memcpy(&message[40 + 8], metainfo->myid, 20);
		printf("\n hand shake end 3 %s \n", message);
		write(sock_fd, message, 68);
		//sleep(2);
		//read(sock_fd, readmessage, 8096);
		//sprintf("hand shake readmessage end %s \n", readmessage);
		memset(readmessage, 0, sizeof(readmessage));

		announceres->PeerList[i].handshakeflag = 1;
		printf("Hand shake end exit retrun the position int he announceres \n");
		announceres->PeerList[i].conn_fd=sock_fd;
		printf("the return num is %d \n", i);
		return i;
	}

	// all has been handsahked;
	return -1;
}

int SendBitFieldMessage(AnnouceRes * announceres, MetaStruct * metainfo) {

	struct sockaddr_in server_address;
	int sock_fd;
	int pton_ret;
	uint8_t message[8096];
	char readmessage[8096];
	int i, j;

	printf("announceres->peernum is %d \n", announceres->peernum);

	for (i = 0; i < announceres->peernum; i++) {

		if ((announceres->LocalIP == announceres->PeerList[i].ipaddress) &&
				(announceres->LocalPort == announceres->PeerList[i].port)) {
			printf("Local ip continue \n");
			continue;
		}

		if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			printf("socket() error");

		// fill out the server_address structure.
		memset((void *) &server_address, 0, sizeof(server_address));
		memset(message, 0, sizeof(message));

		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(announceres->PeerList[i].port);

		printf("send message to %d \n", announceres->PeerList[i].port);

		struct in_addr address_struct;
		address_struct.s_addr = htonl(announceres->PeerList[i].ipaddress);

		pton_ret = inet_pton(AF_INET, inet_ntoa(address_struct),
				&server_address.sin_addr);

		printf("send message to %s \n", inet_ntoa(server_address.sin_addr));

		if (pton_ret <= 0) {
			printf("inet_pton() error");
		}

		// tries to establish a tcp connection to the server
		if (connect(sock_fd, (struct sockaddr *) &server_address,
				sizeof(server_address)) < 0) {
			printf("connect() error");
		}

		printf("bit field len is  %d \n", metainfo->blocknum);
		uint32_t temp = htonl(1 + metainfo->blocknum);
		memcpy(message, &temp, 4);
		message[4] = 5;
		for (j = 0; j < metainfo->blocknum; j++) {
			message[j + 5] = 65535;
		}

		printf("bit field end 3 %s \n", message);
		write(sock_fd, message, j + 2);

		//sleep(2);
		//read(sock_fd, readmessage, 8096);
		//printf("bit field readmessage end %s \n", readmessage);
	}
}

int SendHandShakeMessage2(int sock_fd, AnnouceRes * announceres,
		MetaStruct * metainfo) {

	uint8_t message[8096];

	memset(message, 0, sizeof(message));
	int j;

	printf("hand shake end 1 %s \n", message);
	message[0] = 19;
	memcpy(&message[1], "BitTorrent protocol", 19);
	memcpy(&message[20 + 8], metainfo->infohash, 20);
	printf("hand shake end 2 %s \n", message);
	for (j = 0; j < 20; j++) {
		printf("%02x", message[28 + j]);
	}
	memcpy(&message[40 + 8], metainfo->myid, 20);
	printf("\n hand shake end 3 %s \n", message);
	write(sock_fd, message, 68);

	printf("HANDSHAKE2 SENDED \n");

	return 1;
}

int SendBitFieldMessage2(int sock_fd, AnnouceRes * announceres,
		MetaStruct * metainfo) {

	printf("Bit Filed sending message start \n");
	uint8_t message[6];
	//uint32_t str_len = 2;


	uint32_t str_len = 1 + metainfo->blocknum/9+1;

	if(metainfo->blocknum == 0)
		str_len = 1;


	str_len = htonl(str_len);

	memset(message, 0, sizeof(message));
	memcpy(message, &str_len, 4);

	message[4] = 5;
	if(metainfo->fileflag <= 0)
	{
		message[5] = 0;
		str_len = 1;
		str_len = htonl(str_len);
		memcpy(message, &str_len, 4);
		printf("No File, Bit filed was set to zero\n");
		return 1;
	}
	else if (metainfo->blocknum == 1)
	{
		printf("Block field was set by the block num \n");
		message[5] = 128;
	}
	else if (metainfo->blocknum == 2)
		{
			printf("Block field was set by the block num \n");
			message[5] = 128+64;
		}
	else if (metainfo->blocknum == 3)
		{
			printf("Block field was set by the block num \n");
			message[5] = 128+64+32;
		}
	else if (metainfo->blocknum == 4)
		{
			printf("Block field was set by the block num \n");
			message[5] = 128+64+32+16;
		}
	else if (metainfo->blocknum == 5)
		{
			printf("Block field was set by the block num \n");
			message[5] = 128+64+32+16+8;
		}
	else if (metainfo->blocknum == 6)
		{
			printf("Block field was set by the block num \n");

			message[5] = 128+64+32+16+8+4;
		}
	else if (metainfo->blocknum == 7)
		{
			printf("Block field was set by the block num128+64+32+16+8+4 \n");
			message[5] = 128+64+32+16+8+4+2;
		}
	else if (metainfo->blocknum == 8)
		{
			printf("My UB torrent does not support so large file and I am not sure you can get a corrent result and I am not sure also you can see this printf message \n");
			message[5] = 128+64+32+16+8+4+2+1;
		}



	printf("bit field send message %s \n and bit filed was set to %d \n", message, message[5]);
	write(sock_fd, message, 6);

	printf("Send Bit Filed Message ended\n return 1 for successful \n");
	return 1;
}

int sendInterestMessage2(int sock_fd, AnnouceRes * announceres,
		MetaStruct * metainfo) {
	printf("Send Interest Message start \n");

	uint8_t message[5];
	uint32_t str_len = 1;
	str_len = htonl(str_len);

	memset(message, 0, sizeof(message));
	memcpy(message, &str_len, 4);

	message[4] = 2;
	printf("Send Interest Message %s \n", message);
	write(sock_fd, message, 5);

	printf("Send Interest Message ended \n");
}

int sendRequestMessage2(int sock_fd, AnnouceRes * announceres,
		MetaStruct * metainfo, int piece_index) {
	printf("Send Request Message start \n");

	uint8_t message[17];
	uint32_t str_len = 13;
	str_len = htonl(str_len);

	memset(message, 0, sizeof(message));
	memcpy(message, &str_len, 4);

	message[4] = 6;



	uint32_t requestlen;

	requestlen = metainfo->piecelength;

	if(metainfo->filesize < metainfo->piecelength)
		requestlen = metainfo->filesize;

	if(piece_index == (metainfo->blocknum-1))
	{
		printf(" == block \n");
		requestlen = metainfo->filesize % metainfo->piecelength;
		printf("request length is send to %ld \n", requestlen);
	}



	printf("request length is %ld \n",requestlen);
	requestlen=htonl(requestlen);


	memcpy(&message[13], &requestlen, 4);
	printf("Send Request Message %s \n", message);

	piece_index=htonl(piece_index);
	memcpy(&message[5], &piece_index, 4);

	write(sock_fd, message, 17);

/*	printf("%d\n", message[0]);
	printf("%d\n", message[1]);
	printf("%d\n", message[2]);
	printf("%d\n", message[3]);
	printf("%d\n", message[4]);
	printf("%d\n", message[5]);
	printf("%d\n", message[6]);
	printf("%d\n", message[7]);
	printf("%d\n", message[8]);
	printf("%d\n", message[9]);
	printf("%d\n", message[10]);
	printf("%d\n", message[11]);
	printf("%d\n", message[12]);
	printf("%d\n", message[13]);
	printf("%d\n", message[14]);
	printf("%d\n", message[15]);
	printf("%d\n", message[16]);
	printf("%d\n", message[17]);*/

/*	requestlen=metainfo->filesize-65536;
	printf("request len is %ld \n",requestlen);
	requestlen=htonl(requestlen);
	memcpy(&message[13], &requestlen, 4);
	requestlen = htonl(65536);
	memcpy(&message[9], &requestlen, 4);
	printf("Send Request Message 2 %s \n", message);
	write(sock_fd, message, 17);
	printf("%d\n", message[0]);
	printf("%d\n", message[1]);
	printf("%d\n", message[2]);
	printf("%d\n", message[3]);
	printf("%d\n", message[4]);
	printf("%d\n", message[5]);
	printf("%d\n", message[6]);
	printf("%d\n", message[7]);
	printf("%d\n", message[8]);
	printf("%d\n", message[9]);
	printf("%d\n", message[10]);
	printf("%d\n", message[11]);
	printf("%d\n", message[12]);
	printf("%d\n", message[13]);
	printf("%d\n", message[14]);
	printf("%d\n", message[15]);
	printf("%d\n", message[16]);
	printf("%d\n", message[17]);*/
	printf("Send Request Message ended \n");
}


int SendUnchockMessage2(int sock_fd, AnnouceRes * announceres,
		MetaStruct * metainfo) {

	printf("Send Unchock Message 2 start \n");
	uint8_t message[4];
	uint32_t str_len = 1;
	str_len = htonl(str_len);

	memset(message, 0, sizeof(message));
	memcpy(message, &str_len, 4);

	message[4] = 1;
	printf("Unchock message %s \n", message);
	write(sock_fd, message, 5);

	printf("Send Unchock Message ended \n");
}
