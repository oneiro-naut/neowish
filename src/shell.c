#include <dirent.h> //for directory related syscalls
#include <time.h>

#include "shell.h"
#include "readline.h"
#include "execute.h"
#include "w_parser.h"
#include "w_env.h"
#include "w_io.h"

pid_t shell_PID;
int EXIT_STAT;
STACK DIRSTACK;
STACK HISTSTACK;
char PWD[1000];
struct sigaction act;
char* stream = NULL;
char *host_name = NULL;
char *user_name = NULL;

/*void getprompt()
{
    // Retrieving hostname from /etc/hostaname file
    FILE *file = fopen("/etc/hostname","r");
    host_name=(char *)malloc(15*sizeof(char));
    if(!file)
    exit(0);
    fscanf(file,"%s",host_name);
    fclose(file);

    // Retrieving username using getlogin() fron <unistd.h>
    user_name=(char *)malloc(10*sizeof(char));
    user_name=getlogin();
}*/

void sighandler(int sig_num) { 
  //sleep(100);
  //fflush(stdout); 
} 

int wish_init() {
  //setting up signal handler for Parent process that is wish shell
  memset(&act, 0, sizeof(act));
  act.sa_handler = sighandler;
  act.sa_flags=0;//not SA_RESTART
  sigaction(SIGINT,  &act, 0);//should work only for parent process 
  sigaction(SIGTSTP, &act, 0);//
  getcwd(PWD,PATHLEN);
  init_stack(&DIRSTACK);
  init_stack(&HISTSTACK);
  initreadLine(INPUTSIZE);  
  STD_IN_DUP =0;//0
  STD_OUT_DUP =1;//1
  STD_ERR_DUP =2;//2
  // Retrieving hostname from /etc/hostaname file
  FILE *file = fopen("/etc/hostname","r");
  host_name = (char *) malloc(15*sizeof(char));
  if (!file) exit(0);
  fscanf(file, "%s", host_name);
  fclose(file);
  // Retrieving username using getlogin() fron <unistd.h>
  user_name = (char *) malloc(10*sizeof(char));
  user_name = getlogin();
}

void printPrompt() {
  printf("\033[0;31m"); 
  printf("\033[1m");
  printf("%s  ➜  ", host_name);
  printf("\033[0m");
	printf("\033[0m"); 
}

void shell_loop() {     
  cmd_struct* current_cmd_struct = NULL;
  while (1) {
    /* code */
    printPrompt();
    stream=readLine();
    if(stream!=NULL)push(&HISTSTACK,stream); 
    //parse function here
    current_cmd_struct = parse_input(stream);
    execute_cmd_struct(current_cmd_struct);
    free_cmd_struct(&current_cmd_struct);
    if (stream!=NULL) {
      free(stream); //free cant free NULL lol
      stream= NULL;
    }
  }
}

// void list_files(char *directory) {
// struct dirent *di;
// DIR *dr = directory==NULL  ? opendir(".") : opendir(directory);
// if (dr == NULL) {
//  printf("could not open the directory / directory not present");
//  return;
// }
// while ((di = readdir(dr)) != NULL) {
//  puts(di->d_name);
// }
//}