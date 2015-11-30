#include <naiveConsole.h>
#include <screensaver.h>
#include <terminal.h>
#include <tempfiles.h>
#include <semaphore.h>
#include <msgqueue.h>
#include "./scheduler/scheduler.h"

char buffer[80];
int index = 0;
int done = 0;
int ticker = 0;
int interval = 1000;

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

char * estado(int i){
	switch(i){
		case 0: return "Ready";
		case 1: return "Blocked";
		case 2: return "Running";
		case 3: return "Terminated";
	}

	return "Desconocido";
}

void printProcesses(){
	int i=0;
	int num_processes = numProcesses();
	ncPrint("\n-> Numero Procesos: ");
	ncPrintDec(num_processes);
	ncPrint("\n\n");
	ncPrintCol("Nombre", 20);
	ncPrintCol("PID", 10);
	ncPrintCol("Estado", 10);
	ncNewline();
	ncPrintCol("------", 20);
	ncPrintCol("---", 10);
	ncPrintCol("------", 10);
	ncNewline();
	Process * aux = getProcessList();
	for(i=0; i<num_processes; i++){
		ncPrintCol(aux->name, 20);
		ncPrintDecCol(aux->id, 10);
		ncPrintCol(estado(aux->state), 10);
		ncNewline();
		aux = aux->list_next;
	}
	ncNewline();
}

void printIpcs(){
	SemaphoreList * sl = getSemaphoreList();
	MsgQueueList * mql = getMsgQueueList();

	ncPrintCol("Tipo", 5);
	ncPrintCol("Nombre", 20);
	ncPrintCol("Valor", 5);
	ncNewline();
	ncPrintCol("----", 5);
	ncPrintCol("------", 20);
	ncPrintCol("-----", 5);
	ncNewline();

	while(sl){
		ncPrintCol("Sem", 5);
		ncPrintCol(sl->semaphore->name, 20);
		ncPrintDecCol(ValueSem(sl->semaphore), 5);
		ncNewline();
		sl = sl->next;
	}
	while(mql){
		ncPrintCol("MsgQ", 5);
		ncPrintCol(mql->mq->name, 20);
		ncPrintCol("  -  ", 5);
		ncNewline();
		mql = mql->next;
	}
	ncNewline();
}