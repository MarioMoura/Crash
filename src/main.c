#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


#include "array.h"
#include "cmd.h"
#include "shell.h"


// TODO:
//
//      - Indirection Function
//      - PATH alteration
//      - semicolon syntax
//      - Signal handlers
//      - Background Functions ?
//

int main(void){

	struct Cmd cmd;

	char cwd[1024];  
	while(1){

		char *prompt = sh_get_prompt();
		char *input = readline( prompt );
		free( prompt );

		cmd_init( &cmd , input );

		if(cmd.cmd){
			if(!strcmp(cmd.cmd->array[0],"exit")){
				cmd_finish( &cmd );
				free( input );
				return 0;
			}
			if(!strcmp(cmd.cmd->array[0],"cd")){
				sh_chdir( cmd.cmd );
				cmd_finish( &cmd );
				free( input );
				continue;
			}

			sh_exec_cmd( &cmd );

		}

		cmd_finish( &cmd );
		free( input );

	}
	return 0;
}
