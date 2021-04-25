#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <string.h>
#include <stdlib.h>


#include "array.h"
#include "cmd.h"

#define READ 0
#define WRITE 1

/*
 * Return an malloc`d string thas is a prompt
 *
 */
char *sh_get_prompt(){
	char *prompt;
	char cwd[1024];
	char *dirname;

	struct utsname uts;
	char *hname;

	char *usrname;

	getcwd( cwd, sizeof(cwd));
	dirname = strrchr( cwd, '/' ) + 1 ;

	uname(&uts);
	hname = uts.nodename;

	usrname = getlogin();

	prompt = malloc( 15 +
			strlen(usrname) +
			strlen(hname) +
			strlen(dirname));
	sprintf(prompt, "[ Crash %s@%s %s]$ ",usrname,hname,dirname);

	return prompt;
}

/*
 * First check the existance of the path
 * then change the CWD
 *
 */
int sh_chdir( struct Cmd_Array *cmd_arr ){
	char *path = cmd_arr->array[1];
	if( path == NULL ){ // check if path is null
		puts("Error cd: missing operand !");
		return 0;
	}
	if(access( path, F_OK)){
		printf("Error cd: \"%s\" No such directory\n",path);
		return 0;
	}
	chdir( path );
	return 1;
}

/*
 * Return a malloc'd string that is a pathname to
 * an existing executable file whithin the PATH
 *
 */
char *sh_get_prog( struct Cmd_Array *cmd_arr ){
	char *base_path = strdup(BASE_PATH);
	char *base_path_free = base_path;
	char *ptr;
	char *ret = NULL;

	while( ptr = strsep( &base_path, ":")){
		ret = malloc(
				strlen(ptr) +  	// path
				1 +				// slash
				strlen(cmd_arr->array[0]) + // progname
				1 );			// null byte
		sprintf(ret,"%s%s%s",ptr,"/",cmd_arr->array[0]);
		if( access(ret, X_OK | F_OK ) == 0 ){
			free( base_path_free );
			return ret;
		}
		free(ret);
	}
	free( base_path_free );
	return NULL;
}

/*
 * Read a Cmd_Array, getting the command within the
 * path and executing it with the apropriate args
 *
 * -Fork
 * -Set File descriptors
 * -call execve
 *
 */
int sh_exec_arr( struct Cmd_Array *cmd_arr ){
	int ret_fork;
	char *prog = sh_get_prog( cmd_arr );
	if(prog == NULL){
		printf("Program \"%s\" not found !\n",cmd_arr->array[0]);
		return 0;
	}
	if((ret_fork = fork()) == -1)
		return 0;
	if(ret_fork == 0){
		// Child Code
		if( cmd_arr->fd_in != 0){
			dup2(cmd_arr->fd_in, 0);
		}
		if( cmd_arr->fd_out != 1){
			dup2(cmd_arr->fd_out, 1);
		}
		execv(prog,cmd_arr->array);
		puts("execve error");
		return 0;
	}else{
		// Parent Code
		wait(0);
		free(prog);
		return 1;
	}
}

/*
 * Read an Cmd struct executing it.
 *
 * Run through the array of Cmd_Arrays
 * executing them, taking in consideration
 * the pipes.
 *
 */
int sh_exec_cmd_internal( struct Cmd *cmd ){
	int in_save  = dup(0);
	int out_save = dup(1);
	int pipe_fd[2];
	int in = 0;

	unsigned int loop = cmd->npipes;

	for(int i=0; i < loop; i++){
		pipe(pipe_fd);

		(cmd->cmd + i)->fd_in = in ;
		(cmd->cmd + i)->fd_out = pipe_fd[WRITE] ;
		sh_exec_arr( cmd->cmd + i );

		close(pipe_fd[WRITE]);
		in = pipe_fd[READ];
	}
	if(in != 0)
		(cmd->cmd + loop)->fd_in = in ;
	sh_exec_arr( cmd->cmd + loop );

	dup2(in_save, 0);
	dup2(out_save, 1);
	return 1;
}
int sh_exec_cmd( struct Cmd **cmd ){
	int index = 0;
	while( cmd[index] != NULL)
		sh_exec_cmd_internal(*cmd+index++);

}



