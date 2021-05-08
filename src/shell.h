
#ifndef _shell_INCLUDED
#define _shell_INCLUDED

#include "array.h"
#include "cmd.h"

char *sh_get_prompt();
int sh_chdir( struct Cmd_Array *cmd_arr );
int sh_exec_cmd( struct Cmd ***cmd );
int sh_exec_arr( struct Cmd_Array *cmd_arr );
int shell( struct Cmd *cmd, char *input);
#endif
