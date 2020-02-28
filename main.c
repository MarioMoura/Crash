// A simple, educational program of how a shell
// (more or less) works


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define ARGSIZE 32
// TODO dynamic buffersize

// helper debug function
void printargs(char **args){
	for(int i=0; args[i] != NULL; i++)
		puts(args[i]);
}
// transforms buf into a more readable form
// much like argv array from main
void toarray(char *buf, char  *args[]){
	memset(args, 0, ARGSIZE);
	char inword = 0;
	char wordc = 0;
	int len = strlen(buf);
	//printf("%d\n", len);
	for(int i=0; i < len; i++){
		switch(buf[i]){
			case ' ' :
				inword = 0;
				buf[i] = 0;
				break;
			case '>' :
				inword = 0;
				buf[i] = 0;
				args[wordc++] = ">";	
				break;
			case '|' :
				inword = 0;
				buf[i] = 0;
				args[wordc++] = "|";	
				break;
			default :
				if(inword == 0)
					args[wordc++] = &(buf[i]);	
				inword = 1;
				break;
			
		}
	}
}
// read args between the pipes n and n+1  and returns another
// array the formated command as well as the prefered output
char **get_command(char **array, int n, char **out_f){
	int npipes = 0;
	int size = 1;
	int cmdc = 0;
	char **cmd;
	for(int	i=0; array[i] != NULL; i++){// get size for malloc
		if(!strcmp(array[i],"|"))
			npipes++;
		else
			if(npipes == n)
				size++;
	}
	cmd = malloc( size * (sizeof (char*))); // allocate size	
	npipes = 0;
	for(int i=0; array[i] != NULL; i++){ // fill the array
		if(!strcmp(array[i],"|")) // if arg[i] == "|"
			npipes++;
		else
			if(npipes == n){
				if(array[i][0] == '>'){
					if(array[i + 1] != NULL){
						*out_f = array[i + 1];
					}else
						exit(1);
					break;		
				}
				cmd[cmdc++] = array[i];
			}
	}
	cmd[size-1] = NULL;
	return cmd;	
}
// fork and execute the program arg[0] with the
// arguments args[1...n] having "in" as stdin and
// "out" as stdout
void shell(char *args[], int in, int out){
	// TODO output redirect
		if(fork() == 0){
			// child code
			execvp(*args,args);
			puts("execve error");
			exit(0);
		}else{
			// parent code
			wait(0);
		}	
}
int main(void){
	char buf[256];	// stdin buffer
	char *array[ARGSIZE] = {0}; // arguments for the program	
	char *prompt = "shell>>";
	char **args;

	char *out_f = 0;
	int out = fileno(stdout);

	char *in_f;
	int in = fileno(stdin);

	while(1){ // begin of the shell loop
		printf("%s",prompt); // print the prompt
		if(scanf(" %[^\n]", buf) != 1) // input
			exit(1);

		toarray(buf, array);

		if(!strcmp(array[0],"exit")) // exits is "exit" is typed
			exit(0);

		args = get_command(array, 0, &out_f);

		shell(args, 0,0);
		
		free(args);

	}
	return 0;
}
