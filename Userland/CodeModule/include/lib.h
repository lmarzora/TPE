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
void newProcess(char * nom, void * func, int argc, void * argv);
void killProcess(int num);

void testStuff();

int nmbRange(char c);