#include "Status.h"


int Print_Status(MetaStruct * Metainfo)
{
	printf("Enter Print_Status\n");

	struct stat *buf;
	size_t i;
	int flag;

	buf = malloc(sizeof(struct stat));
	memset(buf, 0, sizeof(buf));

	printf("filename = %s\n", Metainfo->filename);
	printf("filesize = %lld\n", Metainfo->filesize);
	printf("blocknum = %d\n", Metainfo->blocknum);

/*
	if (stat(Metainfo->filename, buf) < 0) {
		printf("stat() error\n");
		exit(1);
	}
*/

	flag = stat(Metainfo->filename, buf);
	printf("stat ok\n");

	if (flag < 0 || buf->st_size == 0) {
		printf(" Uploaded | Downloaded | Left    | My bit field    \n");
		printf("---------------------------------------------------\n");
		printf(" 0          | 0        | %7lld | ", Metainfo->filesize);
		for (i = 1; i <= Metainfo->blocknum; i++)
			printf("0");
		printf("\n");
		return 0;
	}
	else if (flag == 0 && buf->st_size == Metainfo->filesize) {
		printf(" Uploaded | Downloaded | Left    | My bit field    \n");
		printf("---------------------------------------------------\n");
		printf(" %10lld | 0        | 0       | ", Metainfo->filesize);
		for (i = 1; i <= Metainfo->blocknum; i++)
			printf("1");
		printf("\n");
		return 0;
	}
	else {
		return -1;
	}

}
