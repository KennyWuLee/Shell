#include <string.h>
#include <stdlib.h>
#include "alias.h"

char* aliases[100];
char* commands[100];

void add_alias(char* alias, char* command) {
  int i = 0;
  while(aliases[i]) {
    if(strcmp(aliases[i], alias) == 0) {
      free(commands[i]);
      commands[i] = malloc(strlen(command) * sizeof(char));
      strcpy(commands[i], command);
      return;
    }
    i++;
  }
  aliases[i] = malloc(strlen(alias) * sizeof(char));
  strcpy(aliases[i], alias);
  commands[i] = malloc(strlen(command) * sizeof(char));
  strcpy(commands[i], command);
}

char* getCommandForAlias(char* alias) {
  int i = 0;
  while(aliases[i]) {
    if(strcmp(aliases[i], alias) == 0) {
      return commands[i];
    }
    i++;
  }
  return NULL;
}
