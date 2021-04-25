#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "cmd.h"
#include "util.h"



/*
 * Print the array of arrays
 *
 * Used for debugging
 *
 */
static int cmd_print_arrays( struct Cmd *cmd ){

	for(int i=0; i < cmd->npipes+1;i++){
		arr_print( cmd->cmd + i );
	}
}

/*
 * Simply count the number of pipes of
 * the given string
 *
 */
static unsigned long int cmd_count_pipes( char *input ){
	unsigned long int count = 0;
	unsigned long int index = 0;
	while( input[ index++ ] != '\0')
		if( input[ index ] == '|' )
			count++;
	return count;
}
static unsigned long int cmd_count_semicolons( char *input ){
	unsigned long int count = 0;
	unsigned long int index = 0;
	while( input[ index++ ] != '\0')
		if( input[ index ] == ';' )
			count++;
	return count;
}
/*
 * Check if the one of the pipe
 * members is empty
 *
 */
static int check_pipes( char *input ){
	char *str = strdup(input);
	char *free_p = str;
	char *ptr;
	while( ptr = strsep( &str, "|")){
		if(is_null(ptr)){
			free( free_p );
			return 0;
		}
	}
	free( free_p );
	return 1;
}

/*
 * Initialize a Cmd struct.
 *
 * First check syntax, null string or pipe errors
 *
 * Then allocate spaces for the arrays.
 *
 * Finally build the arrays.
 *
 */
int cmd_init_internal( struct Cmd *cmd , char *input ){

	char *lcl_input;
	char *lcl_input_free;
	char *arr_ptr;

	unsigned int index = 0;

	cmd->cmd = NULL;	// A good measure against
						// seg faults

	if( is_null( input ))
		return 1;
	if( !check_pipes( input ) ){
		puts("Pipe Syntax error");
		return 0;
	}
	cmd->npipes = cmd_count_pipes( input );
	cmd->cmd = malloc( sizeof(struct Cmd_Array) * (cmd->npipes + 1));

	lcl_input = strdup( input );
	lcl_input_free = lcl_input ;

	while( arr_ptr = strsep( &lcl_input, "|"))
		arr_init( cmd->cmd + index++ , arr_ptr );


	free( lcl_input_free );
	return 1;
}
int cmd_init( struct Cmd **cmd , char *input ){

	char *lcl_input;
	char *lcl_input_free;
	char *arr_ptr;

	unsigned int index = 0;

	if( is_null( input ))
		return 1;

	lcl_input = strdup( input );
	lcl_input_free = lcl_input ;

	unsigned int n_semicolons = cmd_count_semicolons(input);
	*cmd = malloc( sizeof(struct Cmd) * (n_semicolons + 1 + 1));

	cmd[n_semicolons+1] = NULL;


	/*cmd_init_internal( cmd[0] , "eae man");*/
	/*cmd_print_arrays(cmd[0]);*/
	while( arr_ptr = strsep( &lcl_input, ";")){
		printf("%d\n", index);
		cmd_init_internal( *cmd + index++ , arr_ptr );
	}
	index = 0;
	while( cmd[index] != NULL)
		cmd_print_arrays(*cmd+index++);
	puts("-teste-");
	for(int i = 0; i < n_semicolons + 1; i++){
		cmd_print_arrays(*cmd+i);
	}

	free( lcl_input_free );
}

/*
 * Must be called after each cmd_init call.
 *
 * Cleans a Cmd struct freeing the memory.
 *
 */
int cmd_finish_single( struct Cmd *cmd ){
	if(cmd->cmd == NULL)
		return 1;
	for(int i =0; i < cmd->npipes+1 ; i++){
		arr_finish(cmd->cmd+i);
	}
	free(cmd->cmd);
	return 1;
}
int cmd_finish( struct Cmd **cmd ){
	int index = 0;
	while( cmd[index] != NULL)
		cmd_finish_single(*cmd+index++);
}
