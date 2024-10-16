#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "config.h"
#include "siparse.h"
#include "utils.h"
#include "execvp.h"
#include "argRedirPars.h"
#include "parser.h"
#include <errno.h>
#include <builtins.h>
#include <stdbool.h>
#include <signal.h>
#include "writeHand.h"
int f;
sigset_t myMask;
sigset_t supsendMask;
pid_t fgPID[MAX_LINE_LENGTH/2];
struct sigaction handlerAction;
struct sigaction sigintAction;

struct bgSignal bgProces[MAX_LINE_LENGTH/2];
int main(int argc, char *argv[])
{
	setupSignals();
	f = isatty(STDIN_FILENO);
	char buf[MAX_LINE_LENGTH+1];
	int numberOfBytes;
	char bufHelp[MAX_LINE_LENGTH*8+1]; // temporary buffer
	char *startPointer;
	char *endPointer;
	int beginBuf=0; // contein if our bufferhave something at the beggining
	bool wasEndl;
	wasEndl = 1;
	bool vidmoWait;
	vidmoWait = 1;
	int tempBytes=0;
	while(1){
		if(f){
			printBgDead();
			writeCheck(STDOUT_FILENO,PROMPT_STR,strlen(PROMPT_STR));
		}
		memset(bufHelp+beginBuf,'\0',MAX_LINE_LENGTH*8+1-beginBuf);
		numberOfBytes = read(STDIN_FILENO,bufHelp+beginBuf,MAX_LINE_LENGTH*8-beginBuf);
		if(numberOfBytes<0){
			//5 stage
			continue;
		}
		if(numberOfBytes==0){
			if(beginBuf>MAX_LINE_LENGTH){
				writeCheck(STDERR_FILENO,LINE_ERROR,strlen(LINE_ERROR));
				exit(0);
			}
			memcpy(buf,bufHelp,beginBuf);
			buf[beginBuf]='\n';
			bufParser(buf,beginBuf+1);
			exit(0);
		}
		bufferHandler(buf,bufHelp,bufHelp,&beginBuf,numberOfBytes,&wasEndl,&tempBytes,&vidmoWait,f);
	}
	writeCheck(STDOUT_FILENO,"\n",strlen("\n"));
	return 0;
}