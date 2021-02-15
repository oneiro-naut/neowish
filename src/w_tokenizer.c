#include "../include/w_tokenizer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define INPUTSIZE 1000//sufficient length
#define CMDPERINPUT 10	//sufficient size of cmd queue array
#define PATHLEN 1000 
#define DIRSTCKSIZE 20  //
#define TOKSIZE 100


// this tokenizer handles and tokenizes double quotes
char** wish_tok(char* inp_str)
{
    char** tokarr =(char**) malloc(sizeof(char*)*CMDPERINPUT);

	char *p=inp_str;
	if(p==NULL){ tokarr[0]=NULL; return tokarr;}
	char* start_of_word=NULL;
	int index_of_token= -1;
	int c;
	enum states { DULL, ALT_DULL, IN_WORD, IN_STRINGD, IN_STRING, ERROR, EXIT } state = DULL;

		for(;state!=EXIT && state!=ERROR;p++) {
			c = (unsigned char) *p; /* convert to unsigned char for is* functions */
			switch (state) {
			case DULL: /* not in a word, not in a double quoted string */
				if(c==0){ state=EXIT; continue;}
				if (c==' ') {
					/* still not in a word, so ignore this char */
					continue;
				}
				/* not a space -- if it's a double quote we go to IN_STRING, else to IN_WORD */
				if (c == '"') {
					state = IN_STRINGD;
					start_of_word = p + 1; /* word starts at *next* char, not this one */
					index_of_token++;
					continue;
				}
				if (c == '\'') {
					state = IN_STRING;
					start_of_word = p + 1; /* word starts at *next* char, not this one */
					index_of_token++;
					continue;
				}
				state = IN_WORD;
				start_of_word = p; /* word starts here */
				index_of_token++;
				continue;

			case ALT_DULL: /* not in a word, not in a double quoted string */
				if(c==0){ state=EXIT; continue;}
				if (c==' ') {
					/* still not in a word, so ignore this char */
					state=DULL;
					continue;
				}
				/* not a space -- if it's a double quote we go to IN_STRING, else to IN_WORD */
				if (c == '"') {
					state = IN_STRINGD;
					start_of_word = p + 1; /* word starts at *next* char, not this one */
					index_of_token++;
					continue;
				}
				if (c == '\'') {
					state = IN_STRING;
					start_of_word = p + 1; /* word starts at *next* char, not this one */
					index_of_token++;
					continue;
				}
				state = ERROR;
				continue;


			case IN_STRINGD:
				/* we're in a double quoted string, so keep going until we hit a close " */
				
				if(c==0){ state = ERROR;  }
				if (c == '"') {
					/* word goes from start_of_word to p-1 */
					*p= 0;
					tokarr[index_of_token]=start_of_word;
					state = ALT_DULL; /* back to "not in word, not in string" state */
				}
				continue; /* either still IN_STRING or we handled the end above */

			case IN_STRING:
				/* we're in a single quoted string, so keep going until we hit a close " */
				
				if(c==0){ state = ERROR;  }
				if (c == '\'') {
					/* word goes from start_of_word to p-1 */
					*p= 0;
					tokarr[index_of_token]=start_of_word;
					state = ALT_DULL; /* back to "not in word, not in string" state */
				}
				continue; /* either still IN_STRING or we handled the end above */



			case IN_WORD:
				/* we're in a word, so keep going until we get to a space */
				if(c==0){ 
					state=EXIT;
					tokarr[index_of_token]=start_of_word;
					
					}
				if (c==' ') {
					/* word goes from start_of_word to p-1 */
					
					state = DULL; /* back to "not in word, not in string" state */
					*p= '\0';
					
					tokarr[index_of_token]=start_of_word;
				}
				if(c=='\'' || c=='"'){ state=ERROR;}

				continue; /* either still IN_WORD or we handled the end above */
			

			}
		}
		//2 possible final states of our automata
		if(state==EXIT)tokarr[++index_of_token]=NULL;  //case EXIT:
		else if(state==ERROR)
		{
			printf("input error related to quotes occured\n"); //case ERROR: 
			tokarr[++index_of_token]=NULL;
		}
		    

 	//  int i=0;
 	//  printf("\n");
 	//  for(;tokarr[i]!=NULL;i++)puts(tokarr[i]); //for debugging purpose
    //   if(!tokarr[i])printf("NULL ");
 	//  printf("\n");			
    
	return tokarr;
}


