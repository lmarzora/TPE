#include <lib.h>
#include <syscalls.h>

static char buff[80];


void print(char * line){
	write(line,80);
}

void printLn(char* line) {
	print(line);
	print("\n");
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

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}


void getTime(){
	timeGet();
}

void setTime(char* time){
	timeSet(time,8);
}

void clearScreen(){
	screenClear();
}

void listProcesses(){
	processesList();
}

void setInterval(int num){
	intervalSet("", num);
}
