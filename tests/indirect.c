#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(){
	char *cmd = "sed";
	char *args[] = {"","", NULL };
	int in = 0;
	char *path = "out.txt";

	in = open(path, (O_RDONLY ));
	if(in == -1)
		exit(10);

	if( fork() == 0 ){
		
		dup2(in,fileno(stdin));
		execvp(cmd, args);
		puts("error");
		exit(1);
	}else{
		wait(0);
	}
}