//tokenizer below does not tokenize the delimitor if its inside a balanced double quoted substring
//more delimitors can be added by adding more if conditions 
char** wish_tok_quote_muted(char* inp_str) 
{
    
    //if(*inp_str==0)return NULL;
    char** tokarr =(char**) malloc(sizeof(char*)*CMDPERINPUT);
	if(inp_str==NULL){ tokarr[0]=NULL; return tokarr;}
    char *p=strdup(inp_str);//so that original buffer is not affected
	
	
	char* start_of_word=NULL;
	int index_of_token= -1;
	int c;
	enum states { DULL, IN_WORD, IN_STRING, IN_STRINGD, EXIT, ERROR } state = DULL;

		for (;state!=EXIT && state!=ERROR;p++) {
			c = (unsigned char) *p; /* convert to unsigned char for is* functions */
			switch (state) {
			case DULL: /* not in a word, not in a double quoted string */
				if(c==0){ state=EXIT; continue;}
				if (c==';') {
					/* still not in a word, so ignore this char */
					continue;
				}
				/* not a space -- if it's a double quote we go to IN_STRING, else to IN_WORD */
				if (c == '"') {
					state = IN_STRING;
					//start_of_word = p + 1; /* word starts at *next* char, not this one */
					//index_of_token++;
					continue;
				}
				state = IN_WORD;
				start_of_word = p; /* word starts here */
				index_of_token++;
				continue;

			case IN_STRINGD:
				/* we're in a double quoted string, so keep going until we hit a close " */
				if(c==0){ 
					state=ERROR;
					tokarr[index_of_token]=start_of_word;
					
					}				
				if (c == '"') {
					/* word goes from start_of_word to p-1 */
					// unlike the prev version this IN_STRING state does nothing just skips tokenizing process
					//until it finds that its finally going to next non STRING state
					state = IN_WORD; /* back to "not in word, not in string" state */
				}
				continue; /* either still IN_STRING or we handled the end above */

			case IN_STRING:
				/* we're in a double quoted string, so keep going until we hit a close " */
				if(c==0){ 
					state=ERROR;
					tokarr[index_of_token]=start_of_word;
					
					}
				if (c == '\'') {
					/* word goes from start_of_word to p-1 */
					// unlike the prev version this IN_STRING state does nothing just skips tokenizing process
					//until it finds that its finally going to next non STRING state
					state = IN_WORD; /* back to "not in word, not in string" state */
				}
				continue; /* either still IN_STRING or we handled the end above */
			case IN_WORD:
				/* we're in a word, so keep going until we get to a space or our delimiter char */
				if(c==0){ 
					state=EXIT;
					tokarr[index_of_token]=start_of_word;
					
					}				
				if (c==';') {
					
					
					state = DULL; 
					*p= '\0';
					
					tokarr[index_of_token]=start_of_word;
				}
				if(c=='"')
				{
					state=IN_STRINGD;

				}
				if(c=='\'')
				{
					state=IN_STRING;
				}
				continue; /* either still IN_WORD or we handled the end above */

			}
		}
		//exit state check here sort of pythonic while-else construct
		if(state==EXIT)tokarr[++index_of_token]=NULL;  //case EXIT:
		else if(state==ERROR)
		{
			printf("input error related to quotes occured\n"); //case ERROR: 
			tokarr[++index_of_token]=NULL;
		}    
	return tokarr;
}

//tokenizer returns argv now**
//dependency on this tokenizer is to be removed as it just sucks 
//tokenization of double quotes can now be turned off
// *****cases like echo "some;thing" remain unhandled***
char **w_tokenizer(char* inp_str, const char* deli, int dquote) { 
  char** tokarr =(char**) malloc(sizeof(char*)*CMDPERINPUT);
  int length =strlen(inp_str);
  char* curr=inp_str;
  char* temp;
  char* tok;
  if(curr==NULL){ tokarr[0]=NULL; return tokarr;}
  int index=-1;
  int err=-1;
	do {
		while (*(curr) == ' ') { 
			curr++;
		}
		if (*(curr) != '\"') {
			if (index==-1) tok = strtok_r(inp_str, deli, &curr);
			else tok = strtok_r(NULL, deli, &curr);
			index++;
			tokarr[index] = tok;
		}
		else {
      temp = curr + 1;
      do {
				curr++;
			} while ((*curr) != '\"' && (*curr) != '\0');
			index++;
			if (*curr == '\"' && dquote) {
				*curr='\0';
				tokarr[index] = temp;
				curr++;
			}
			else if (*curr == '\"' && dquote == 0) {
        curr++; //dont tokenize just skip tokenizing whats between the quotes
      }
      else {
				err++;
				tokarr[index] = NULL;
			}
		}
	} while (tokarr[index]);  
  return tokarr;
}