#ifndef _cmd_INCLUDED
#define _cmd_INCLUDED

/* 
 * Cmd consists of an array of
 * Cmd_Arrays together with its
 * number of pipes
 */
struct Cmd {
	unsigned int npipes;
	struct Cmd_Array *cmd;
};

int cmd_init( struct Cmd **cmd , char *input );
int cmd_finish( struct Cmd **cmd );
#endif
