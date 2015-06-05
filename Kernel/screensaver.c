#include <naiveConsole.h>

int activated = 0;

void activateSS(){
	if(activated)
		return;
	backupScreen();
	blankScreen();
	ncPrint("SE ACTIVO EL SCREEN SAVER");
	activated = 1;
}

void deactivateSS(){
	if(!activated)
		return;
	restoreScreen();
	activated = 0;
}