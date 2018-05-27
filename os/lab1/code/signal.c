#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <unistd.h>
#include <stdlib.h>
void ChildHandler (int sig, siginfo_t* sip, void* notused)
{
   int status;
   printf("The process generating the signal is PID: %d\n",sip->si_pid);
   fflush(stdout);
   status=0;
   if(sip->si_pid==waitpid(sip->si_pid, &status, WNOHANG))
   {
        if(WIFEXITED(status)||WTERMSIG(status))
            printf("The child is gone!!!!!\n");
        else
            printf("Uninteresting\n");
   }
   else
   {
      printf("Uninteresting\n");
   }
};

int main()
{
   struct sigaction action;
   action.sa_sigaction=ChildHandler;
   sigfillset(&action.sa_mask);
   action.sa_flags = SA_SIGINFO;
   sigaction(SIGCHLD,&action,NULL);
   int pid;
   pid = fork();
   while(1)
   {
      if (pid)
         printf("PID(parent): %d\n",getpid());
      else
         printf("PID(child): %d\n",getpid());
      sleep(1);
   }
   return 0;
}
