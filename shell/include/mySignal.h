/*#ifndef _MYSIGNAL_H_
#define _MYSIGNAL_H_ 

#include "siparse.h"
#include "config.h"
#include <stdbool.h>
#include <signal.h>
#include <fcntl.h>
volatile int fgCount = 0;
volatile int bgLastDead = 0;
sigset_t myMask;
sigset_t supsendMask;
pid_t fgPID[MAX_LINE_LENGTH/2];
struct bgSignal
{
    pid_t bgPID;
    int bgStat;
    int bgType;
} bgProces[MAX_LINE_LENGTH/2];
void handler(int chpid);
struct sigaction handlerAction;
struct sigaction sigintAction;


#endif
 */