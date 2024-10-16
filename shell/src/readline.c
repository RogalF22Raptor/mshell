#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include "readline.h"

void bufferHandler(char *buf, char *bufHelp, char * startPointer, int * beginBuf, int bufBytes, bool * wasEndl,int  *tempBytes, bool * vidmoWait,int f){

	bufBytes+=*beginBuf;
	char *endlPointer;
	startPointer=bufHelp;
	int bytes;
	int noWhile=0;
	while(endlPointer=strchr(startPointer,'\n')){
		
		noWhile=1;
		if(*wasEndl==0 && *vidmoWait==0){
			startPointer=endlPointer+1;
			*wasEndl=1;
			continue;
		}
		bytes=endlPointer-startPointer+1;
		if(bytes>MAX_LINE_LENGTH){
			writeCheck(STDERR_FILENO,SYNTAX_ERROR_STR,strlen(SYNTAX_ERROR_STR));
			if(f){
				writeCheck(STDOUT_FILENO,PROMPT_STR,strlen(PROMPT_STR));
			}
			
			startPointer=endlPointer+1;
			continue;
		}
		memcpy(buf,startPointer,bytes);
		bufParser(buf,bytes);
		startPointer=endlPointer+1;
		*vidmoWait=0;
		*wasEndl=1;
	}
	if(!noWhile){
		if(bufBytes<MAX_LINE_LENGTH*8){
			*vidmoWait=1;
		}
		else{
			*vidmoWait=0;
		}
		*wasEndl=0;
	}
	if(*wasEndl){
		memmove(bufHelp,startPointer,bufBytes-(int)(startPointer-bufHelp));
		*beginBuf=bufBytes-(int)(startPointer-bufHelp);
	}
	else if (!(*wasEndl) && !(*vidmoWait)){
		if(bufBytes>MAX_LINE_LENGTH){
			writeCheck(STDERR_FILENO,SYNTAX_ERROR_STR,strlen(SYNTAX_ERROR_STR));
			startPointer=endlPointer+1;
		}
		else{
			*beginBuf = bufBytes;
		}
	}
	else{
		*beginBuf = bufBytes;
	}
}