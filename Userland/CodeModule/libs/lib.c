#include <lib.h>
#include <syscalls.h>

char buff[80];


void print(char * line){
	write(line,80);
}

void printLn(char* line) {
	print(line);
	print("\n");
}

char* getLn() {
	read(buff,80);
	return buff;
}

int cmpstr(char * s1, char * s2){
	int i = 0;
	while(s1[i] == s2[i] && s1[i] != 0)
		i++;
	return s1[i] == 0 && s2[i] == 0;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}


void getTime(){
	timeGet();
}

void setTime(char* time){
	timeSet(time,8);
}

void clearScreen(){
	screenClear();
}

void listProcesses(){
	processesList();
}

void setInterval(int num){
	intervalSet("", num);
}

void newProcess(char * nom, void * func, int argc, void * argv, int isForeground){
	processNew(nom, func, argc, argv, isForeground);
}

void killProcess(int num){
	processKill("", num);
}

int isPid(char * buf){
	int num=0;
	int i;
	for(i=0; buf[i] != 0; i++){
		if(!nmbRange(buf[i]))
			return -1;
		num = num*10 + (buff[i] - '0');
	}

	return num;
}

int nmbRange(char c){
	return (c>='0' && c<='9');
}

void testStuff(){
	stuffTest();
}

void newMsgQueue(char * nombre, unsigned max, unsigned size){
	msgQueueSysCall(0, "", nombre, max, size);
}
void putMsgQueue(char * msg, char* nombre){
	msgQueueSysCall(1, msg, nombre, 0, 0);
}

void getMsgQueue(char * msg, char* nombre){
	msgQueueSysCall(2, msg, nombre, 0, 0);
}

void deleteMsgQueue(char * nombre){
	msgQueueSysCall(3, "", nombre, 0, 0);
}

void newSemaphore(char * nombre, int valor){
	semaphoreSysCall(0, nombre, valor);
}
void waitSemaphore(char* nombre){
	semaphoreSysCall(1, nombre, 0);
}

void signalSemaphore(char* nombre){
	semaphoreSysCall(2, nombre, 0);
}

void deleteSemaphore(char * nombre){
	semaphoreSysCall(3, nombre, 0);
}

void sleep(int num){
	cpuSysCall(0, num);
}

void yield(){
	cpuSysCall(1,0);
}