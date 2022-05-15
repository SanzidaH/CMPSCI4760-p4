/**
* @Author: Sanzida Hoque
* @Course: CMP_SCI 4760 Operating Systems 
* @Sources: https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_message_queues.htm
//https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
*           
*/

#include "config.h"


struct PCB * pct_ptr = NULL;
struct Queue * queue = NULL;//queue[0] blocked_queue, queue[1] - priority 1, queue[2] - priority 2, queue[3] - priority 3, queue[4] - priority 4
char lbuf[500];
int opt, nprocs = 20, terminate_time = 3, errno, pid = 0, forkedchild = 0, linenum = 0;

const unsigned int maxTimeBetweenNewProcsNS = TO_NANO;
const unsigned int maxTimeBetweenNewProcsSecs = 1;

int main(int argc, char *argv[]){

    /* Interrupt signal handling */
    signal(SIGALRM, signal_timer);//Abort for end of termination time 
    signal(SIGINT, signal_abort);// Abort for Ctrl+C  
    
    //signal(SIGALRM, cleanAll);//Abort for end of termination time 
    //signal(SIGINT, cleanAll);// Abort for Ctrl+C  
    
    allpid = malloc(sizeof(int) * MAXPROC);
     /* Logging */
    srand(time(NULL));
    file = fopen("osslog", "w+");
    fclose(file);
    
    /* Creating a new shared memory segment */ 
    clock_nsid = shmget(ftok("Makefile", '1'), sizeof(unsigned int), IPC_CREAT | 0666);
    clock_sid = shmget(ftok("Makefile", '2'), sizeof(unsigned int), IPC_CREAT | 0666);
    pct_id = shmget(ftok("Makefile", '3'), sizeof(struct PCB) * MAXPROC, IPC_CREAT | 0666);
    msgq_id = msgget(ftok("Makefile", '4'), 0644 | IPC_CREAT);
    
    //printf("ipcrm -m %d\n", clock_nsid);
    //printf("ipcrm -m %d\n", clock_sid);
    //printf("ipcrm -m %d\n", pct_id);
    
    if (clock_nsid == -1 || clock_sid == -1 || pct_id == -1 || msgq_id == -1){
        perror("oss: Error: Shared memory allocation failed");
     // return 1;
    }
    
    clock_ns = (unsigned int *)shmat(clock_nsid, NULL, 0);
    clock_s = (unsigned int *)shmat(clock_sid, NULL, 0);
    pct_ptr = (struct PCB *)shmat(pct_id, NULL, 0);

    if (clock_ns == (void *) -1 || clock_s == (void *) -1 || pct_ptr == (void *) -1) {
       perror("oss: Error: Shared memory attachment failed");
      //return 1;
    }

    /* Setting System clock to zero */
    clock_ns[0] = 0;
    clock_s[0] = 0;
	
    int localPidCount = 0;
    unsigned int rIntervalS, rIntervalNS, processScheduleNS, currentNS;
    alarm(terminate_time);
    //struct msgbuf msgBuf;
    for (int i = 0; i < MAXPROC; i++){    
    struct PCB tempProc = { 0.0, 0.0, 0.0, i, 1, false}; // initially priority 1
    pct_ptr[i] = tempProc; // Allocating and initializing PCB 
    }
    
    while (1){
       while (clock_ns[0] + clock_s[0]*TO_NANO < terminate_time*TO_NANO || localPidCount< MAXPROC){                
          increase_clock(100);
          /* random intervals to spawn process */
         if(processScheduleNS == 0){
            processScheduleNS = (clock_ns[0]) + (clock_s[0] * TO_NANO) + (rand() % maxTimeBetweenNewProcsNS) + 1;
         }          
        /* incrementing the clock until it is the time where it should launch a process. */
            //Hit time
             if((clock_ns[0] + clock_s[0]*TO_NANO) >= processScheduleNS && forkedchild < 50){ 
             struct msgbuf msgBuf;


        /** Generating new process **/                                                             
        pct_ptr[localPidCount].pid = fork();
         sprintf(lbuf, "OSS: Generating process with local PID %d and putting in queue 1 at time %d:%d\n", localPidCount+1, clock_s[0], clock_ns[0]);
         logging(lbuf);
         printf("OSS: Generating process with local PID %d and putting in queue 1 at time %d:%d\n", localPidCount+1, clock_s[0], clock_ns[0]);
         /* Checking fork */
        if (pct_ptr[localPidCount].pid == -1){
             perror("OSS: Error: Failed to create a child process");
            abort();
        }
        if (pct_ptr[localPidCount].pid == 0){
            char * procid = malloc(10*sizeof(char));
            sprintf(procid, "%d", localPidCount);
            execl("./process", "./process", procid, NULL);
        }
        else{
             forkedchild++;
            //localPidCount++;
             pct_ptr[localPidCount].inUse = true;
             allpid[localPidCount] = pct_ptr[localPidCount].pid;  
        }
        /* making message */
        msgBuf.mtype = pct_ptr[localPidCount].pid;
        strcpy(msgBuf.mtext, "to userprocess\0");
        int len = strlen(msgBuf.mtext);
        bool flag = false;
        if (msgsnd(msgq_id, &msgBuf, len+1, 0) == -1){
            perror("OSS: Error: msgsnd failed\n");
            abort();
        }
                  int rdispatchNS = (rand() % (10000 - 100 + 1 )) + 100; // int num = (rand() % (upper - lower + 1)) + lower;
                  increase_clock(rdispatchNS);//time taken for scheduling process before launching              
                  //file = fopen("osslog", "a+"); 
                  //fprintf(lbuf, "OSS: Dispatching process with local PID %d", localPidCount); 
                 // fclose(file); 
                   //sprintf(lbuf, "OSS: Dispatching process %d\n", localPidCount); 
                   //file = fopen("osslog", "a+"); 
                   //fprintf(lbuf, "OSS: Dispatching process"); 
                   //fclose(file); 
                   flag = true;                  
                  printf("OSS: Dispatching process with local PID %d and putting in queue 1 at time %d:%d\n", localPidCount+1, clock_s[0], clock_ns[0]); 
                  sprintf(lbuf, "OSS: Dispatching process with local PID %d and putting in queue 1 at time %d:%d\n", localPidCount+1, clock_s[0], clock_ns[0]); 
                  //printf("OSS: Dispatching process with local PID %d and putting in queue %d at time %d:%d\n", localPidCount, clock_s[0], clock_ns[0]);
                  logging(lbuf);
         
        /* receiving message */
        if (msgrcv(msgq_id, &msgBuf, sizeof(msgBuf.mtext), localPidCount, 0) == -1){
           perror("OSS: Error: msgrcv failed\n");
            abort();
        }
        char msg[100];
        strcpy(msg, msgBuf.mtext);
        printf("OSS: Message received from user %d\n", localPidCount);
        sprintf(lbuf,"OSS: Message received from user %d\n", localPidCount);
        logging(lbuf);
       // sprintf("OSS: Message received from user %d\n", localPidCount);
         /* localpid increment */
        localPidCount++;
       }
      }
    }

    cleanAll();

}

