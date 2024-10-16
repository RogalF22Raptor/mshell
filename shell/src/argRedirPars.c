#include <stdio.h>
#include "argRedirPars.h"
#include "parser.h"
#include "config.h"
#include "builtins.h" 
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
sigset_t myMask;
sigset_t supsendMask;
pid_t fgPID[MAX_LINE_LENGTH/2];
struct sigaction handlerAction;
struct sigaction sigintAction;
void ernoCheck(int exeRM, char * file){
    char errorHandling[MAX_LINE_LENGTH];
			if(exeRM<0){
				switch (errno)
				{
				case ENOENT:
					memset(errorHandling,0,MAX_LINE_LENGTH);
					strcat(errorHandling,file);
					strcat(errorHandling,": no such file or directory\n");
					writeCheck(STDERR_FILENO,errorHandling,strlen(errorHandling));
					break;
				case EACCES:
					memset(errorHandling,0,MAX_LINE_LENGTH);
					strcat(errorHandling,file);
					strcat(errorHandling,": permission denied\n");
					writeCheck(STDERR_FILENO,errorHandling,strlen(errorHandling));
					break;
				
				default:
					memset(errorHandling,0,MAX_LINE_LENGTH);
					strcat(errorHandling,file);
					strcat(errorHandling,": exec error\n");
					writeCheck(STDERR_FILENO,errorHandling,strlen(errorHandling));
					break;
				}
				exit(1);
			}
}
void argPars(char * ar[], command * com){
    if(com==NULL){
        return;
    }
	argseq * argseq = com->args;
	int i=0;
	do{
		ar[i] = argseq->arg;
		argseq= argseq->next;
		i++;
	}while(argseq!=com->args);
	ar[i]=NULL;
}
void redirPars(command * com, int * fd){
    redirseq * redir = com->redirs;
    if(redir != NULL){
        do{
            if(IS_RIN(redir->r->flags)){
                close(STDIN_FILENO);
                *fd = open(redir->r->filename, O_RDONLY, S_IWUSR | S_IRUSR);
                ernoCheck(*fd,redir->r->filename);
		        dup2(*fd,STDIN_FILENO);
            }
            else if(IS_ROUT(redir->r->flags)){
                close(STDOUT_FILENO);
                *fd = open(redir->r->filename,O_CREAT | O_WRONLY | O_TRUNC,S_IWUSR | S_IRUSR);
                ernoCheck(*fd,redir->r->filename);
		        dup2(*fd,STDOUT_FILENO);
            }
            else if(IS_RAPPEND(redir->r->flags)){
                close(STDOUT_FILENO);
                *fd = open(redir->r->filename,O_CREAT | O_APPEND | O_WRONLY,S_IWUSR | S_IRUSR);
                ernoCheck(*fd,redir->r->filename);
		        dup2(*fd,STDOUT_FILENO);
            }
		    redir=redir->next;
	    }while(redir!=com->redirs);
    }
}