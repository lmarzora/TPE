#include <lib.h>
#include<syscalls.h>

static char buff[80];

void printLn(char* line) {
	write(line,80);
}
char* getLn() {
	read(buff,80);
	return buff;
}