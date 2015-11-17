#include <shell.h>
#include <lib.h>
#include<memory.h>
#include<debug.h>
extern char bss;
extern char endOfBinary;

int main() {
	

	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);
	
	intro();
	
	
	char * line;
	
	while(1) {	
		print("$ ");
		line = getLn(line);
		if(!cmpstr(line, "")){
			if(cmpstr(line,"get time")){
				getTime();
			
			}else if(cmpstr(line, "set time")){
				printLn("Input time in the format hh:mm:ss");
				print("->");
				line = getLn();
				if(verifyTime(line)){
					setTime(line);
					printLn("Time set correctly");
				}else{
					printLn("Error setting time: invalid input");
				}
				
			}else if (cmpstr(line, "clear")){
				clearScreen();
				intro();
			}else if(cmpstr(line, "help")){
				help();
			}else if(cmpstr(line, "set interval")){
				printLn("Input seconds to wait for screen saver activation (from 1 to 9999)");
				print("->");
				line = getLn();
				int nmbrInterval = verifyInterval(line);
				if(nmbrInterval){
					setInterval(nmbrInterval);
					printLn("Interval set correctly");
				}else{
					printLn("Error setting interval: invalid input");
				}

			}else{
				print("Error: Command \"");
				print(line);
				printLn("\" not found");
				
			}
		}
	}
	
	return 0;
}



void intro(){
	int i;
	for(i=0; i<80; i++){
		print("*");
	}
	for(i=0; i<32; i++){
		print(" ");
	}
	printLn("LeLuJo Shell v2.0");
	for(i=0; i<80; i++){
		print("*");
	}
	print("\n");
}

void help(){
	printLn("--------------------------    Help    --------------------------");
	printLn("-> get time: shows time");
	printLn("-> set time: sets time");
	printLn("   Shows a prompt to input time in the format hh:mm:ss");
	printLn("-> set interval: sets time to wait for screen saver activation");
	printLn("   Shows a prompt to input seconds between 1 and 9999");
	printLn("-> clear: clears screen");
	printLn("-> help: shows this help");
	printLn("----------------------------------------------------------------");
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

int verifyInterval(char * interval){
	int i=0;
	int nmbr = 0;
	while(interval[i] != 0){
		if(!nmbRange(interval[i]))
			return 0;
		
		nmbr = nmbr*10 + (interval[i] - '0');
		if(nmbr>9999)
			return 0;

		i++;
	}

	return nmbr;
}


