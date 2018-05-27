#include <stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
//#include <error.h>

int hasPrefix(char const *, char const *);
int cd(char *pth);

#define BUFFERSIZE 200
int main(){

    char buffer[BUFFERSIZE];
    char *prompt = "OS";
    char *a = ">";

    char *tok;
    tok = strtok (buffer," ");


    while(buffer != NULL){
        bzero(buffer, BUFFERSIZE);
        printf("%s%s",prompt,a);
        fgets(buffer, BUFFERSIZE, stdin);
        if(hasPrefix(buffer,"cd") == 0){
            tok = strchr(buffer,' '); //use something more powerful
            if(tok) {
                char *tempTok = tok + 1;
                tok = tempTok;
                char *locationOfNewLine = strchr(tok, '\n');
                if(locationOfNewLine) {
                    *locationOfNewLine = '\0';
                }
                cd(tok);
            }
        }else{
            system("ls"); //for testing the CWD/PWD
        }
    }
    return 0;
}

int hasPrefix(char const *p, char const *q)
{
    int i = 0;
    for(i = 0;q[i];i++)
    {
        if(p[i] != q[i])
            return -1;
    }
    return 0;
}

int cd(char *pth){
    char path[BUFFERSIZE];
    strcpy(path,pth);

    char cwd[BUFFERSIZE];
    if(pth[0] != '/')
    {// true for the dir in cwd
        getcwd(cwd,sizeof(cwd));
        strcat(cwd,"/");
        strcat(cwd,path);
        chdir(cwd);
    }else{//true for dir w.r.t. /
        chdir(pth);
    }

    return 0;
}
