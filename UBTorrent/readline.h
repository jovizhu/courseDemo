#ifndef READLINE_H_
#define READLINE_H_

#include "NetworkUtil.h"

ssize_t readline(int fd, void *vptr, size_t maxlen);
static ssize_t my_read(int fd, char* ptr);

#endif /* READLINE_H_ */
