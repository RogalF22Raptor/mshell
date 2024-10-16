/*#include "bgDead.h"
#include "mySignal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h>
void printBgDead(){
    char PID[55];
    if(bgLastDead>0){  
        sigprocmask(SIG_BLOCK,&myMask,NULL);
        for(int i = 0; i<bgLastDead;i++){
            sprintf(PID,"Background process %d terminated. ",bgProces[i].bgPID);
            writeCheck(STDERR_FILENO,PID, strlen(PID));
            if(bgProces[i].bgType){
                writeCheck(STDERR_FILENO,"(exited with status ", 20);
            }
            else{
                writeCheck(STDERR_FILENO,"(killed by signal ", 18);
            }
            char status[10];
            sprintf(status, "%d", bgProces[i].bgStat);
            writeCheck(STDERR_FILENO, status, strlen(status));
            writeCheck(STDERR_FILENO, ")\n", 2);
        }
        bgLastDead = 0;
        sigprocmask(SIG_UNBLOCK,&myMask,NULL);
    }
}*/