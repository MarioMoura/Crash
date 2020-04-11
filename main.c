/* A simple, educational program of how a shell
 * (more or less) works
 *
 * TODO: - Redict of output
 *       - Indirect of input
 *       - Prompt
*/

/*
==============================
= Includes
==============================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define ARGSIZE 32

/* definitions that help with the
   pipes filedescriptors
*/
#define READ 0
#define WRITE 1

#ifdef DEBUG
#endif

/*
==============================
= Functions
==============================
*/

/* used to print the array of
 * strings, generally used in
 * debug
 */
void printargs   (char **args){
	#ifdef DEBUG
	printf("Command: ");
	#endif
	for(int i=0; args[i] != NULL; i++)
		printf("%s ", args[i]);
	printf("\n");
}
/* transforms buf into a more readable form
 * much like argv array of main, ready to 
 * be passed to exec*
 */
void toarray(char *buf, char  *args[]){
	memset(args, 0, ARGSIZE);
	char inword = 0;
	char wordc = 0;
	int len = strlen(buf);
	for(int i=0; i < len; i++){
		switch(buf[i]){
			case ' ' :
				inword = 0;
				buf[i] = 0;
				break;
			case '\n' :
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
/* read args between the pipes n and n+1  and returns another
 * array the formated command as well as the prefered output
 */
char **parse_pipe(char **array, int n, char **out_f){
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
/* count the number of pipes in the command
 * generally the hole array
 */
int count_pipe(char **array){
	int npipe = 0;
	for(int i=0; array[i] != NULL; i++)
		if(!strcmp(array[i], "|"))
			npipe++;
	return npipe;
}
/* fork and execute the program arg[0] with the
 * arguments args[1...n] having "in" as stdin and
 * "out" as stdout
 * basically a helper function
 */
int shell_exec(char *args[], int in, int out){
	if(fork() == 0){
		if( in != fileno(stdin)){
			dup2(in, 0);
			close(in);
		}
		if( out != 1){
			dup2(out, 1);
			close(out);
		}
		execvp(*args,args);
		puts("execve error");
		exit(0);
	}else{
		wait(0);
		return 0;
	}	
}
/* This is the big boy, array is
 * the array of strings that is 
 * the command, and out_f is
 * a pointer to the file redirect
 * in case of one
 */
int run(char **array, char **out_f){
	int in_save = dup(0);
	int fd[2];
	int i, in;
	int loop = count_pipe(array);
	char **args;
	for( i=0; i < loop ; i++){
		pipe(fd);
		args = parse_pipe(array, i, out_f); // alloc
		shell_exec(args, in, fd[WRITE]);
		free(args);// free
		close(fd[WRITE]);
		in = fd[READ];
	}
	if(in != 0)
		dup2(in, 0);
	args = parse_pipe(array, loop , out_f);
	shell_exec(args, in, fileno(stdout));
	free(args);
	dup2(in_save, 0);
	return 0;
}
/*

 *
 */
#ifndef DEBUG
int main(void){
	char buf[256];// stdin buffer
	char *array[ARGSIZE] = {0}; // arguments for the program
	char *prompt = "protsh>> ";
	char **args;

	char *out_f = 0; // supposed to be path to the redirect file

	while(1){ // begin of the shell loop
		printf("%s",prompt); // print the prompt
		if(scanf(" %[^\n]", buf) != 1){ // input
			puts("input error");
			exit(1);
		}

		toarray(buf, array);

		if(!strcmp(array[0],"exit")) // exits is "exit" is typed
			exit(0);

		run( array, &out_f);
		
	}
	return 0;
}
#else
/* 
==============================
= Debug mode
==============================
*/
int main(int argc, char *argv[]){
	char buf[256];
	char *array[ARGSIZE] = {0};
	char *out_f = 0;
	FILE *ifile;

	puts("Single execution mode");

	if( argc == 1){
		strcpy(buf, "ls -lah");
	}else{
		printf("Opening \"%s\"...\n", argv[1] );
		if ( (ifile = fopen(argv[1],"r")) != NULL ){
			fread(buf, sizeof buf, 1, ifile); 
		}else{
			fprintf(stderr, "could not open file \"%s\"", argv[1]);
			return 1;
		}
	}
	toarray(buf, array);
	printargs(array);
	run(array, &out_f); 
	return 0;
}
#endif
