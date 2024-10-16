#ifndef _READLINE_H_
#define _READLINE_H_

#include "siparse.h"
#include "config.h"
#include <stdbool.h> 
#include <signal.h>
#include <fcntl.h> 
void bufferHandler(char *buf, char *bufHelp, char * startPointer, int * beginBuf, int bufBytes, bool * wasEndl, int *, bool *,int);
#endif
