/*#include "mask.h"
#include "mySignal.h"
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
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
    sigintAction.sa_handler = SIG_IGN;
    sigintAction.sa_flags = 0;
    sigemptyset(&sigintAction.sa_mask);
    sigaction(SIGINT, &sigintAction, NULL);
}*/