 
/* 
 * Simply check is a string is either
 * filled with white spaces or just 
 * empty
 */
int is_null( char *str){
	if(!str)
		return 1;
	unsigned int sum = 0;
	unsigned int i = 0;
	while(str[i] != '\0')
		sum += !!(str[i++] - ' '); 
	return !sum;
}
