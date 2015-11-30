int read(char * buff ,int size);
int write(char * buff ,int size);
int timeGet();
void timeSet(char * time,int size);
void screenClear();
void processesList();
void intervalSet(char * , int );
void alloc(char*,int);

void stuffTest();
void msgQueueSysCall(int code, char * msg, char * nombre, unsigned max, unsigned size);
void semaphoreSysCall(int code, char * nombre, int valor);
void processNew(char * nom, void * func, int argc, void * argv, int isForeground);
void processKill(char *, int num);
void cpuSysCall(int code, int num);