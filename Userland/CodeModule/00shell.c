#include <lib.h>
#include <00shell.h>

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;


int main() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	intro();

	char * line;
	char * timeLine;
	
	while(1) {	
		print("$ ");
		line = getLn(line);
		if(!cmpstr(line, "")){
			if(cmpstr(line,"get time")){
				getTime();
			
			}else if(cmpstr(line, "set time")){
				printLn("Input time in the format hh:mm:ss");
				print("->");
				timeLine = getLn();
				if(verifyTime(timeLine)){
					setTime(timeLine);
					printLn("Time set correctly");
				}else{
					printLn("Error setting time: invalid input");
				}
				
			}else if (cmpstr(line, "clear")){
				clearScreen();
				intro();
			}else if(cmpstr(line, "help")){
				help();
			}else{
				print("Error: Command \"");
				print(line);
				printLn("\" not found");
				
			}
		}
	}
	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}



void intro(){
	int i;
	for(i=0; i<80; i++){
		print("*");
	}
	for(i=0; i<32; i++){
		print(" ");
	}
	printLn("LeLu Shell v1.0");
	for(i=0; i<80; i++){
		print("*");
	}
	print("\n");
}

void help(){
	printLn("----------------------    Help    ----------------------");
	printLn("-> get time: shows time");
	printLn("-> set time: sets time");
	printLn("   Shows a prompt to input time in the format hh:mm:ss");
	printLn("-> clear: clears screen");
	printLn("-> help: shows this help");
	printLn("--------------------------------------------------------");
}

int verifyTime(char * time){
	if(time[2] != ':' || time[5] != ':')
		return 0;
	if(time[8] != 0)
		return 0;
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


