#include <stdio.h>
#include <sys/utsname.h>

int main(void){


	struct utsname buf;

	uname(&buf);
	
	puts(buf.sysname);
	puts(buf.nodename);
	puts(buf.release);	
	puts(buf.version);
	puts(buf.machine);

	return 0;
}
