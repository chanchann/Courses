#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc,char*argv[])
{
  int pid;
  char *prog_argv[4];
  prog_argv[0]="/bin/ls";
  prog_argv[1]="-lh";
  prog_argv[2]="/";
  prog_argv[3]=NULL;
  if ((pid=fork())<0)
  {
     perror("Fork failed");
     printf("error:%s\n",strerror(errno));
     exit(0);
  }
  if (!pid)
 {
    execvp(prog_argv[0],prog_argv);
 }
 if (pid)
 {
   waitpid(pid,NULL,0);
 }
return 0;
}
