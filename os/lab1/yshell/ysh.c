
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<stdlib.h>
#include<signal.h>

#define M_PATH 256    //max path length  
#define M_LINE 256    //max line length  
#define M_CMD 10       //max command number  
#define M_HIS 50

typedef struct job_node   //job node   
{
    pid_t pid;
    char cmd[100];
    char state[10];
    struct job_node* next;
}job_node;

char buf[M_LINE],sav_cmd[M_LINE];    //buffer,save command line buffer  
char *fir_cmd,*pipe_cmd[M_CMD];   //first command,pipe commands  
char *fir_vec[M_LINE],*pipe_vec[M_CMD][M_LINE]; //first command vector,pipe command vectors  
char *infile,*outfile;           //input file,output file  

char history[M_HIS][M_LINE];        //history command array  
int his_start,his_end;              //history command start index,history command end index  

int pfd[M_CMD][2];                   //plural pipes' file descriptors  
int pipe_num;                    //pipe command number  
int is_bg;                       //is background command  
int f_pid;                       //forge command pid, 0 is none  
int sig_z;                       //1 SIGCHID signal is by "ctrl-z",0 not     

job_node *head_job,*tail_job;     //job next's head and end  
int job_num;                      //job numbers  

int main()
{
    //*********************function statement***************************  
    void init_environ();       //init variants  
    int analyse();           //anlalyse commandline  
    void exe_fir_cmd();      //execute first command  
    void exe_pipe_cmd();      //execute pipe commands  
    void end_job();           //end job  
    void ctrlz_handler();     //ctrl-z handler function    
    void ctrlc_handler();     //ctrl-c handler function   
    void childf_handler(int sig,siginfo_t *sip);     //SIGCH  
    char path[M_PATH];

    //process STGTSTP signal by ctrl-z    
    struct sigaction ctrlz_act;
    ctrlz_act.sa_handler =ctrlz_handler;     //point to our function "ctrlz_handler()"     
    sigemptyset(&ctrlz_act.sa_mask);   //clear mask  
    ctrlz_act.sa_flags=0;               //no special flags  
    sigaction(SIGTSTP,&ctrlz_act,NULL);

    //process STGINT signal by ctrl-c  
    struct sigaction ctrlc_act;
    ctrlc_act.sa_handler=ctrlc_handler;
    sigemptyset(&ctrlc_act.sa_mask);
    ctrlc_act.sa_flags=0;
    sigaction(SIGINT,&ctrlc_act,NULL);

    //process STGCHLD signal by finished child process  
    struct sigaction child_act;
    child_act.sa_handler=childf_handler;
    sigemptyset(&child_act.sa_mask);
    child_act.sa_flags=SA_SIGINFO;
    sigaction(SIGCHLD,&child_act,NULL);

    init_environ();
    for(;;)
    {
        if(getcwd(path,M_PATH)==NULL)
            fprintf(stderr,"**ERROR:%s",strerror(errno));
        printf("%s=>",path);
        if(analyse()==1)
        {
            exe_fir_cmd(infile);
            if(pipe_num!=0)
                exe_pipe_cmd(outfile);
        }
    }
    return 0;
}

//init variants  
void init_environ()
{
    int i,j;

    his_start=his_end=0;
    pipe_num=0;
    infile=outfile=NULL;
    head_job=tail_job=NULL;
    job_num=0;
    sig_z=0;
    fir_cmd=fir_vec[0]=NULL;
    for(i=0;i<M_CMD;i++)
    {
        pipe_cmd[i]=NULL;
        for(j=0;j<M_LINE;j++)
        {
            buf[j]=sav_cmd[j]='\0';
            fir_vec[j]=NULL;
            pipe_vec[i][j]=NULL;
        }
    }
}

