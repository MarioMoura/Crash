#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>

#define READ  0
#define WRITE  1


int main(int argc, char **argv){
	int p[2];
	int out[2];
	if(pipe(p) == -1){
		puts("error pipe");
		exit(1);
	}
	if(pipe(out) == -1){
		puts("error pipe");
		exit(1);
	}

	if(fork() == 0){
		puts("prog1");
		close(STDOUT_FILENO);
		dup2(p[WRITE], STDOUT_FILENO);
		//dup(p[1]);
		close(p[READ]);
		//close(p[WRITE]);
		char *prog1[] = { "echo", "", NULL };
		execvp(prog1[0], prog1);
	}
	if(fork() == 0){
		puts("prog 2");
		dup2(p[READ], STDIN_FILENO);
		dup2(out[WRITE], STDOUT_FILENO);
		//dup(p[0]);
		close(p[WRITE]);
		close(p[READ]);
		char *prog2[] = { "mydmenu", "-l", "10", "-w", "$WINDOWID", NULL };
		execvp(prog2[0], prog2);

		
	}
	puts("main prog");

	close(p[WRITE]);

	wait(0);
	wait(0);

	close(out[WRITE]);
	//close(out[READ]);

	//struct stat bf;
	//if(fstat(out[READ], &bf) == -1){
	//	puts("could not read last pipe");
	//	exit(-1);	
	//}
	//printf("%d\n", bf.st_size); 
	//
	//char buffer[20];
	//read(out[READ], buffer, 20);
	//puts("read");
	//puts(buffer);
	//
	FILE *f = fdopen(out[READ], "r");
	char buf[20];
	struct stat st;
	fstat( fileno(f), &st);
	fread(buf, 20, 1, f);
	puts("fread");
	puts(buf);
	printf("size: %d\n", st.st_size);
	return 0;
}

