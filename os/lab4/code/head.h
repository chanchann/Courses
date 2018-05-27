#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO "/home/chanchan/os/lab2/co/fifo"
#define BUFLEN 100

enum jobstate {
	READY, RUNNING, DONE
};
enum cmdtype {
	ENQ = -1, DEQ = -2, STAT = -3
};
/* this is data passed in fifo */
struct jobcmd {	
	enum cmdtype type;
	int argnum;
	int owner;
	int defpri;
	char data[BUFLEN];
};
#define DATALEN sizeof(struct jobcmd)
struct jobinfo {
	int jid; /* job id */
	int pid; /* process id */
	char** cmdarg; /* the command & args to execute */
	int defpri; /* default priority */
	int curpri; /* current priority */
	int ownerid; /* the job owner id */
	int wait_time; /* the time job in waitqueue */
	time_t create_time; /* the time job create */
	int run_time; /* the time job running */
	enum jobstate state; /* job state */
};
struct waitqueue { /* double link list */
	struct waitqueue *next;
	struct jobinfo *job;
};

struct waitqueue* jobselect();
struct waitqueue *head = NULL;
struct waitqueue *next = NULL, *current = NULL;
int fifo;
int siginfo = 1;
int jobid=0;