//analyse commmandline. legal return 1;illegal return 0  
int analyse()
{
    int n,m;
    char *pipe_buf,*p;//pipe command bufer  

    if(fgets(buf,sizeof buf,stdin)==NULL)   //when user press like ctr-z eg.   
    {
        printf("\n");
        return 0;
    }
    if(strcmp(buf,"\n")==0)   //no input  
        return 0;
    buf[strlen(buf)-1]='\0';
    strcpy(sav_cmd,buf);   //save command  

    //add history  
    his_end=(his_end+1)%M_HIS;
    if(his_end==his_start)
        his_start=(his_start+1)%M_HIS;
    strcpy(history[his_end],buf);
    //********is background command  
    is_bg=0;
    if((p=strstr(buf," &"))!=NULL)
    {
        is_bg=1;*p='\0';
        for(p=p+2;is_bg && *p!='\0';p++)
        {
            if(*p!=' ' && *p!='\t')
            {
                printf("**ERROR: command error!\n");
                return 0;
            }
        }
    }

    //output file name  
    outfile=strstr(buf,">");
    if(outfile)
    {
        *outfile='\0';
        outfile=strtok(outfile+1," \t");
    }

    //pipe command  
    pipe_buf=strstr(buf,"|");
    m=0;
    while(pipe_buf!=NULL)
    {
        *pipe_buf='\0'; p=pipe_buf;
        p=strstr(p+1,"|");
        if(p!=NULL) *p='\0';
        n=0;
        pipe_cmd[m]=strtok(pipe_buf+1," \t");
        pipe_vec[m][n++]=pipe_cmd[m];
        while((pipe_vec[m][n++]=strtok(NULL," \t")))
            ;
        pipe_buf=p;
        m++;
    }
    pipe_num=m;

    //input file name  
    infile=strstr(buf,"<");
    if(infile)
    {
        *infile='\0';
        infile=strtok(infile+1," \t");
    }

    //first commandline  
    fir_cmd=strtok(buf," \t");
    if(fir_cmd==NULL)   //no first command  
    {
        printf("ERROR: command error!\n");
        return 0;
    }
    else
    {
        n=0;
        fir_vec[n++]=fir_cmd;
        while((fir_vec[n++]=strtok(NULL," \t")))
            ;
    }

    return 1;
}

//input redirect. successed return 1,failed return 0;  
int in_redirect()
{
    int fdi=-1;

    if((fdi=open(infile,O_RDONLY))!=-1)
    {
        dup2(fdi,0);
        close(fdi);
        return 1;
    }
    else
    {
        fprintf(stderr,"**ERROR:%s\n",strerror(errno));
        return 0;
    }
}

//output redirect. successed return 1,failed return 0;  
int out_redirect()
{
    int fdo=-1;

    if((fdo=open(outfile,O_CREAT|O_WRONLY,0666))!=-1)
    {
        dup2(fdo,1);
        close(fdo);
        return 1;
    }
    else
    {
        fprintf(stderr,"**ERROR:%s\n",strerror(errno));
        return 0;
    }
}

//excute "exit" command,logout  
void exit_cmd()
{
    printf("Logout!\n");
    exit(1);
}

//excute "cd" command,change directory  
void cd_cmd()
{
    if(chdir(fir_vec[1])==-1)
        fprintf(stderr,"**ERROR:%s\n",strerror(errno));
}
//excute "history" command,show history commands  
void history_cmd()
{
    int i,j;
    if(his_start==his_end)
        return;
    if(his_start<his_end)
    {
        for(i=his_start,j=0;i<=his_end;i++,j++)
            printf("%d\t%s\n",j,history[i]);
    }
    else
    {
        for(i=his_start,j=0;i<M_HIS;i++,j++)
            printf("%d\t%s\n",j,history[i]);
        for(i=0;i<=his_end;i++,j++)
            printf("%d\t%s\n",j,history[i]);
    }
}
//execute "jobs" command,show background jobs  
void jobs_cmd()
{
    job_node *p;
    int i=1;

    for(p=head_job;p!=NULL;p=p->next,i++)
        printf("[%d] %d %s\t%s\n",i,p->pid,p->state,p->cmd);
}

//excute "bg" command,make suspended job run in background  
void bg_cmd(int job_num)
{
    job_node *p;
    int i;

    for(i=1,p=head_job;i<job_num && p!=NULL;i++)
        p=p->next;
    if(p!=NULL)            //find job  
    {
        printf("\n");
        kill(p->pid,SIGCONT);
        strcpy(p->state,"Running");
        printf("%s\t&\n",p->cmd);
    }
    else
        printf("**ERROR: no such job\n");
}


