#ifndef SHELL_H
#define SHELL_H
#include <signal.h>

#include "stack.h"

extern pid_t shell_PID;
extern int EXIT_STAT;
extern struct sigaction act;//should be a Global since it wont be shared anyway
extern char* stream;
extern char PWD[1000];
extern STACK DIRSTACK;
extern STACK HISTSTACK;
extern char *host_name;
extern char *user_name;

int wish_init();
void shell_loop();
void printPrompt();
void list_files(char *directory);
void changedir(char** argv);
void sighandler(int);
#endif