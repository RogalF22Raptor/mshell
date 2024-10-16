#include <stdio.h>
#include "execvp.h"
#include "parser.h"
#include "config.h"
#include "builtins.h" 
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
sigset_t myMask;
sigset_t supsendMask;
pid_t fgPID[MAX_LINE_LENGTH/2];
struct sigaction handlerAction;
struct sigaction sigintAction;
void myExecvp(command * com, char * ar[]){
    int exeRM;
    char errorHandling[MAX_LINE_LENGTH];
			exeRM = execvp(com->args->arg,ar);
			if(exeRM<0){
				switch (errno)
				{
				case ENOENT:
					memset(errorHandling,0,MAX_LINE_LENGTH);
					strcat(errorHandling,com->args->arg);
					strcat(errorHandling,": no such file or directory\n");
					writeCheck(STDERR_FILENO,errorHandling,strlen(errorHandling));
					break;
				case EACCES:
					memset(errorHandling,0,MAX_LINE_LENGTH);
					strcat(errorHandling,com->args->arg);
					strcat(errorHandling,": permission denied\n");
					writeCheck(STDERR_FILENO,errorHandling,strlen(errorHandling));
					break;
				
				default:
					memset(errorHandling,0,MAX_LINE_LENGTH);
					strcat(errorHandling,com->args->arg);
					strcat(errorHandling,": exec error\n");
					writeCheck(STDERR_FILENO,errorHandling,strlen(errorHandling));
					break;
				}
				_exit(EXEC_FAILURE);
			}
}