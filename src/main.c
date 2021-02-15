#include "shell.h"

int main() {
  //  printf("Welcome to wish shell !\n");
  //a function to retreive hostname and user 
  wish_init(); //initialize globals
  //getprompt();
  shell_loop();
  return 0;
}
