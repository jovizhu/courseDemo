
#ifndef _CHATTYME_H_
#define _CHATTYME_H_

#define LOCAL_UDP_PORT 8316
#define LOCAL_TCP_PORT 8315
#define LISTENQ      1024     /* 2nd argument to listen()          */
#define MY_FD_SETSIZE 5
#define MAXLINE 4096
#define DEFAULT_DNS_PORT 53
#define DEFAULT_DNS_IP   "8.8.8.8"
#define DEFAULT_LOCAL_IP "127.0.0.1"
#define DEFAULT_LOCAL_NAME "localhost"
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

#define	min(a,b)     ((a) < (b) ? (a) : (b))
#define	max(a,b)     ((a) > (b) ? (a) : (b))

struct ConnectObject
{
	in_port_t  localport;
	in_port_t  remotport;
	char hostname[MAXLINE];
	char ipaddress[MAXLINE];
	};

struct TCPMessageFormat
{
	int32_t length;
	int32_t pid;
	char  *message;
	};


struct UDPmessageFormat
{
	int32_t pid;
	char  message[MAXLINE];
	};

#endif
