#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MAXLINE 8000 
#define MAXFILENUM 12
#define LISTENQ 5

// Using for the Lsserver and lsclient

// Using for the dserver
#define FILEPATH "./"

typedef struct block
{
		uint8_t blocka[256];
} block;

// Define File Object for the File Server
typedef struct FileObject
{
	char Filename[9];
	short Filesize;
	short BlockToUse;// File size is less than 2000, so the max block use will no more than 4
} FileObject;

