#include <naiveConsole.h>
#include <naiveConsole.h>


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
/*
int setTime(int hora, int min, int sec){
	if(hora<0 || hora>23 || min<0 || min>60 || sec<0 || sec > 60){
		return 0;
	}

	setHora(decToHexa(hora));
	setMin(decToHexa(min));
	setSec(decToHexa(sec));

	return 1;
}*/