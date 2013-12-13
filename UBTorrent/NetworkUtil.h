/*
 * NetworkUtil.h
 *
 *  Created on: Dec 9, 2010
 *      Author: jovi
 */

#ifndef NETWORKUTIL_H_
#define NETWORKUTIL_H_


#include <stdio.h>
#include  <sys/socket.h>  /* basic socket definitions               */
#include  <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include  <netdb.h>       /* name and address conversion            */
#include  <arpa/inet.h>   /* inet_pton/ntop                         */
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>

#define DEFAULT_PORT 2233
#define LISTENQ 15
#define MAXLINE 8000
#define MY_FD_SETSIZE 10
#define MAXPEERNUM 20

#define	min(a,b)     ((a) < (b) ? (a) : (b))
#define	max(a,b)     ((a) > (b) ? (a) : (b))

#define MAXLENGTH 8096

typedef struct MetaStruct
		{
			char  torrentfilename[MAXLENGTH];
			char hostip[255];
			uint16_t port;
			uint8_t  myid[20];
			uint8_t  infohash[20];
			char  filename[MAXLENGTH];
			long long piecelength;
			long long filesize;
			char  announceURL[MAXLENGTH];
			uint8_t pieceshash[MAXLENGTH][20];
			long long creatdate;
			int fileflag;
			int blocknum;

		} MetaStruct;



typedef struct PeerStruct {
	uint32_t ipaddress;
	uint16_t port;
	int handshakeflag;
	uint8_t hashinfo[20];
	int conn_fd;
} PeerStruct;


typedef struct AnnounceRes {
	long long complete;
	long long incomplete;
	long long downloaded;
	long long interval;
	long long mininterval;
	struct PeerStruct PeerList[8096];
	uint32_t LocalIP;
	uint16_t LocalPort;
	int peernum;
} AnnouceRes;

typedef struct PeerObject {

	uint16_t remotport;
	char ipaddress[8096];
	uint8_t peerid[20];
	uint8_t bitfiled;
	uint8_t MsgBitFieldFlag;
	uint8_t chockflag;
	uint8_t MsgChockFlag;
	uint8_t interestflag;
	uint8_t MsgInterestFlag;
	uint8_t MsgHandShakeFlag;
	uint32_t downloaded;
	uint32_t uploaded;
	int time_up;
	int time_down;
	struct timeval up;
	struct timeval down;
} PeerObject;

int AddtoPeerObjectList(struct PeerObject *, char *, int);


#define DEFAULT_DNS_IP "8.8.8.8"
#define DEFAULT_DNS_PORT 53


#endif /* NETWORKUTIL_H_ */
