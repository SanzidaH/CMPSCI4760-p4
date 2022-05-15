/*libraries*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<getopt.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <limits.h>


/* macros */
#define MAXPROC 20
#define SHM_KEY 0x12345 //shared memory key
#define LEN 150
#define TO_NANO 1000000000
#define timeslice 100000


/* DS */
struct msgbuf
{
    long mtype;
    char mtext[200];
};

struct PCB {//processCtrlBlock
    float cpuTimeUsed;//totalCpuTime
    float totalTimeInSystem;//timeInSystem
    float lastBurstTime;//lastBurstTime
    int pid;
    int processPriority;
    bool inUse;    

};
struct PCB *pct_ptr;

/* shm variables */

//System clock
unsigned int *clock_s;
unsigned int *clock_ns;
int clock_sid, clock_nsid;
int pct_id, msgq_id, errno;
FILE *cstest = NULL, *file = NULL, *logfile = NULL;
int * allpid = NULL;
/* functions */   
void cleanAll();
void increase_clock(int inc);
char msg[100];
/* signalhandlers */
void signal_timer(int signal);
void signal_abort();
void logging(char * str);
/* iterators */
int i, j, c; 
