#include "readline.h"
/*
 * ----------------------------------------------------------------------------
 * readline:
 *   read upto '\n' or maxlen bytes from 'fd' into 'vptr'
 *   returns number of bytes read or -1 on error
 *   due to the call to my_read, this function is NOT thread safe
 *   furthermore, it must not be called interleavingly with readn because
 *   my_read is a buffered read
 * ----------------------------------------------------------------------------
 */

ssize_t readline(int fd, void *vptr, size_t maxlen) {
	int n, rc;
	char c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ((rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n') {
				break; /* newline is stored, like fgets() */
			}
		} else if (rc == 0) {
			if (n == 1) {
				return (0); /* EOF, no data read       */
			} else {
				break; /* EOF, some data was read */
			}
		} else {
			return (-1); /* error, errno set by read()  */
		}
	}

	*ptr = 0; /* null terminate like fgets() */
	return (n);
}


/*
 * ----------------------------------------------------------------------------
 * my_read:
 *   internal function, reads upto MAXLINE characters at a time, and then
 *     return them one at a time.
 *   this is much more efficient than 'read'ing one byte at a time, but the
 *     price is that it's not "reentrant" or "thread-safe", due to the use
 *     of static variables
 * ----------------------------------------------------------------------------
 */
static ssize_t my_read(int fd, char* ptr)
{
  static int   read_count = 0;
  static char* read_ptr;
  static char  read_buf[8096];
  int          got_signal;

  got_signal = 0;
  if (read_count <= 0) {
  again:
    if ( (read_count = read(fd, read_buf, sizeof(read_buf))) < 0) {
      if (errno == EINTR) {
	goto again;  /* Dijkstra hates this, but he's not reading our code */
      } else {
	return(-1);
      }
    } else if (read_count == 0) {
      return(0);
    }
    read_ptr = read_buf;
  }

  read_count--;
  *ptr = *read_ptr++;
  return(1);
}
