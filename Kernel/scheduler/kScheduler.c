#include "scheduler.h"
#include <scheduler_interface.h>
#include <terminal.h>

//TEST
#include <naiveConsole.h>
//

uint64_t newProcess(process_data* pdata)
{
	uint64_t pid = createProcess(pdata->func, pdata->argc, pdata->argv, pdata->name, pdata->isForeground);
	
	return pid;
}

int isForeground(){
	return is_foreground();
}

void becomeForeground(){
	become_foreground();
}


void killProcess(int num){
	
	if(num==0){
		ncPrint("No se puede matar al shell\n");
		return;
	}

	if(num==1){
		ncPrint("No se puede matar al garbage collector\n");
		return;
	}

	Process * p = findProcess(num);

	if(p){
		delete_process(p);
	}else{
		ncPrint("No se encontro tal proceso\n");
	}
	
}

void exit(int status)
{
	terminateProcess();

}

void freeTerminated(){
	free_terminated();
}

void yield()
{
	yield_cpu();

}

void sleep(uint64_t ticks)
{
	if(ticks>0){
		block(0, ticks);
	}
		

}

uint64_t schedule(uint64_t rsp)
{

	incTick();
	//ncPrintDec(count);
	/*		
	ncNewline();
	ncPrint("rsp: ");
	ncPrintHex(rsp);
	ncNewline();
	*/
	
	uint64_t newRsp = select_process(rsp);

	/*
	ncNewline();
	ncPrint("newRsp: ");
	ncPrintHex(newRsp);
	ncNewline();
	*/

	//stack_frame * sf = (stack_frame*)newRsp;
	uint64_t * sp = (uint64_t*)newRsp;
	
	/*ncNewline();	
	int i;
	for(i=0;i<sizeof(stack_frame)/8;i++)
	{	
		ncPrintHex(sp[i]);
		ncPrint(" ");
	}
	
	
	ncNewline();
	ncPrint("newRsp: ");
	ncPrintHex(newRsp);
	ncNewline();	
	//while(1);
	//ncClear();
	
*/

	return newRsp;
		

}

void setUpScheduler()
{
	setScheduler();

}
