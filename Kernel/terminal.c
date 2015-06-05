#include <naiveConsole.h>
#include <screensaver.h>
#include <time.h>
#include <terminal.h>

#define CHARTOINT(a) (int) (a - '0')

char buffer[80];
int index = 0;
int done = 0;
int ticker = 0;

//200 ticks = aprox 10 segundos
void incTick(){
	ticker++;
	if(ticker == 200){
		activateSS();
	}
}

void resetTick(){
	deactivateSS();
	ticker = 0;
}

void backspace(){
	erase();
	
	if(index){
		index--;
		//buffer[index] = 0;
	}

}

void enter(){
	ncNewline();
	done = 1;
}

void insertKey(char key){
	ncPrintKey(key);

	if(index != 79)
		buffer[index++] = key;
	
}

void upArrow(){
	scrollUp();
}

void downArrow(){
	scrollDown();
}

void cleanBuffer(){
	/*int i;
	for(i=0; i<80; i++){
		buffer[i] = 0;
	}*/
	index = 0;
	done = 0;
}

int sysread(char* buff , int size) {

	while(!done);
	
	int i = 0;

	while (i<index) {
		buff[i] = buffer[i];
		i++;
	}

	buff[i] = 0;

	cleanBuffer();

	return i;
}

int syswrite(char* buff, int size) {
	ncPrint(buff);	
	//ncNewline();
	return 0;
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


