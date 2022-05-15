/**
* @Author: Sanzida Hoque
* @Course: CMP_SCI 4760 Operating Systems 
* @Sources: https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_message_queues.htm
*           
*/

#include "config.h"

int main(int argc, char *argv[]){

    signal(SIGINT, signal_abort);
    signal(SIGALRM, signal_abort);
    signal(SIGTERM, signal_abort);
    
    alarm(3);
    //char lbuf2[200];
    //int procStartNS, procStartS; 
    int procid = atoi(argv[1]);
    bool terminate = false, block= false;
    struct msgbuf msgBuf;

    printf("Process %d starts...\n", procid+1);
   // sprintf(lbuf2,"Process %d starts...\n", procid);
    //logging(lbuf2);
    /* Creating a new shared memory segment */ 
    clock_nsid = shmget(ftok("Makefile", '1'), sizeof(unsigned int), IPC_EXCL | 0666);
    clock_sid = shmget(ftok("Makefile", '2'), sizeof(unsigned int), IPC_EXCL | 0666);
    pct_id = shmget(ftok("Makefile", '3'), sizeof(struct PCB) * MAXPROC, IPC_EXCL | 0666);
    
    //printf("ipcrm -m %d\n", clock_nsid);
    //printf("ipcrm -m %d\n", clock_sid);
 
    
    if (clock_nsid == -1 || clock_sid == -1 || pct_id == -1){
        perror("process: Error: Shared memory allocation failed");
        abort();
     // return 1;
    }
    
    clock_ns = (unsigned int *)shmat(clock_nsid, NULL, 0);
    clock_s = (unsigned int *)shmat(clock_sid, NULL, 0);
    pct_ptr = (struct PCB*)shmat(pct_id, 0, 0);

    if (clock_ns == (void *) -1 || clock_s == (void *) -1 || pct_ptr == (void *) -1) {
       perror("process: Error: Shared memory attachment failed");
       abort();
      //return 1;
    }
 
    msgq_id = msgget(ftok("Makefile", '4'), 0644 | IPC_EXCL);
       //printf("ipcrm -m %d\n", msgq_id);
    if (msgq_id == -1){
        perror("Process: Error: msgget failed\n");
        cleanAll();
        //exit(EXIT_SUCCESS);
        abort();
    }

    while(1){
    
        if(terminate){
           //TODO tell OSS - increase cpu time for this process, last burst time, remove from queue
        }
        else if(block){
           //TODO tell OSS - increase cpu time for this process, last burst time, increase priority, add to block queue. 
        }
           //TODO time slice completed, increse CPU time one time slice
        sleep(1);
        if (msgrcv(msgq_id, &msgBuf, sizeof(msgBuf.mtext), getpid(), 0) == -1){
            perror("Process: Error: msgrcv failed\n");
            cleanAll();
        }
        strcpy(msg, msgBuf.mtext);
        printf("process %d: Message received: \"%s\"\n", procid, msgBuf.mtext);
        msgBuf.mtype = procid;
        
        if (msgsnd(msgq_id, &msgBuf, strlen(msgBuf.mtext)+1, 0) == -1){
            perror("Process: Error: msgsnd failed\n");
            cleanAll();
        }
        break;
    }

    cleanAll();
    return 0;

}


void increase_clock(int inc){
        clock_ns[0] = clock_ns[0] + inc;//Increasing system clock
        while (clock_ns[0] >= TO_NANO) {//Coversion of nanosecond to second 
               clock_s[0] = clock_s[0] + 1;
               clock_ns[0] = clock_ns[0] - TO_NANO;
         }
}

void signal_abort(){ 
    //perror("process: Warning: CTRL + C received, master is terminating");

    for (int p = 0; p < MAXPROC; p++) {
        wait(NULL);}
    perror("Process exits..");
    kill(getpid(), SIGTERM);
    cleanAll();
    
    //abort();
    //exit(EXIT_FAILURE);
}

void cleanAll(){
    if (shmdt(clock_ns) == -1 || shmdt(clock_s) == -1 || shmdt(pct_ptr) == -1) {
      perror("process: Error: shmdt failed to detach memory");
      abort();
    }
    abort();
    //exit(EXIT_SUCCESS);  
}
/*
void logging(char * str){
    file = fopen("osslog", "a+");   
    fputs(str, file);
    fclose(file);
}*/
