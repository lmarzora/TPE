#include <lib.h>
#include <syscalls.h>

static char buff[80];

void printLn(char* line) {
	write(line,80);
}
char* getLn() {
	read(buff,80);
	return buff;
}
int cmpstr(char * s1, char * s2){
	int i = 0;
	while(s1[i] == s2[i] && s1[i] != 0)
		i++;
	return s1[i] == 0 && s2[i] == 0;
}

void getTime(){
	timeGet();
}
