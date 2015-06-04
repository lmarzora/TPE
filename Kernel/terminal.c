#include <naiveConsole.h>

char buffer[80] ;
int last = 0;
int next = 0;

int writeBuffer(char c) {
	buffer[next] = c;
	last= next;
	next++;
	return 1;
}

int sysread(char* buff , int size) {

	while( buffer[last]!='\n' && last < size) {
	}
	int i = 0;
	
	while (i<last) {
		buff[i] = buffer[i];
		i++;
	}
	buff[last] = 0;
	buffer[last] = 0;
	last = 0;
	next = 0;
	return i;
}

int syswrite(char* buff, int size) {
	//ncPrint("ffffffff");
	ncPrint(buff);	
	ncNewline();
	return 0;
}
