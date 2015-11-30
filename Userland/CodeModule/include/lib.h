#include <stdint.h>

void printLn(char*);
void print(char*);
char* getLn();
void * memset(void * destiny, int32_t c, uint64_t length);
int cmpstr(char * s1, char * s2);
int ispid(char * buf);
void getTime();
void setTime(char*);
void setInterval(int);
void clearScreen();

void listProcesses();
void newProcess(char * nom, void * func, int argc, void * argv, int isForeground);
void killProcess(int num);
void listIpcs();

void testStuff();

int nmbRange(char c);
int isPid(char * buf);

void newMsgQueue(char * nombre, unsigned max, unsigned size);
void putMsgQueue(char * msg, char* nombre);
void getMsgQueue(char * msg, char* nombre);
void deleteMsgQueue(char * nombre);

void newSemaphore(char * nombre, int valor);
void waitSemaphore(char * nombre);
void signalSemaphore(char* nombre);
void deleteSemaphore(char * nombre);

void sleep(int num);
void yield();