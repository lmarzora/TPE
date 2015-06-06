#include <stdint.h>
#include <lib.h>

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

void * memset(void * destiny, int32_t c, uint64_t length);
int cmpstr(char * s1, char * s2);
void intro();

int main() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	intro();

	char* line;
	while(1) {	
		print("$ ");
		line = getLn(line);
		if(!cmpstr(line, "")){
			if(cmpstr(line,"get time")){
				getTime();
			
			}else if(cmpstr(line, "set time")){
				printLn("Ingrese la hora de forma: hh.mm.ss");
				print("->");
				line = getLn();
				if(setTime(line)){
					printLn("Hora seteada correctamente");
				}else{
					printLn("Hora ingresada no valida");
				}
				
			}else if (cmpstr(line, "clear")){
				//printLn("todo: clearScreen");
				clearScreen();
				intro();
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

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
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


