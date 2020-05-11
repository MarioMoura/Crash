#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

#define READ 0
#define WRITE 1

int main(int argc, char** argv){
	int des_p[2];
	if(pipe(des_p) == -1) {
		perror("Pipe failed");
		exit(1);
	}

	if(fork() == 0){            //first fork
		// close(STDOUT_FILENO);  //closing stdout
		dup2(des_p[WRITE], fileno(stdout)); //replacing stdout with pipe write 
		// dub2 stdout = des_p[1]
		// dup2(des_p[1], fileno(stdout)); 
		close(des_p[READ]);       //closing pipe read
	   	close(des_p[WRITE]);

		// -----------------------------
		char *prog1[] = { "ls", "-l", 0};
		execvp(prog1[0], prog1);

		perror("execvp of ls failed");
		exit(1);
	}

	if(fork() == 0){		   //creating 2nd child
		close(STDIN_FILENO);   //closing stdin
		dup2(des_p[0], fileno(stdin));  //replacing stdin with pipe read
		close(des_p[WRITE]);       //closing pipe write
		close(des_p[READ]);


		// -----------------------------
		char *prog2[] = { "wc", "-l", 0};
		execvp(prog2[0], prog2);

		perror("execvp of wc failed");
		exit(1);
	}

	close(des_p[0]);
	close(des_p[1]);
	wait(0);
	wait(0);
	return 0;
}