//excute "fg" command, make suspended job or backgound job run in forge  
void fg_cmd(int job_num)
{
    job_node *p=head_job;
    int i;

    for(i=1;i<job_num && p!=NULL;i++)
        p=p->next;
    if(p!=NULL)           //find job  
    {
        printf("%s\n",p->cmd);
        strcpy(p->state,"Running");
        f_pid=p->pid;
        kill(p->pid,SIGCONT);
        waitpid(f_pid,NULL,0);
    }
    else
        printf("**ERROR: no such job\n");
}
//execute first command.   
void exe_fir_cmd()
{
    int pid;  //fork() return pid  

    if(strcmp(fir_cmd,"exit")==0)
    {
        exit_cmd();
        return ;
    }
    if(strcmp(fir_cmd,"cd")==0)
    {
        cd_cmd();
        return ;
    }
    if(strcmp(fir_cmd,"history")==0)
    {
        history_cmd();
        return ;
    }
    if(strcmp(fir_cmd,"jobs")==0)
    {
        jobs_cmd();
        return ;
    }
    if(strcmp(fir_cmd,"fg")==0)
    {
        int i;
        if(((strcmp(fir_vec[1],"%")==0) && (i=atoi(fir_vec[2]))!=0)
           || (*fir_vec[1]=='%' && (i=atoi(fir_vec[1]+1))!=0))
            fg_cmd(i);
        return ;
    }
    if(strcmp(fir_cmd,"bg")==0)
    {
        int i;
        if(((strcmp(fir_vec[1],"%")==0) && (i=atoi(fir_vec[2]))!=0)
           || (*fir_vec[1]=='%' && (i=atoi(fir_vec[1]+1))!=0))
            bg_cmd(i);
        return ;
    }
    f_pid=0;              //init f_pid,no forge command is running  
    if(pipe_num!=0)     pipe(pfd[0]);
    if((pid=fork())==0)
    {
        //inside command  
        if(infile!=NULL)  //input redirect  
            in_redirect();
        if(pipe_num==0 && outfile!=NULL)  //output redirect and no pipe  
            out_redirect();
        if(pipe_num!=0)   //has pipes  
        {
            dup2(pfd[0][1],1);
            close(pfd[0][1]);
            close(pfd[0][0]);
        }
        execvp(fir_cmd,fir_vec);
        fprintf(stderr,"**ERROR: %s\n",strerror(errno));
        exit(1);
    }
    f_pid=pid;
    if(is_bg==0)
        waitpid(pid,NULL,0);
}

//execute pipe commands  
void exe_pipe_cmd()
{
    int i,pid;

    for(i=0;i<pipe_num-1;i++)
    {
        pipe(pfd[i+1]);     //pipe for next command  
        if(fork()==0)
        {
            dup2(pfd[i][0],0);
            close(pfd[i][0]);
            close(pfd[i][1]);
            dup2(pfd[i+1][1],1);
            close(pfd[i+1][1]);
            close(pfd[i+1][0]);
            execvp(pipe_cmd[i],pipe_vec[i]);
            fprintf(stderr,"**ERROR: %s\n",strerror(errno));
            exit(1);
        }
        close(pfd[i][0]);
        close(pfd[i][1]);
    }
    //execute last pipe command  
    if((pid=fork())==0)
    {
        dup2(pfd[i][0],0);
        close(pfd[i][1]);
        close(pfd[i][0]);
        if(outfile!=NULL)                     //output redirect  
            out_redirect();
        execvp(pipe_cmd[i],pipe_vec[i]);
        fprintf(stderr,"**ERROR: %s\n",strerror(errno));
        exit(1);
    }
    close(pfd[i][0]);
    close(pfd[i][1]);
    if(is_bg==0)
        waitpid(pid,NULL,0);
}

//"ctrl-z" handler,SIGTSTP signal,make forge process suspend.   
void ctrlz_handler()
{
    job_node *p;

    if(f_pid==0)
        return ;
    sig_z=1;
    p=(job_node *)malloc(sizeof(job_node));
    p->pid=f_pid;
    strcpy(p->cmd,sav_cmd);
    strcpy(p->state,"Stopped");
    p->next=NULL;
    if(head_job==NULL)
        head_job=tail_job=p;
    else
    {
        tail_job->next=p;

        tail_job=p;
    }
    job_num++;
    kill(f_pid,SIGTSTP);
    printf("\n[%d]+\t%s\t\t\t%s\n",job_num,tail_job->state,tail_job->cmd);

    f_pid=0;
}

//"ctrl-c" handler,SIGINT signal,kill forge process .   
void ctrlc_handler()
{
    if(f_pid==0)
        return ;
    kill(f_pid,SIGINT);
    printf("\n");
    f_pid=0;
}

//child process finish handler,SIGCHLD signal ,means child finished  
void childf_handler(int sig,siginfo_t *sip)
{
    job_node *p,*q;
    int pid=sip->si_pid;    //get sender's pid  

    if(head_job==NULL||sig_z==1)
    {
        sig_z=0;
        return ;
    }
    for(p=q=head_job;p!=NULL;p=p->next)
    {
        if(p->pid==pid)
            break;

        q=p;
    }
    if(p==NULL)
        return ;
    if(p==head_job)
        head_job=head_job->next;
    else
    {
        if(p==tail_job)
        {
            tail_job=q;
            tail_job->next=NULL;
        }
        else

            q->next=p->next;
    }
    printf("\n[%d]+\t%s\t\t\tDone\n",job_num,tail_job->cmd);
    free(p);
}  
