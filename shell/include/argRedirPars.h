#ifndef _argRedirPars_H_
#define _argRedirPars_H_

#include "siparse.h" 
void argPars(char * ar[], command * com);
void redirPars(command * com, int *);
void ernoCheck(int exeRM, char * file);

#endif
