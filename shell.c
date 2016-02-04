#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* prompt = ">";

int getinput(char* input) {
	printf("%s", prompt);
	int i = 0;
	char c;
	while((c = getchar()) != '\n')
		input[i++] = c;
	input[i] = '\0';
}

int parsecmd(char* cmd, char** arguments, int* argc) {
	int i = 0;
	int argindex = 0;
	int charpos = 0;
	while(cmd[i] != '\0') {
		charpos = 0;
		while(cmd[i] == ' ')
			i++;
		while(cmd[i] != ' ' && cmd[i] != '\0') {
			arguments[argindex][charpos++] = cmd[i];
			i++;
		}
		while(cmd[i] == ' ')
			i++;
		arguments[argindex][charpos] = '\0';	
		if(charpos)
			argindex++;	
	}	
	arguments[argindex] = NULL;	
	*argc = argindex;
}

int run(char* cmd) {
	char* arguments[10];
	int cpid;
	int i;
	int argc;
	int background = 0;
	for(i = 0; i < 10; i++)
		arguments[i] = malloc(sizeof(char) * 100);
	//parse command
	parsecmd(cmd, arguments, &argc);
	//deal with ampersand
	int length = strlen(arguments[argc-1]);
	if(arguments[argc-1][length-1] == '&') {
		arguments[argc-1][length-1] = '\0';
		if((length-1) == 0)
			arguments[argc-1] = NULL;
		background = 1;	
	}
	cpid = fork();
	//if child
	if(cpid == 0) {
		execvp(arguments[0], arguments);
		printf("Unknown command");
	}
	//if parent
	else 
		if(! background) 
			wait(cpid);
}

int main() {
	char input[100];
	getinput(input);
	while(strcmp(input, "exit")) {
		//run command
		if(*input)
			run(input);
		getinput(input);
	}	
}
