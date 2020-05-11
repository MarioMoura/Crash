
#ifndef _array_INCLUDED
#define _array_INCLUDED

/* 
 * Every Cmd_Array is an array of strings
 * ending with a NULL string, ready to be 
 * passed to execv functions.
 *
 */
struct Cmd_Array {
	char *input;     // Usefull only in the building process
	size_t array_wc; // Usefull only in the building process
	char **array;    // array of strings
	int fd_out;      // Output filedescriptor
	int fd_in;       // Input filedescriptor
};

int arr_init(struct Cmd_Array *cmd, char *input);
int arr_show_stats(struct Cmd_Array *cmd);
void arr_finish(struct Cmd_Array *cmd);
int arr_print(struct Cmd_Array *cmd);
#endif
