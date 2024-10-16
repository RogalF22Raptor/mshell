/*#include <stdio.h>
#include "writeHand.h" 
#include "argRedirPars.h"
#include "execvp.h"
#include "utils.h"
#include "builtins.h"
#include "mySignal.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h> 
void handler(int chpid){
    int pid;
    int d;
    while(pid=waitpid(-1,&d,WNOHANG)){
        bool inFG = 0;
        for(int i=0;i<MAX_LINE_LENGTH/2;i++){
            if(fgPID[i]==pid){
                inFG=1;
            }
            if(fgPID[i]==-1)
                break;
        }
        if(inFG){
            fgCount--;
        }
        else{
            if(WIFEXITED(d)){
                bgProces[bgLastDead].bgType=1;
                bgProces[bgLastDead].bgStat = WEXITSTATUS(d);
                bgProces[bgLastDead].bgPID = pid;
                bgLastDead++;
            }
            else if (WIFSIGNALED(d))
            {
                bgProces[bgLastDead].bgType=0;
                bgProces[bgLastDead].bgStat = WIFSIGNALED(d);
                bgProces[bgLastDead].bgPID = pid;
                bgLastDead++;
            }
            

        }
    }
}
*/