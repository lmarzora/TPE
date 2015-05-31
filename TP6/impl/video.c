// ((char*) 0x8000)[0] --> para acceder zonas memorias

char *video = (char *) 0xB8000;
static int posx = 0;
static int posy = 0;
static int capsLock = 0;
char blanco = (char) 0x0F;

void printInt(int);
void printf(char * msg);
void clearScreen();
void println(char*);
void putchar(char);
void toggleCursor();
int poseval();
void insertKeyboard(char c);

unsigned static char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


void toggleCursor(){
	int pos = poseval();	
	if(video[pos] == '_'){
		video[pos] = ' ';
	}else{
		video[pos] = '_';
	}
	video[pos+1] = blanco;
}

void putchar(char c){
	if(posx==79)
		return;
	int pos = poseval();
	video[pos] = c;
	video[pos+1] = blanco;
	posx++;
}

void insertKeyboard(char c){
	char key = kbdus[c];
	int pos = poseval();
	if(key == '\b'){ //backspace
		if(posx){
			video[pos+1] = 0;		
			video[pos] = 0;
			video[pos-1] = 0;
			video[pos-2] = 0;
			posx--;
		}
	}else if((c>=16 && c<=25) || (c>=30 && c<=38) || (c>=44 && c<=50)){//letra
		putchar(key - 32*capsLock);
	}else if(c==57){
		putchar(key);
	}else if(c == 58){
		capsLock = (capsLock + 1)%2;
	}else if(key == '\n'){
		video[pos+1] = 0;		
		video[pos] = 0;
		posx = 0;
		posy++;
	}
	/*else	if (!(c & 0x80)){
		putchar(key);
	}*/
}

void println(char * msg){
	int i =0;
	while(msg[i]){
		putchar(msg[i++]);
	}
	posy++;
	posx = 0;
}

int poseval(){
	if(posx == 80){
		posx = 0;
		posy++;
	}
	return (posy*80 + posx)*2;
}

void clearScreen(){
	int total = 80*25;
	int i = 0;
	
	for(i=0; i<total; i++){
		video[i] = 0;
	}
	posx=0;
	posy=0;
}

void printInt(int n){
	char string[128];
	int aux = n;

	int i = 0;
	while(aux != 0){
		aux /= 10;
		i++;
	}

	aux = n;
	//OJO CON MAXIMOS
	string[i--] = 0;
	while(aux !=0){
		string[i--] = '0' + aux%10; 
		aux /= 10;
	}
	println(string);
}



