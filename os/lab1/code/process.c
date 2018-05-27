#include <stdio.h>
#include <signal.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <unistd.h>
#include<string.h>
#include <stdlib.h>
int main()
{
   int cpid;
   int ppid;
   char buf[256];
   signal(SIGTTOU, SIG_IGN);
   signal(SIGTTIN, SIG_IGN);
   ppid=getpid();
   cpid=fork();
   if (cpid<0)
   exit(-1);
   if(!cpid)
   {
      setpgid(0,0);
      tcsetpgrp(0,getpid());
      execl("/bin/vi", "vi", NULL);
      exit(-1);
   }
   setpgid(cpid,cpid);
   tcsetpgrp(0,cpid);
   waitpid(cpid,NULL,0);
   tcsetpgrp(0,ppid);
   while(1)
   {
      printf("ECHO your inputs...\n");
      memset(buf, 0, 256);
      fgets(buf, 256, stdin);
      puts("ECHO: ");
      puts(buf);
   }
}
