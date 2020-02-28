#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(){
	char *cmd = "ls";
	char *args[] = {"","-l", NULL };
	int out = 0;
	char *path = "out.txt";

	out = open(path, (O_RDWR | O_CREAT), 0600);
	if(out == -1)
	   out = open(path, O_RDWR);	
	if(out == -1)
		exit(-1);

	if( fork() == 0 ){
		
		dup2(out,fileno(stdout));
		execvp(cmd, args);
		puts("error");
		exit(1);
	}else{
		wait(0);
	}





}
