#include <stdio.h>
#include <errno.h> 
#include "config.h"
#include "builtins.h"
#include "parser.h"
#include "argRedirPars.h"
#include "execvp.h"
#include "utils.h"
#include <unistd.h>
#include <sys/wait.h>
#include "mySignal.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "writeHand.h"
sigset_t myMask;
sigset_t supsendMask;
pid_t fgPID[MAX_LINE_LENGTH/2];
struct sigaction handlerAction;
struct sigaction sigintAction;
volatile int fgCount = 0;
volatile int bgLastDead = 0;
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
}
void setupSignals() {
    sigemptyset(&myMask);
    sigaddset(&myMask, SIGCHLD);
    sigemptyset(&supsendMask);
    sigaddset(&supsendMask, SIGINT);

    // Signal handler for SIGCHLD
    handlerAction.sa_handler = handler;
    handlerAction.sa_flags = 0;
    sigemptyset(&handlerAction.sa_mask);
    sigaction(SIGCHLD, &handlerAction, NULL);

    // Ignore SIGINT
    sigintAction.sa_handler = siginHandler;
    sigintAction.sa_flags = 0;
    sigemptyset(&sigintAction.sa_mask);
    sigaction(SIGINT, &sigintAction, NULL);
}
void siginHandler(int chpid){
    if(isatty(STDIN_FILENO)){
        writeCheck(STDOUT_FILENO,"\n",1);
    }
	
}
void handler(int chpid){
    int pid;
    int d;
    while(1){
		pid=waitpid(-1,&d,WNOHANG);
		if(pid<=0){
			return;
		}
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

bool hasNext(commandseq * com, commandseq * firstCom){
	return com->next!=firstCom;
}
bool hasPrev(commandseq * com, commandseq * firstCom){
	return com->prev!=firstCom->prev;
}
void run(command * com, int fdIn, int fdOut, char * ar[], int * pipe){
	if(fdIn!=0){
		close(STDIN_FILENO);
		dup2(fdIn,STDIN_FILENO);
		close(fdIn);
	}
	if(fdOut!=1){
		close(STDOUT_FILENO);
		dup2(fdOut,STDOUT_FILENO);
		close(pipe[1]);
		close(pipe[0]);
	}
    int fd=-1;
	argPars(ar,com);
	redirPars(com,&fd);
	myExecvp(com,ar);
    close(fd);
}
void bufParser(char *buf, int bytes){
	int PID;
	int exeRM;
	char *ar[MAX_LINE_LENGTH/2+1];
	char errorHandling[MAX_LINE_LENGTH];
	int syntaxLen = strlen(SYNTAX_ERROR_STR)+2;
	buf[bytes]='\0';
	pipelineseq * ln = parseline(buf);
		if(ln==NULL){
			char er[syntaxLen];
			memset(er,0,syntaxLen);
			strcat(er,SYNTAX_ERROR_STR);
			strcat(er,"\n");
			writeCheck(STDERR_FILENO,er,syntaxLen); 
		}
	if(ln==NULL){
		return;
	}
    //printparsedline(ln);
	pipelineseq * tempPipeline=ln;
    sigprocmask(SIG_BLOCK,&myMask,NULL);
	do{
		int comandCounter=0;
		commandseq * firstComand = tempPipeline->pipeline->commands;
		commandseq * comIterator = tempPipeline->pipeline->commands;
		do
		{
			comandCounter++;
			comIterator=comIterator->next;
		} while (comIterator!=firstComand);
        if(firstComand->com==NULL){
            
            return;
        }
        
		if(comandCounter==1){
            bool ifBuiltins=0;
            ar[0]=NULL;
            ar[1]=NULL;
            ar[2]=NULL;
            ar[3]=NULL;
            ar[4]=NULL;
			argPars(ar,firstComand->com);
			for(int z=0;builtins_table[z].name!=NULL;z++){
                if(strcmp(firstComand->com->args->arg,builtins_table[z].name)==0){
                    builtins_table[z].fun(ar);
                    ifBuiltins=1;
                    break;
                }
	        }
            if(ifBuiltins){
                tempPipeline=tempPipeline->next;
                continue;
            }
            
		}
		int fdIn=0;
		int fdOut=1;
        int fgChild = 0;
		int nextpipe[2]={-1,-1};
		int prevpip[2]={-1,-1};
		do
		{
            fdIn=0;
		    fdOut=1;
			if(hasNext(comIterator,firstComand)){
				pipe(nextpipe);
				fdOut = nextpipe[1];
			}
			if(hasPrev(comIterator,firstComand)){
				fdIn = prevpip[0];
			}
			PID=fork();
			if(PID<0){ 
				writeCheck(STDERR_FILENO,"error\n",strlen("error\n"));
				exit(1);
			}
			if(PID==0){
                if(tempPipeline->pipeline->flags == INBACKGROUND){
                    setsid();
                }
				run(comIterator->com,fdIn,fdOut, ar,nextpipe);
			}
			else{
                if(tempPipeline->pipeline->flags != INBACKGROUND){
                    fgPID[fgChild++] = PID;
                    fgPID[fgChild] = -1;
                    fgCount++;
                }
                close(nextpipe[1]);
                nextpipe[1]=-1;
				close(prevpip[0]);
				close(prevpip[1]);
				prevpip[0]=nextpipe[0];
				prevpip[1]=nextpipe[1];
                nextpipe[0] = -1;
                nextpipe[1] = -1;
                if(fdOut==1){
                    close(prevpip[1]);
                }
                
			}
			comIterator=comIterator->next;
		} while (comIterator!=firstComand);
		tempPipeline=tempPipeline->next;
        while(fgCount>0){
            sigsuspend(&supsendMask);
        }
	}while(tempPipeline!=ln);
    sigprocmask(SIG_UNBLOCK,&myMask,NULL);
}