#ifndef _PARSER_H_
#define _PARSER_H_

#include "siparse.h"
#include "config.h"
#include <stdbool.h> 
#include <signal.h>
#include <fcntl.h> 
extern volatile int fgCount; 
extern volatile int bgLastDead; 
void bufParser(char *buf, int bytes);
bool hasNext(commandseq * com, commandseq * firstCom);
bool hasPrev(commandseq * com, commandseq * firstCom);
void run(command * com, int fdIn, int fdOut, char * ar[], int *);

extern sigset_t myMask;
extern sigset_t supsendMask;
extern pid_t fgPID[MAX_LINE_LENGTH/2];
extern struct bgSignal
{
    pid_t bgPID;
    int bgStat;
    int bgType;
};
extern struct bgSignal bgProces[MAX_LINE_LENGTH/2];
void handler(int chpid);
void siginHandler(int chpid);
extern struct sigaction handlerAction;
extern struct sigaction sigintAction;
void printBgDead();
void setupSignals(); 
#endif
