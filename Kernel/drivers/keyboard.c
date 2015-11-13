#include <terminal.h>
#include <keyboard.h>

static int rightShift = 0;
static int leftShift = 0;
static int capsLock = 0;
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
    0,   0,   0,    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

int shiftCaps(){
  return ((rightShift || leftShift) + capsLock)%2;
}

char shiftKeys(char c){
  if(!(leftShift || rightShift))
    return c;

  switch(c){
    case '1':
      return '!';
    case '2':
      return '@';
    case '3':
      return '#';
    case '4':
      return '$';
    case '5':
      return '%';
    case '6': 
      return '^';
    case '7':
      return '&';
    case '8':
      return '*';
    case '9':
      return '(';
    case '0':
      return ')';
    case '-':
      return '_';
    case '=':
      return '+';
    case ';':
      return ':';
    case '[':
      return '{';
    case ']':
      return '}';
    case '\\':
      return '|';
    case '\'':
      return '\"';
    case ',':
      return '<';
    case '.':
      return '>';
    case '/':
      return '?';
    case '`':
      return '~';
  }
  return 'Y';
}

void analizeKeyboard(unsigned char c){
	char key = kbdus[c];
 
	if(key == '\b'){ //backspace
		backspace();
	}else if(key == '\n'){
		enter();
  }else if(c==42){
    leftShift = 1;
  }else if (c==170){
    leftShift = 0;
  }else if(c == 54){
    rightShift = 1;
  }else if(c == 182){
    rightShift = 0;
  }else if(c == 58){
    capsLock = (capsLock +1)%2;
  }else if(c>=2 && c<58 && (c!=15 && c!=29 && c!=42 && c!= 54 && c!=56)){
		if(key >= 'a' && key<='z'){
      insertKey(key - 32*shiftCaps());
    }else{
      insertKey(shiftKeys(key));
    }
	}else if(c == 80){
		downArrow();
	}else if(c == 72){
		upArrow();
	}
}

