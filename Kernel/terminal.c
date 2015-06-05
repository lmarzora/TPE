#include <naiveConsole.h>
#include <time.h>
char buffer[80];
int index = 0;
int done = 0;
int ticker = 0;

void incTicker(){
	ticker++;
	if(ticker == 200){
		ncPrint("LLegue a 200");
		ticker = 0;
	}
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
	if(index)
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
	ncNewline();
	return 0;
}
void sysGetTime() {
	printTime();
}
