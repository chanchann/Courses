#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include "head.h"

int globalfd;

int allocjid(){
	return ++jobid;
}

void do_enq(struct jobinfo *newjob, struct jobcmd enqcmd){
	//为每个作业创建一个进程，将其状态置为READY，然后放入就绪队列中
	//在标准输出上打印出信息，并给出jid，若出错，则打印错误信息
	struct	waitqueue *newnode, *p;
	int	i=0, pid;
	char *offset, *argvec, *q;
	char **arglist;
	sigset_t zeromask;
	sigemptyset(&zeromask);
	
	newjob = (struct jobinfo *)malloc(sizeof(struct jobinfo));
	newjob->jid = allocjid();
	newjob->defpri = enqcmd.defpri;
	newjob->curpri = enqcmd.defpri;		
	newjob->ownerid = enqcmd.owner;
	newjob->state = READY;
	newjob->create_time = time(NULL);
	newjob->wait_time = 0;	
	newjob->run_time = 0;
	arglist = (char**)malloc(sizeof(char*)*(enqcmd.argnum+1));
	newjob->cmdarg = arglist;	
	offset = enqcmd.data;
	argvec = enqcmd.data;
	while (i < enqcmd.argnum) {  
		if (*offset == ':') {
			*offset++ = '\0';
			q = (char*)malloc(offset - argvec);
			strcpy(q,argvec);
			arglist[i++] = q;
			argvec = offset;
		} 
		else
			offset++;
	}
	arglist[i] = NULL;
	
	printf("enqcmd argnum %d\n",enqcmd.argnum);
	for (i = 0; i < enqcmd.argnum; i++)
		printf("parse enqcmd:%s\n",arglist[i]);
	newnode = (struct waitqueue*)malloc(sizeof(struct waitqueue));
	newnode->next = NULL;
	newnode->job = newjob;	
	if (head) {
		for (p = head; p->next != NULL; p = p->next);
		p->next = newnode;
	}
	else
		head = newnode;
	if ((pid = fork()) < 0) //创建一个进程
		printf("enq fork failed");
	if (pid == 0) { //子进程
		newjob->pid = getpid();
		raise(SIGSTOP); //在子进程中使用该函数发出SIGSTOP信号，使子进程暂停
		printf("begin running\n");
		for (i = 0; arglist[i] != NULL; i++)
			printf("arglist %s\n",arglist[i]);
			dup2(globalfd,1); //复制文件描述符
			if (execv(arglist[0],arglist) < 0)
				printf("exec failed\n");
			exit(1);
	}
	else { //父进程
		newjob->pid = pid;
		printf("\nnew job: jid=%d, pid=%d\n", newjob->jid, newjob->pid);
	}
}

void do_deq(struct jobcmd deqcmd){
	//使该作业出队，然后清除相关的数据结构
	//如果该作业当前正在运行，则发信号给它，使它停止
	int deqid,i;
	struct waitqueue *p,*prev,*select,*selectprev;
	deqid = atoi(deqcmd.data);
	
	printf("deq jid %d\n",deqid);
	
	if (current && current->job->jid == deqid) {	
		printf("terminate job: %d\n", current->job->jid);
		kill(SIGTERM, current->job->pid); //将当前作业杀死
		for (i = 0; (current->job->cmdarg)[i] != NULL; i++) {
			free((current->job->cmdarg)[i]);
			(current->job->cmdarg)[i] = NULL;
		}
		free(current->job->cmdarg);
		free(current->job);
		free(current);
		current = NULL;
	} 
	else {
		select = NULL;
		selectprev = NULL;
		if (head) {
			for (prev = head, p = head; p != NULL; prev = p, p = p->next) {
				if (p->job->jid == deqid) {
					select = p;
					selectprev = prev;
					break;
				}
			}
			selectprev->next = select->next;
			if (select == selectprev)
				head = NULL;
		}
		if (select) {
			for (i = 0; (select->job->cmdarg)[i] != NULL; i++) {
				free((select->job->cmdarg)[i]);
				(select->job->cmdarg)[i] = NULL;
			}
			free(select->job->cmdarg);
			free(select->job);
			free(select);
			select = NULL;
		}
	}
}

void do_stat(){
	//在标准输出上打印出就绪队列中各作业的信息
	struct waitqueue *p;
	char timebuf[BUFLEN];
	printf( "JID\tPID\tOWNER\tRUNTIME\tWAITTIME\tCREATTIME\tSTATE\nJOB NAME\tCURPRI\tDEFPRI\n");
	if (current) {
		strcpy(timebuf,ctime(&(current->job->create_time)));
		timebuf[strlen(timebuf) - 1] = '\0';
		printf("%d\t%d\t%d\t%d\t%d\t%s\t%s\n%s\t%d\t%d\n",current->job->jid,current->job->pid,current->job->ownerid,current->job->run_time,current->job->wait_time,timebuf,"RUNNING",*(current->job->cmdarg),current->job->curpri,current->job->defpri);
	}
	for (p = head; p != NULL; p = p->next) {
		strcpy (timebuf,ctime(&(p->job->create_time)));
		timebuf[strlen(timebuf) - 1] = '\0';
		printf("%d\t%d\t%d\t%d\t%d\t%s\t%s\n%s\t%d\t%d\n",p->job->jid,p->job->pid,p->job->ownerid,p->job->run_time,p->job->wait_time,timebuf,"READY" ,*(current->job->cmdarg),p->job->curpri,p->job->defpri);
	}
	printf("\n");
}

void updateall(){ //更新各作业的优先级
	struct waitqueue *p;
	if (current)
		current->job->run_time += 1; //加1代表10ms
	for (p = head; p != NULL; p = p->next) {
		p->job->wait_time += 10;
		if (p->job->wait_time >= 100 && p->job->curpri < 3)
			p->job->curpri++;
	}
}

