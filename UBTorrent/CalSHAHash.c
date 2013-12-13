#include "CalSHAHash.h"


void CalSHAHash(char * seed, uint8_t * shahash)
{
	  size_t       i;
	  int         err;
	  SHA1Context sha;
	  uint8_t     message_digest[20]; /* 160-bit SHA1 hash value */

	  err = SHA1Reset(&sha);
	  if (err) printf("SHA1Reset error %d\n", err);

	  /* 'seed' is the string we want to compute the hash of */
	  err = SHA1Input(&sha, (const unsigned char *) seed, strlen(seed));

	  if (err) printf("SHA1Input Error %d.\n", err );

	  err = SHA1Result(&sha, message_digest);
	  if (err) {
	    printf("SHA1Result Error %d, could not compute message digest.\n", err);
	  }

	  printf("\n");
	  for (i=0;i<20;i++)
	  {
		  printf("%02x", message_digest[i]);
	  }

	  memcpy(shahash,message_digest,20);

	}
