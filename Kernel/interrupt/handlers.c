#include <handlers.h>
#include <keyboard.h>
#include <terminal.h>
#include <naiveConsole.h>
#include <sysCallAttention.h>
#include <tempfiles.h>
#include <scheduler_interface.h>

static Semaphore * foreground_sem = 0;

void irqDispatcher(int64_t irq) {


	switch (irq) {
		case 0:
			incTick();
			break;
		case 1:	
			resetTick();
			analizeKeyboard(getKey());
			break;
		default:
			break;
	}
	return;
}

int syscall(int code , char* buff , int size) {
	int dim = -1;
	
	switch (code) {
		case 1:
			if(foreground_sem == 0){
				foreground_sem = CreateSem("foreground_sem", 0);
			}
			if(!isForeground()){
				WaitSem(foreground_sem);
				becomeForeground();
			}
			dim = sysread(buff,size);
			break;
		case 2:
			dim = syswrite(buff,size);
			break;
		case 3:
			sysGetTime();
			break;
		case 4:			
			sysSetTime(buff);
			break;
		case 5:
			clearScreen();
			break;
		case 6:
			sysSetInterval(size);
			break;
		case 7:
			ncPrintDec(size);
			getPages(size,buff);
			break;
		case 8:
			printProcesses();
			break;
		case 9:
			
			break;
		case 10:
			killProcess(size);
			break;
		case 23:
			break;
		default:
			break;
	}
	return dim;
}

void processHandler(char* nombre, void* func, int argc, void * argv, int isForeground){

	process_data* data = kalloc(sizeof(process_data),0);

	data->name = nombre;
	data->func = func;
	data->argc = argc;
	data->argv = argv;
	data->isForeground = isForeground;
	
	newProcess(data);

}

void msgQueueHandler(int code, char * msg, char * nombre, unsigned max, unsigned size){
	MsgQueue * aux;
	switch(code){
		case 0:
			aux = CreateMsgQueue(nombre, max, size);
			break;
		case 1:
			aux = getMessageQueue(nombre);
			if(aux)
				PutMsgQueue(aux, msg);
			else
				ncPrint("No se encontro MsgQueue\n");
			break;
		case 2:
			aux = getMessageQueue(nombre);
			if(aux)
				GetMsgQueue(aux, msg);
			else
				ncPrint("No se encontro MsgQueue\n");
			break;
		case 3:
			aux = getMessageQueue(nombre);
			if(aux)
				DeleteMsgQueue(aux);
			else
				ncPrint("No se encontro MsgQueue\n");
			break;
	}
	/*
	ncPrint("-------------------\n");
	ncPrintDec(code);
	ncNewline();
	ncPrint(msg);
	ncNewline();
	ncPrint(nombre);
	ncNewline();
	ncPrintDec(max);
	ncNewline();
	ncPrintDec(size);
	ncNewline();
	ncPrint("-------------------\n");*/

}

void semaphoreHandler(int code, char * nombre, int valor){
	Semaphore * aux;
	switch(code){
		case 0:
			aux = CreateSem(nombre, valor);
			break;
		case 1:
			aux = getSemaphore(nombre);
			if(aux)
				WaitSem(aux);
			else
				ncPrint("No se encontro Semaforo\n");
			break;
		case 2:
			aux = getSemaphore(nombre);
			if(aux)
				SignalSem(aux);
			else
				ncPrint("No se encontro Semaforo\n");
			break;
		case 3:
			aux = getSemaphore(nombre);
			if(aux)
				DeleteSem(aux);
			else
				ncPrint("No se encontro Semaforo\n");
			break;
	}
}

int setInterrupt(int valor){
	uint16_t flags = getFlags();
	flags = flags&0x200;
	flags = flags>>9;

	if(valor){
		sti();
	}else{
		cli();
	}

	return flags;
}
