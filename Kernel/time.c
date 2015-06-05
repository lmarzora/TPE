#include <naiveConsole.h>
#include <time.h>

void printTime(){
	int hora = getHora();
	int min = getMin();
	int sec = getSec();

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

int setTime(int hora, int min, int sec){
	if(hora<0 || hora>23 || min<0 || min>60 || sec<0 || sec > 60){
		return 0;
	}

	setHora(intToTime(hora));
	setMin(intToTime(min));
	setSec(intToTime(sec));
	
	return 1;
}

//transforma un numero tipo int de 2 cifras a time
uint8_t intToTime(int num) {
	uint8_t time;
	ncPrintDec(num);
	ncPrint(" to: ");
	time = (uint8_t) (((num/10 & 0xf) << 4) + (num%10 & 0xf));
	ncPrintHex(time);
	ncNewline();
	return time;

}

