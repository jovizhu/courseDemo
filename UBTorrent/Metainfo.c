
#include "Metainfo.h"


void GetMetaFileInfo(be_node *node, ssize_t indent, MetaStruct * Metainfo);

void GetMetainfo(char * filename, MetaStruct * Metainfo) {
	FILE * metafile;
	int k;
	metafile = fopen(filename, "r");
	strcpy(Metainfo->torrentfilename, filename);
	char ch[8192];
	char seed[8192];
	char * ptr;
	struct hostent* host_info = NULL;
	struct in_addr* address;


	printf("aa\n");

	char temp[255];
	if ((gethostname(temp, sizeof(temp))) == -1) {
		perror("gethostname");
		exit(1);
	}

	host_info = gethostbyname(temp);

	if (host_info) {
		address = (struct in_addr*) host_info->h_addr_list[0];
		memset(temp, NULL, sizeof(temp));
		strcpy(temp, inet_ntoa(*address));
	}

	strncpy(Metainfo->hostip, temp, strlen(temp) + 1);

	printf("ip address is %s \n, length is %d", Metainfo->hostip, strlen(temp));

	memset(temp, NULL, sizeof(temp));
	sprintf(temp, "%d", Metainfo->port);
	printf("String of integer %d (radix 10): %s\n", Metainfo->port, temp);
	strcat(temp, Metainfo->hostip);
	printf("%s \n", temp);
	CalSHAHash(temp, Metainfo->myid);

	printf("my id\n");
	for (k = 0; k < 20; k++) {
		printf("%02x", Metainfo->myid[k]);
	}

	strcpy(Metainfo->torrentfilename, filename);
	while (fgets(ch, 8192, metafile)) {
		// Capital character
		be_node *n = be_decoden(ch,sizeof(ch));
		strcpy(seed, ch);
		seed[strlen(ch) - 1] = '\0';
		ptr = seed;

		printf("%d, \n", strlen(ch) - 1);
		for (k = 0; k < strlen(ch) - 1; k++) {
			//printf("%d\  %d  \n", k, seed[k]);
			if ((seed[k] == 58) && (seed[k + 1] == 105) && (seed[k + 2] == 110)
					&& (seed[k + 3] == 102) && (seed[k + 4] == 111) && (seed[k
					+ 5] == 100)) {			// :infod
				ptr = ptr + k + 5;
				//printf("%s \n", ptr);
			}

		}
		printf("%s ", ptr);

		CalSHAHash(ptr, Metainfo->infohash);

		printf("Menta Info Hash\n");
		for (k = 0; k < 20; k++) {
			printf("%02x", Metainfo->infohash[k]);
		}

		printf("DECODING: %s\n", ch);
		if (n) {
			GetMetaFileInfo(n, 0, Metainfo);
			be_free(n);
		} else
			printf("\tparsing failed!\n");

	}

	Metainfo->fileflag = open ( Metainfo->filename, O_RDONLY);
	printf("File Flag is %d \n", Metainfo->fileflag);
}

void GetMetaFileInfo(be_node *node, ssize_t indent, MetaStruct * Metainfo) {
	size_t i, j, k;
	char * pEnd;
	long int li;
	char temp[1024];

	indent = abs(indent);

	switch (node->type) {
	case BE_DICT:
		for (i = 0; node->val.d[i].val; ++i) {
			//printf("%s : ", node->val.d[i].key);
			if (strcmp(node->val.d[i].key, "announce") == 0) {
				strcpy(Metainfo->announceURL, node->val.d[i].val->val.s);
				//printf("Announde URL: %s \n", Metainfo->announceURL);
			} else if (strcmp(node->val.d[i].key, "creation date") == 0) {
				Metainfo->creatdate = node->val.d[i].val->val.i;
				printf("Create Date: %lli \n", Metainfo->creatdate);
				//printf("Announde URL %s\n");
			} else if (strcmp(node->val.d[i].key, "name") == 0) {
				strcpy(Metainfo->filename, node->val.d[i].val->val.s);
				//printf("File Name: %s \n", Metainfo->filename);
			} else if (strcmp(node->val.d[i].key, "piece length") == 0) {
				Metainfo->piecelength = node->val.d[i].val->val.i;
				//Metainfo->filesize = Metainfo->piecelength * 3 + 221184;
				printf("Piece Length: %lli \n", Metainfo->piecelength);
				//printf("File Size: %lli \n", Metainfo->filesize);
				//printf("Announde URL %s\n");
			} else if (strcmp(node->val.d[i].key, "pieces") == 0) {
				for (j = 0; j < (Metainfo->filesize/Metainfo->piecelength)+1; j++) {
					memset(Metainfo->pieceshash[j], 0, 20);
					strncpy(Metainfo->pieceshash[j], node->val.d[i].val->val.s+(j*20),
							20);
					//printf("hash pieces%s \n",Metainfo->pieceshash[j]);
					printf(" Pices Hash: \n ");
					printf("%d  :", j);
					for (k = 0; k < 20; k++) {
						printf("%02x", Metainfo->pieceshash[j][k]);
					}
					printf("\n");

				}
				Metainfo->blocknum=j;
			} else if (strcmp(node->val.d[i].key, "length") == 0) {
				Metainfo->filesize = node->val.d[i].val->val.i;
				printf("File Size: %lli \n", Metainfo->filesize);
			}
			GetMetaFileInfo(node->val.d[i].val, -(indent + 1), Metainfo);
		}
		break;
	default:
		break;
	}
}


void Print_MetaInfo(MetaStruct * Metainfo)
{
	size_t i, j;
	int piece_num;
	piece_num = Metainfo->filesize / Metainfo->piecelength + 1;

	printf("my IP/port    : %s/%d\n", Metainfo->hostip, Metainfo->port);
	printf("my ID         : ");
	for (i = 0; i < 20; i++) {
		printf("%02x", Metainfo->myid[i]);
	}
	printf("\n");
	printf("metainfo file : %s\n", Metainfo->torrentfilename);
	printf("info hash     : ");
	for (i = 0; i < 20; i++) {
		printf("%02x", Metainfo->infohash[i]);
	}
	printf("\n");
	printf("file name     : %s\n", Metainfo->filename);
	printf("piece length  : %lli\n", Metainfo->piecelength);
	printf("file size     : %lli\n", Metainfo->filesize);
	printf("announce URL  : %s\n", Metainfo->announceURL);
	printf("pieces' hashes: \n");
	printf("blocknum = %d\n", Metainfo->blocknum);
	for (j = 0; j < Metainfo->blocknum; j++) {
		printf("  %d  ", j);
		for (i = 0; i < 20; i++) {
			printf("%02x", Metainfo->pieceshash[j][i]);
		}
		printf("\n");
	}
	printf("\n");

}
