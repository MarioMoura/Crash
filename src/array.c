#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "array.h"
#include "util.h"

// Syntax checking first !!!

// TODO:
//       the indirection function


/*
 * check append syntax
 *
 * return: 0 - Syntax Error
 *         1 - Normal Execution
 *         2 - No Symbol found
 */
int check_append( struct Cmd_Array *cmd ){
	char *input = cmd->input;
	char *lcl_input;
	char *lcl_input_free;
	char *ptr;
	if( strstr( input, ">>")){
		lcl_input = strdup( input );
		lcl_input_free = lcl_input;
		ptr = strstr( lcl_input, ">>" ) + 2;
		ptr = strtok( ptr, " " );
		if( !ptr ){
			free( lcl_input_free );
			return 0; // syntax error
		}
		if( access(ptr, F_OK) == 0)
			cmd->fd_out = open(ptr, (O_WRONLY | O_APPEND), 0644);
		else
			cmd->fd_out = open(ptr, (O_WRONLY | O_CREAT|O_TRUNC | O_APPEND), 0644);
		free( lcl_input_free );
	}else{
		return 2; // no symbol
	}
	return 1; // Syntax OK
}

/*
 * check Redirection syntax
 *
 * return: 0 - Syntax Error
 *         1 - Normal Execution
 *         2 - No Symbol found
 */
int check_redir( struct Cmd_Array *cmd ){
	char *input = cmd->input;
	char *lcl_input;
	char *lcl_input_free;
	char *ptr;
	if( strstr( input, ">")){
		lcl_input = strdup( input );
		lcl_input_free = lcl_input;
		ptr = strstr( lcl_input, ">" ) + 1;
		ptr = strtok( ptr, " " );
		if( !ptr ){
			free( lcl_input_free );
			return 0;
		}
		cmd->fd_out= open(ptr, (O_WRONLY | O_CREAT|O_TRUNC), 0644);
		free( lcl_input_free );
	}else{
		return 2;
	}
	return 1;
}
/*
 * Using the two functions above,
 * checks append and redirection
 * together
 *
 * return: 0 - Syntax Error
 *         1 - Normal Execution
 *         2 - No Symbol found
 */
int arr_parse_out( struct Cmd_Array *cmd ){
	int ret;
	ret = check_append( cmd );
	if(ret != 2)
		return ret;
	ret = check_redir( cmd );
	return ret;
}
/*
 * Count the number of words inside the
 * input field in a Cmd_Array struct
 *
 * this function should only be called after
 * the parsing of input and output redirection
 *
 */
static void arr_wc(struct Cmd_Array *cmd){
	char *str = cmd->input;
	char inword = 0;
	size_t count = 0;
	for(int i = 0; str[i] != 0; i++){
		switch(str[i]){
			case ' ':
				inword = 0;
				break;
			default:
				if(inword == 0){
					count++;
					inword = 1;
				}
				break;
		}
	}
	cmd->array_wc = count;
}
/*
 * First allocate an array of pointers to chars
 * terminanting with a NULL pointer as exec needs,
 * then allocate an string to each pointer with
 * strdup
 *
 * arr_cleanarray is used to revert this process
 *
 */
static void arr_mkarray(struct Cmd_Array *cmd){
	// allocate the number of words plus the NULL pointer in the end
	cmd->array = malloc( (cmd->array_wc + 1) * sizeof(char*) );
	cmd->array[ cmd->array_wc ] = NULL;
	char *input = cmd->input;
	char *str_ptr;
	unsigned int i = 0;

	str_ptr = strtok( input , " " );
	cmd->array[ i++ ] = strdup(str_ptr);
	while( (str_ptr = strtok( NULL ,  " ")) != NULL ){
		cmd->array[ i++ ] = strdup(str_ptr);
	}

	free( cmd->input );
}

/*
 * First frees each index of the array then
 * the array itself
 *
 */
static int arr_cleanarray(struct Cmd_Array *cmd){
	size_t i = 0;
	if( cmd->array == NULL)
		return 0;
	while( cmd->array[i] != NULL )
		free( cmd->array[i++]);
	free( cmd->array );
	return 1;
}
/*
 * if the array is not NULL print it
 *
 * Used for debug reasons
 */
int arr_print(struct Cmd_Array *cmd){
	if( cmd->array == NULL)
		return 0;
	size_t i = 0;
	puts("------------Array-------------");
	while( cmd->array[i] != NULL )
		puts( cmd->array[i++]);
	puts("------------Array-------------");
	return 1;
}

/*
 * if the array is not NULL print some stats
 *
 * Used for debug reasons
 */
int arr_show_stats(struct Cmd_Array *cmd){
	if( cmd->array == NULL)
		return 0;
	printf("arr_wc: %lu\n", cmd->array_wc);
	printf("fd_in: %d\n", cmd->fd_in);
	printf("fd_out: %d\n", cmd->fd_out);
}

/*
 * used to initialize the Cmd_Array struct.
 * - set in and out to default values
 * - set array to defualt value
 * - set input to the given string
 *
 * - parse the input append/redirection,
 *   removing that part of the string
 *
 * - if the string means something,
 *   builds the array
 *
 */
int arr_init(struct Cmd_Array *cmd, char *input){
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->input = strdup( input );
	cmd-> array = NULL; // crucial protection against
						// seg faults


	if( !arr_parse_out( cmd ) ){
		free( cmd->input );
		return 0;
	}

	// remove everything after these characters
	cmd->input = strsep( &(cmd->input), "<>");

	if(!is_null(cmd->input)){
		arr_wc       ( cmd );
		arr_mkarray  ( cmd );
	}else{
		free( cmd->input );
	}
	return 1;
}
/*
 * called after every arr_init
 */
void arr_finish(struct Cmd_Array *cmd){
		arr_cleanarray( cmd );
}
