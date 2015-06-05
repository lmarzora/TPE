#include <lib.h>
#include <syscalls.h>

static char buff[80];

void printLn(char* line) {
	write(line,80);
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

void getTime(){
	timeGet();
}

int verifyTime(char * time){
	if(!(nmbRange(time[0]) || nmbRange(time[1])))
		return 0;
	if(!(nmbRange(time[3]) || nmbRange(time[4])))
		return 0;
	if(!(nmbRange(time[6]) || nmbRange(time[7])))
		return 0;

	int hora = (time[0]-'0')*10 + (time[1]-'0');
	int min = (time[3]-'0')*10 + (time[4]-'0');
	int sec = (time[6]-'0') *10 + (time[7]-'0');

	if(hora<0 || hora>=24 || min<0 || min>=60 || sec<0 || sec>=60)
		return 0;

	return 1;
	
}

int nmbRange(char c){
	return (c>='0' && c<='9');
}

int setTime(char* time){
	if(verifyTime(time)){
		timeSet(time,8);
		return 1;
	}
	return 0;
	
}