struct waitqueue* jobselect(){ //选择优先级最高的作业
	struct waitqueue *p, *prev, *select, *selectprev;
	int highest = -1;
	
	select = NULL;
	selectprev = NULL;
	
	if (head) {
		for (prev = head, p = head; p != NULL; prev = p, p = p->next) {
			if (p->job->curpri > highest) {
				select = p;
				selectprev = prev;
				highest = p->job->curpri;
			}
		}
		selectprev->next = select->next;
		if (select == selectprev) {
			head = NULL;
		}
	}
	return select;
}

void jobswitch(){ //作业切换
	struct waitqueue *p;
	int i;
	
	if (current && current->job->state == DONE) {
		for (i = 0; (current->job->cmdarg)[i] != NULL; i++) {
			free((current->job->cmdarg)[i]);
			(current->job->cmdarg)[i] = NULL;
		}	
		free(current->job->cmdarg);
		free(current->job);
		free(current);
		current = NULL;
	}
	if (next == NULL && current == NULL)
		return;
	else if (next != NULL && current == NULL) {
		printf("begin start new job\n");
		current = next;
		next = NULL;
		current->job->state = RUNNING;
		kill(current->job->pid, SIGCONT);
		return;
	}
	else if (next != NULL && current != NULL) { 
		kill(current->job->pid, SIGSTOP);
		current->job->curpri = current->job->defpri;
		current->job->wait_time = 0;
		current->job->state = READY;

		if (head) {
			for (p = head; p->next != NULL; p = p->next);
			p->next = current;
		}
		else {
			head = current;
		}
		current = next;
		next = NULL;
		current->job->state = RUNNING;
		kill(current->job->pid, SIGCONT);
		return;
	}
	else {
		return;
	}
}

void schedule(){
	struct jobinfo *newjob = NULL;
	struct jobcmd cmd;
	int count = 0;
	bzero(&cmd, DATALEN);
	//读FIFO，判断有无命令，若有则对命令处理
	if((count = read(fifo, &cmd, DATALEN))<0)
		printf("read fifo failed");
	if(count){
		printf("cmd cmdtype\t%d\n""cmd defpri\t%d\n""cmd data\t%s\n",cmd.type,cmd.defpri,cmd.data);
	}
	switch(cmd.type){
		case ENQ:
			do_enq(newjob,cmd);
			break;
		case DEQ:
			do_deq(cmd);
			break;
		case STAT:
			do_stat();
			break;
		default:
			break;
	}
	//否则
	//更新就绪队列作业信息
	updateall();
	//选择优先级最高的作业
	next = jobselect();
	//进行作业切换
	jobswitch();
}

void sig_handler(int sig, siginfo_t *info, void *notused){
	int status;
	int ret;
	switch(sig){
		case SIGVTALRM:  //时钟定时信号, 计算的是实际的时间或时钟时间. 
			schedule();
			return;
		case SIGCHLD:  //子进程结束时, 父进程会收到这个信号。
			ret = waitpid(-1, &status, WNOHANG); //获取子进程的状态
			if(ret==0 || ret==-1)
				return;
			if(WIFEXITED(status)){ //指出子进程是否为正常退出的，如果是，它会返回一个非零值。
				current->job->state = DONE;
				printf("normal termation, exit status = %d\tjid=%d, pid=%d\n\n",WEXITSTATUS(status),current->job->jid,current->job->pid);
			}
			else if(WIFSIGNALED(status)){ //为非0 表明进程异常终止。
				printf("abnormal termation, signal number = %d\tjid=%d, pid=%d\n\n",WTERMSIG(status),current->job->jid,current->job->pid);
			}
			else if(WIFSTOPPED(status)){ //为非0 表明进程处于暂停状态 
				printf("child stopped, signal number = %d\tjid=%d, pid=%d\n\n",WSTOPSIG(status),current->job->jid,current->job->pid);
			}
			return;
		default:
			return;
	}
}

int main(int argc, char *argv[])
{
	struct timeval interval;
	struct itimerval new1;
	struct itimerval old;
	struct stat statbuf;
	struct sigaction newact, oldact1, oldact2;
	
	if(stat(FIFO, &statbuf)==0){ //获取文件对应属性    成功返回0
		if(remove(FIFO)<0){
			printf("remove failed");
		}
	}
	if(mkfifo(FIFO, 0666)<0){ //建立实名管道
		printf("mkfifo failed");
	}
	if((fifo = open(FIFO, O_RDONLY|O_NONBLOCK))<0){ //打开管道
		printf("open fifo failed");
	}
	
	if((globalfd = open("/dev/null",O_WRONLY)) < 0){
		printf("open global file failed!\n");
	}
	
	newact.sa_sigaction = sig_handler;
	sigemptyset(&newact.sa_mask); //清空信号集
	newact.sa_flags = SA_SIGINFO; //若设置了此处，则可以向信号处理函数发送附加信息，否则，只能发送信号的数值
	sigaction(SIGCHLD, &newact, &oldact1);
	sigaction(SIGVTALRM, &newact, &oldact2);

	interval.tv_sec = 0;
	interval.tv_usec = 10;
	new1.it_interval = interval;
	new1.it_value = interval;
	setitimer(ITIMER_VIRTUAL, &new1, &old); //以该进程在用户态下花费的时间来计算，它送出SIGVTALRM信号。
	printf("OK! Scheduler is starting now!!\n");
	
	while(siginfo == 1);
	
	close(fifo);
	return 0;
}
/*
#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[])
{
	int i = 0;
	for(i=0;i<argc;i++){
		printf("%s\n", argv[i]);
	}
	sleep(0.01);
	return 0;
}*/