#include <naiveConsole.h>
#include <screensaver.h>
#include <terminal.h>

char buffer[80];
int index = 0;
int done = 0;
int ticker = 0;
int interval = 270;

//18 ticks = aprox 1 segundo
void incTick(){
	ticker++;
	if(ticker%30 == 0){
		toggleScreenSaver();
	}
	if(ticker == interval){
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
	}

}

void clearScreen(){
	ncClear();
}

void enter(){
	ncNewline();
	done = 1;
}

void insertKey(char key){
	ncPrintKey(key);

	if(index != 77)
		buffer[index++] = key;
	
}

void upArrow(){
	scrollUp();
}

void downArrow(){
	scrollDown();
}


void copyBuffer(char * buff){
 	while (!done);

 	int i = 0;

	while (i<index) {
		buff[i] = buffer[i];
		i++;
	}

	buff[i] = 0;

	index = 0;
	done = 0;

 }

 void timePrint(int hora, int min, int sec){
 	if(hora < 0xA){
		ncPrintDec(0);
	}
	ncPrintHex(hora);
	ncPrintChar(':');

	if(min < 0xA){
		ncPrintDec(0);
	}
	ncPrintHex(min);
	ncPrintChar(':');

	if(sec < 0xA){
		ncPrintDec(0);
	}
	ncPrintHex(sec);
	
	ncNewline();
 }

 void printBuffer(char * buff){
 	ncPrint(buff);
 }

 void changeInterval(int num){
 	interval = num*18;
 }
