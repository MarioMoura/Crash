#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  

#include "array.h"
#include "cmd.h"



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
	while( input[ index++ ] != '\0'){
		if( input[ index ] == '|' )
			count++;
	}
	return count;
}

/* 
 * Simply check is a string is either
 * filled with white spaces or just 
 * empty
 *
 */
static int is_null( char *str){
	int sum = 0;
	int i = -1;
	while(str[++i] != '\0')
		sum += (str[i] - ' ' ? 1 : 0); 
	return !sum;
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
int cmd_init( struct Cmd *cmd , char *input ){

	char *lcl_input;
	char *lcl_input_free;
	char *arr_ptr;

	unsigned int index = 0;
	
	cmd->cmd = NULL; // A good measure against
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

/*
 * Must be called after each cmd_init call.
 *
 * Cleans a Cmd struct freeing the memory.
 *
 */
int cmd_finish( struct Cmd *cmd ){
	if(cmd->cmd == NULL)
		return 1;
	for(int i =0; i < cmd->npipes+1 ; i++){
		arr_finish(cmd->cmd+i);
	}
	free(cmd->cmd);
	return 1;
}
