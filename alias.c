#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "alias.h"

char* aliases[100];
char* commands[100][10];

void add_alias(char* alias, char** command) {
  int i = 0, j = 0;
  while(aliases[i]) {
    if(strcmp(aliases[i], alias) == 0) {
      j=0;
      while(commands[i][j])
        free(commands[i][j++]);
      //copy command
      j=0;
      while (command[j]) {
        commands[i][j] = malloc(strlen(command[j]) * sizeof(char));
        strcpy(commands[i][j], command[j]);
        j++;
      }
      commands[i][j] = NULL;
      return;
    }
    i++;
  }
  aliases[i] = malloc(strlen(alias) * sizeof(char));
  strcpy(aliases[i], alias);
  //copy command
  j=0;
  while (command[j]) {
    commands[i][j] = malloc(strlen(command[j]) * sizeof(char));
    strcpy(commands[i][j], command[j]);
    j++;
  }
  commands[i][j] = NULL;
}

char** getCommandForAlias(char* alias) {
  int i = 0;
  while(aliases[i]) {
    if(strcmp(aliases[i], alias) == 0) {
      return commands[i];
    }
    i++;
  }
  return NULL;
}
