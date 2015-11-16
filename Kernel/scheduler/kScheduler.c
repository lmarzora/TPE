#include"scheduler.h"
#include<scheduler_interface.h>

uint64_t newProcess(process_data* pdata)
{
	uint64_t pid = createProcess(pdata->func, pdata->argc, pdata->argv, pdata->name);
	
	return pid;
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
	
			
	ncNewline();
	ncPrint("rsp: ");
	ncPrintHex(rsp);
	ncNewline();

	uint64_t newRsp = select_process(rsp);

	ncNewline();
	ncPrint("newRsp: ");
	ncPrintHex(newRsp);
	ncNewline();

		

}

void setUpScheduler()
{
	setScheduler();

}
