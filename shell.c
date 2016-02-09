#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include "list.h"
#include "alias.h"

char* prompt = ">";
node_t* backgroundTasks = NULL;
int foregroundTask = 0;

void intHandler(int sig) {
	int pid;
	if(sig == SIGINT) {
		if(foregroundTask)
			kill(foregroundTask, 9);
		while((pid = pop(&backgroundTasks)) != -1)
			kill(pid, 9);
	}
}

int getinput(char* input) {
	int i = 0, c;
	printf("%s", prompt);
	while((c = getchar()) != EOF) {
		if(c == '\n') {
			input[i] = '\0';
			return 1;
		}
		input[i++] = c;
	}
	return 0;
}

void parsecmd(char* cmd, char** arguments, int* background) {
	int i = 0, argindex = 0, charpos = 0, length = 0;
	char buffer[100];
	while(cmd[i] != '\0') {
		charpos = 0;
		while(cmd[i] == ' ')
			i++;
		while(cmd[i] != ' ' && cmd[i] != '\0') {
			buffer[charpos++] = cmd[i];
			i++;
		}
		while(cmd[i] == ' ')
			i++;
		buffer[charpos] = '\0';
		if(charpos) {
			arguments[argindex] = malloc((charpos+1)*sizeof(char));
			strcpy(arguments[argindex++], buffer);
		}
	}
	arguments[argindex] = NULL;
	//deal with ampersand
	*background = 0;
	if(argindex > 0) {
		length = strlen(arguments[argindex-1]);
		if(arguments[argindex-1][length-1] == '&') {
			arguments[argindex-1][length-1] = '\0';
			//if last argumnet only contained ampersand delete it completely
			if((length-1) == 0) {
				free(arguments[argindex-1]);
				arguments[argindex-1] = NULL;
			}
			*background = 1;
			}
		}
}

void run(char** arguments, int background) {
	int cpid, status;
	if(arguments[0]) {
		cpid = fork();
		//if child
		if(cpid == 0) {
					execvp(arguments[0], arguments);
					printf("Unknown command\n");
					exit(1);
		}
		//if parent
		else
			if(! background) {
				foregroundTask = cpid;
				waitpid(cpid, &status, 0);
				foregroundTask = 0;
			} else {
				push(&backgroundTasks, cpid);
			}
		}
}

int main() {
	char input[100];
	int terminated, status, background = 0, i, j;
	char *arguments[10], *temp[10];
	char** alias;

	signal(SIGINT, intHandler);

	while(getinput(input)) {
		parsecmd(input, arguments, &background);
		if(arguments[0]) {
			alias = getCommandForAlias(arguments[0]);
			if(alias) {
				while(alias[i])
					printf("%s\n", alias[i++]);
				free(arguments[0]);
				//save original argumetns
				i = 0;
				while (arguments[i+1]) {
					temp[i] = arguments[i+1];
					i++;
				}
				temp[i]=NULL;
				//put in alias and arguments
				i = 0;
				while(alias[i]) {
					arguments[i] = malloc(strlen(alias[i]) * sizeof(char));
					strcpy(arguments[i], alias[i]);
					i++;
				}
				//append original arguments
				j = 0;
				while(temp[j]) {
					arguments[i+j] = temp[j];
					j++;
				}
				arguments[i+j] = NULL;
			}
			if(strcmp(arguments[0], "exit") == 0) {
				break;
			}
			else if(strcmp(arguments[0], "alias") == 0) {
				if(! arguments[1] || ! arguments[2])
					printf("%s\n", "usage: alias alias command");
				else
					add_alias(arguments[1], arguments+2);
			} else {
				run(arguments, background);
			}
		}
		//clean up arguments array
		i = 0;
		while(arguments[i]) {
			free(arguments[i]);
			arguments[i++] = NULL;
		}
		//check if any background processes are done
		while((terminated = waitpid(-1, &status, WNOHANG)) > 0) {
			printf("\nbackground process %d terminated\n", terminated);
			remove_by_value(&backgroundTasks, terminated);
		}
	}
	printf("%s\n", "");
}
