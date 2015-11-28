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

uint64_t getProcessSS(){
	return get_process_SS();
}

uint64_t getReservedPages(){
	return get_reserved_pages();
}

void addCantPages(uint64_t pages){
	add_cant_pages(pages);
}

void killProcess(int num){
	
	if(num==0){
		ncPrint("No se puede matar al shell\n");
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

void yield()
{
	yield_cpu();

}

int usleep(uint64_t time)
{
	if(time>0)
		return bedTime(1,time);
	else
		return -1;

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

	stack_frame * sf = (stack_frame*)newRsp;
	uint64_t * sp = (uint64_t*)newRsp;
	/*
	ncNewline();	
	int i;
	for(i=0;i<sizeof(stack_frame)/8;i++)
	{	
		ncPrintHex(sp[i]);
		ncPrint(" ");
	}
	
	/*
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


uint64_t getPid(){
	return get_pid();
}