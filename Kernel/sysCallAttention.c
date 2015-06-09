#include <terminal.h>
#include <time.h>

#define CHARTOINT(a) (int) (a - '0')

int sysread(char* buff , int size) {
	
	copyBuffer(buff);

	return 1;
}

int syswrite(char* buff, int size) {	
	printBuffer(buff);
	return 1;
}

void sysGetTime() {
	printTime();
}

void sysSetTime(char* buff) {
	int hora =(CHARTOINT(buff[0]))*10 + CHARTOINT(buff[1]);
	int min =(CHARTOINT(buff[3]))*10 + CHARTOINT(buff[4]);
	int sec =(CHARTOINT(buff[6]))*10 + CHARTOINT(buff[7]);

	setTime(hora,min,sec);
}

void sysSetInterval(int num){
	changeInterval(num);
}