void increase_clock(int inc){
        clock_ns[0] = clock_ns[0] + inc;//Increasing system clock
        while (clock_ns[0] >= TO_NANO) {//Coversion of nanosecond to second 
               clock_s[0] = clock_s[0] + 1;
               clock_ns[0] = clock_ns[0] - TO_NANO;
         }
}

void cleanAll(){   
    
    sprintf(lbuf, "OSS: Cleaning and terminating ...");
    logging(lbuf);
    
    if (shmdt(clock_ns) == -1 || shmdt(clock_s) == -1 || shmdt(pct_ptr) == -1) {
      perror("OSS: Error: shmdt failed to detach memory");
    }
    if (shmctl(clock_nsid, IPC_RMID, 0) == -1 || shmctl(clock_sid, IPC_RMID, 0) == -1 || shmctl(pct_id, IPC_RMID, 0) == -1) {
      perror("OSS: Error: shmctl failed to delete shared memory");
    } 
    
    if(msgctl(msgq_id, IPC_RMID, NULL) == -1){
       perror("OSS: Error: msgctl failed");
    }
    abort();
}

void logging(char * str){
    file = fopen("osslog", "a+");   
    linenum++;
    if (linenum <= 10000){
        fputs(str, file);
    }
    else{
        printf("OSS: logfile exceeds 10000 line, terminating\n");
        fputs("OSS: logfile exceeds 10000 line, terminating\n", file);       
    }
    fclose(file);
}

/* signal handle for time out */
void signal_timer(int signal){
	   perror("OSS: Warning: timer ends");
	  /* killpg((*allpid), SIGTERM);
	    for(int i = 0; i < MAXPROC; i++){
		wait(NULL);
	    }*/
	   
	   
	   cleanAll();
	   
	   //abort();
	   //exit(EXIT_FAILURE);
	
}


/* signal handle for receiving CTRL + C */
void signal_abort(){ 
    perror("OSS: Warning: CTRL + C received, master is terminating");
    /*killpg((*allpid), SIGTERM);
    for(int i = 0; i < MAXPROC; i++){
		wait(NULL);
	    }*/
    cleanAll();
    //abort();
    //exit(EXIT_FAILURE);
}


struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
};
 

struct Queue* createQueue(int capacity)
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(
    queue->capacity * sizeof(int));
    return queue;
}
 

int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
 
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}
 
// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}
 
// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}
 
// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}
