#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#define READ  0
#define WRITE  1

int count_pipe(char **cmd){
	int npipe = 0;
	for(int i=0; cmd[i] != NULL; i++)
		if(!strcmp(cmd[i], "|"))
			npipe++;
	return npipe;
}

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
// ls -l | sort | wc -l
int main(int argc, char **argv){
	int p[2];
	int ncmd = 3;
	char *cmd[] = { "ls", "-l","|", "|",  "|", "wc", 0};

	printf("%d\n", count_pipe(cmd));


	return 0;
}

