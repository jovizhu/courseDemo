#include "network_util.h"
#include "ChattyMe.h"
#include  <sys/socket.h>  /* basic socket definitions               */
#include  <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include  <netdb.h>       /* name and address conversion            */
#include  <arpa/inet.h>   /* inet_pton/ntop                         */
/*
 * ----------------------------------------------------------------------------
 * simpler_sigaction:
 *   appropriately calls POSIX's sigaction, except for SIGALARM, we try to
 *     restart any interrupted system calls after any other signals
 *   'signo' is the signal number
 *   'func' is the signal handler
 *   SIG_ERR is returned if the call to sigaction fails
 * ----------------------------------------------------------------------------
 */
Sigfunc* simpler_sigaction(int signo, Sigfunc *func)
{
  struct sigaction act, oact;

  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  if (signo == SIGALRM) {
#ifdef  SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;  /* SunOS 4.x : only in the case of SIGALARM
                                    * we do not want to restart the sys. call */
#endif
  } else {
#ifdef  SA_RESTART
    act.sa_flags |= SA_RESTART;    /* SVR4, 44BSD : restart interrupted system
                                    * calls */
#endif
  }

  if (sigaction(signo, &act, &oact) < 0) {
    return(SIG_ERR);
  }
  return(oact.sa_handler);
}

/*
 * ----------------------------------------------------------------------------
 * sig_child_handler:
 *   we do not want zombies, so try to wait for all children to finish whenever
 *     a SIGCHLD is received
 * ----------------------------------------------------------------------------
 */
void sig_child_handler(int signo)
{
  pid_t pid;
  int   stat;

  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
    printf("Child process %d terminated\n", pid);
  }
}
