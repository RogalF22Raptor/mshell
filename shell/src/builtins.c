#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"
#include <dirent.h>
#include <sys/stat.h>
#include <parser.h>
sigset_t myMask;
sigset_t supsendMask;
pid_t fgPID[MAX_LINE_LENGTH/2];
struct sigaction handlerAction;
struct sigaction sigintAction;
int echo(char*[]);
int lexit(char *[]);
int lkill(char *[]);
int lcd(char *[]);
int lls(char *[]);
int undefined(char *[]);
char error[30];
builtin_pair builtins_table[]={
	{"exit",	&lexit},
	{"lecho",	&echo},
	{"lcd",		&lcd},
	{"lkill",	&lkill},
	{"lls",		&lls},
	{NULL,NULL}
};

int 
echo( char * argv[])
{
	int i =1;
	if (argv[i]) printf("%s", argv[i++]);
	while  (argv[i])
		printf(" %s", argv[i++]);

	printf("\n");
	fflush(stdout);
	return 0;
}
int lexit(char * argv[]){
	if(argv[1]!=NULL){
		sprintf(error, "Builtin %s error.\n", argv[0]);
		writeCheck(STDERR_FILENO,error, strlen(error));
		return BUILTIN_ERROR;
	}
	//error if more than argv[1] ont null
	_exit(EXIT_SUCCESS);
}
int lkill(char * argv[]){

	if(argv[1]==NULL){
		sprintf(error, "Builtin %s error.\n", argv[0]);
		writeCheck(STDERR_FILENO,error, strlen(error));
		return BUILTIN_ERROR; 
	}
	int signal_number=SIGTERM;
	int PID;
	if(argv[1]!=NULL && argv[1][0]=='-'){
		signal_number=atoi(argv[1]+1);
		if(argv[2]!=NULL){
			PID=atoi(argv[2]);
		}
		else{
			sprintf(error, "Builtin %s error.\n", argv[0]);
			writeCheck(STDERR_FILENO,error, strlen(error));
			return BUILTIN_ERROR; 
		}
	}
	else if(argv[1][0]!='-' && argv[2]==NULL){
		PID=atoi(argv[1]);
	}
	else{
		sprintf(error, "Builtin %s error.\n", argv[0]);
		writeCheck(STDERR_FILENO,error, strlen(error));
		return BUILTIN_ERROR; 
	}

	if(!kill(PID,signal_number)){
		return 0;
	}
	else{
		_exit(EXIT_FAILURE);
	}
}
int lcd(char *argv[]){
	char * path;
	if(argv[2]!=NULL){
		sprintf(error, "Builtin %s error.\n", argv[0]);
		writeCheck(STDERR_FILENO,error, strlen(error));
		return BUILTIN_ERROR;
	}
	else if(argv[1]!=NULL){
		path=argv[1];
		if(chdir(path)<0){
			sprintf(error, "Builtin %s error.\n", argv[0]);
			writeCheck(STDERR_FILENO,error, strlen(error));
			return BUILTIN_ERROR;
		}
	}
	else{
		path=getenv("HOME");
		if(chdir(path)<0){
			sprintf(error, "Builtin %s error.\n", argv[0]);
			writeCheck(STDERR_FILENO,error, strlen(error));
			return BUILTIN_ERROR;
		}
	}

	return 0;
}

int lls(char * argv[]){
	if(argv[1]!=NULL){
		sprintf(error, "Builtin %s error.\n", argv[0]);
		writeCheck(STDERR_FILENO,error, strlen(error));
		return BUILTIN_ERROR;
	}
	DIR *path;
	struct dirent * tab;
	path=opendir(".");
	if(path==NULL){
		sprintf(error, "Builtin %s error.\n", argv[0]);
		writeCheck(STDERR_FILENO,error, strlen(error));
		return BUILTIN_ERROR;
	}
	
	while ((tab=readdir(path))!=NULL)
	{
		if(tab->d_name[0]!='.'){
			strcat(tab->d_name,"\n");
			writeCheck(STDOUT_FILENO,tab->d_name,strlen(tab->d_name));
		}
	}
	closedir(path);
	return 0;
}

int 
undefined(char * argv[])
{
	sprintf(error, "Command %s undefined.\n", argv[0]);
	writeCheck(STDERR_FILENO,error, strlen(error));
	return BUILTIN_ERROR;
}
