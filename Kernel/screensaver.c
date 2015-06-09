#include <naiveConsole.h>
#include <screensaver.h>

static char blue = 0x99;
static char orange = 0x66;
static char white = 0xFF;
static char green = 0x22;

int activated = 0;
int screen = 0;

void activateSS(){
	if(activated)
		return;
	backupScreen();
	blankScreen();
	
	activated = 1;
	toggleScreenSaver();
}

void toggleScreenSaver(){
	if(!activated){
		return;
	}
	fillTank();

	if(screen){
		fish(3);
		plantVines(7,-1);
		screen = 0;
	}else{
		fish(0);
		plantVines(1,1);
		screen = 1;
	}
}

void deactivateSS(){
	if(!activated)
		return;
	restoreScreen();
	activated = 0;
}

void fillTank(){
	int i,j;
	for(i = 0; i<25; i++){
		for(j=0; j<80; j++){
			colorScreen(i,j, blue);
		}
	}
}

void fish(int x){
	int i,k;
	for(i=2; i<11; i++){
		colorScreen(i+x, 22, orange);
	}
	k = 0;
	for(i=2; i<7; i++){
		colorScreen(i+x, 23+k, orange);
		colorScreen(10+x-k, 23+k, orange);
		k++;
	}
	for(i=5; i<8; i++){
		colorScreen(i+x, 28, orange);
	}
	colorScreen(4+x,29, orange);
	colorScreen(7+x, 29, orange);
	for(i=0; i<2; i++){
		colorScreen(3+x,30+i, orange);
		colorScreen(8+x,30+i, orange);
	}
	for(i=0; i<10; i++){
		colorScreen(2+x,32+i, orange);
		colorScreen(9+x,32+i, orange);
	}
	colorScreen(8+x,42, orange);
	colorScreen(3+x, 42, orange);
	colorScreen(7+x,43, orange);
	colorScreen(4+x, 43, orange);
	colorScreen(6+x,44, orange);
	colorScreen(5+x, 44, orange);

	//aleta
	colorScreen(5+x, 36, orange);
	colorScreen(6+x, 36, orange);
	colorScreen(7+x, 36, orange);
	colorScreen(6+x, 35, orange);
	colorScreen(6+x, 34, orange);

	//ojo
	colorScreen(4+x, 40, white);
	

}

void vines(int x, int s){
	int k=0;
	for(int i=0; i<8; i++){
		colorScreen(24-i, x+(k%2)*s, green);
		k++;
	}
}

void plantVines(int x, int s){
	int i;
	for(i=x; i<80; i+=15){
		vines(i, s);
	}
		
}

