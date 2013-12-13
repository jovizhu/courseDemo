
#ifndef _NETWORK_UTIL_H
#define _NETWORK_UTIL_H

typedef	void  Sigfunc(int);   /* for signal handlers               */
Sigfunc* simpler_sigaction(int, Sigfunc*);
void     sig_child_handler(int);

#endif /* _NETWORK_UTIL_H */
