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

	out = open(path, (O_WRONLY | O_CREAT|O_TRUNC), 0644);
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
