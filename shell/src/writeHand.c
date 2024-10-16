#include <stdio.h>
#include <errno.h> 
#include "config.h"
#include "builtins.h"
#include "utils.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void writeCheck(int fd, const void *buf, size_t count){
	if(write(fd,buf,count)<0){
		exit(EXIT_FAILURE);
	}
}