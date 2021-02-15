#ifndef W_PARSER_H
#define W_PARSER_H

#include "cmd_struct.h"

static int checkTokens(char **argv);

static int countPipes(char * str,int* fileoptflag);// can be implemented using a FSM

cmd_struct* parse_input(char* stream);
cmd_struct* new_parser(char *stream);

#endif