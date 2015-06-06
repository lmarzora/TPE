#include <stdint.h>
#include <lib.h>

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

void * memset(void * destiny, int32_t c, uint64_t length);
int cmpstr(char * s1, char * s2);

int main() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	//All the following code may be removed 
	
	char* line;
	while(1) {	
		printLn("$ ");
		line = getLn(line);
		if(!cmpstr(line, "")){
			if(cmpstr(line,"get time")){
				getTime();
			
			}else if(cmpstr(line, "set time")){
				printLn("Ingrese la hora (hh:mm:ss)\n");
				printLn("->");
				line = getLn();
				if(setTime(line)){
					printLn("Hora seteada correctamente\n");
				}else{
					printLn("Hora ingresada no valida\n");
				}
				
			}else if (cmpstr(line, "clear")){
				printLn("todo: clearScreen\n");
				
			}else{
				printLn("Error: Command \"");
				printLn(line);
				printLn("\" not found\n");
				
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


