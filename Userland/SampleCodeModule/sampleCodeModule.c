#include <stdint.h>
//#include "libc.h"

int read(char * buff ,int size);
int write(char * buff ,int size);



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
	
	char * hola[80] ;
	while(1) {	
		read(hola,80);
		if(cmpstr(hola,"get time")){
			write("todo: ejecutar getTime", 80);
		}else if(cmpstr(hola, "set time")){
			write("todo: ejecutar setTime", 80);
		}else if (cmpstr(hola, "clear")){
			write("todo: clearScreen", 80);
		}else{
			write("error: command not found", 20);
			write(hola,80);
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

int cmpstr(char * s1, char * s2){
	int i = 0;
	while(s1[i] == s2[i] && s1[i] != 0)
		i++;
	return s1[i] == 0 && s2[i] == 0;
}