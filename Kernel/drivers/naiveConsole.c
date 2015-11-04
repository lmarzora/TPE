#include <naiveConsole.h>

enum SCREEN {KERNEL, USER};

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };

static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;

static const uint32_t width = 80;
static const uint32_t height = 25 ;

static char backupUser[8000];
static int indexBackupUser = 0;
static char backupKernel[8000];
static int indexBackupKernel = 0;

//static char originalScreen[4000];
static uint8_t * originalVideo;

static char userScreen[4000];
static int indexUserScreen = 0;
static char kernelScreen[4000];
static int indexKernelScreen = 0;

static int screen = USER;


void switchScreen(){
	backupScreenWrap();
	if(screen == USER){
		screen = KERNEL;
	}else{
		screen = USER;
	}
	restoreScreenWrap();
}

void printKernel(char * s){
	int i=0;
	while(s[i] != 0 && s[i] != '\n'){
		kernelScreen[indexKernelScreen] = s[i];
		kernelScreen[indexKernelScreen+1] = 0x0F;
		indexKernelScreen+=2;
		i++;
	}
}
//Para screensaver

void backupScreenWrap(){
	if(screen == USER){
		backupScreen(userScreen);
	}else{
		backupScreen(kernelScreen);
	}
}

void restoreScreenWrap(){
	if(screen == USER){
		restoreScreen(userScreen);
	}else{
		restoreScreen(kernelScreen);
	}
}

void backupScreen(char * originalScreen){
	int i;
	for(i=0; i<height*width*2; i++){
		originalScreen[i] = *(video+i);
	}
	if(screen == USER){
		indexUserScreen = currentVideo - video;
	}else{
		indexKernelScreen = currentVideo - video;
	}
	
	currentVideo = video;
}

void restoreScreen(char * originalScreen){
	int i;
	for(i=0; i<height*width*2; i++){
		*(video+i) = originalScreen[i];
		
	}

	if(screen == USER){
		currentVideo = video + indexUserScreen;
	}else{
		currentVideo = video + indexKernelScreen;
	}
	showCursor();
	//currentVideo = originalVideo;
}
////////////////////

void pushUpBackup(){
	char * backup;
	if(screen == USER){
		backup = userScreen;
	}else{
		backup = kernelScreen;
	}
	int i;
	int j;
	for(i=15; i<100; i++){
		for(j=0; j<width; j++){
			backup[(i-15)*width + j] = backup[i*width + j];
		}
	}
	if(screen == USER){
		indexBackupUser -=15;
	}else{
		indexBackupKernel -=15;
	}
	
}

void scrollDownWrap(){
	if(screen == USER){
		scrollDown(userScreen, indexBackupUser);
	}else{
		scrollDown(kernelScreen, indexBackupKernel);
	}
}

void scrollUpWrap(){
	if(screen == USER){
		scrollUp(userScreen, indexBackupUser);
	}else{
		scrollUp(kernelScreen, indexBackupKernel);
	}
}

void scrollDown(char * backup, int indexBackup){
	if((uint64_t)(currentVideo - video) < width*2){
		return;
	}
	int i;
	int limit = (uint64_t)(currentVideo - video)/2;
	currentVideo = video;
	if(indexBackup == 100){
		pushUpBackup();
	}
	for (i = 0; i < width; ++i)
	{
		backup[indexBackup*width + i] = *(currentVideo+i*2);
	}
	indexBackup++;
	for (i = 0; i < limit; i++)
	{
		*currentVideo = *(currentVideo + width*2);
		currentVideo +=2;
	}
	currentVideo -= width*2;
	showCursor();

	if(screen == USER){
		indexBackupUser = indexBackup;
	}else{
		indexBackupKernel = indexBackup;
	}
}



void erase(){
	if(((uint64_t)(currentVideo - video) % (width * 2)) >4){
			currentVideo -=2;
			ncPrintChar(' ');
			currentVideo -=2;
			showCursor();
	}
}

void ncPrintKey(char c){
	//limit
	int point = (uint64_t)(currentVideo - video)%(width*2);
	if(point == ((width-1)*2) ){
		return;
	}
	
	automaticScroll();

	ncPrintChar(c);
}

void scrollUp(char * backup, int indexBackup){
	if(!indexBackup){
		return;
	}
	int i;
	int limit = (uint64_t)(currentVideo - video)/2;
	uint8_t * backupCurrentVideo = currentVideo;
	
	for (i = 0; i <= limit; i++)
	{
		*(currentVideo+width*2) = *currentVideo;
		currentVideo -=2;
	}

	indexBackup--;
	for(i=0; i<width; i++){
		*(video + i*2) = backup[indexBackup*width+i];
	}
	
	currentVideo = backupCurrentVideo + width*2;
	showCursor();
	if(screen == USER){
		indexBackupUser = indexBackup;
	}else{
		indexBackupKernel = indexBackup;
	}
}

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintChar(char character)
{
	if(character == '\n'){
		ncNewline();
	}else{
		*currentVideo = character;
		currentVideo += 2;
		showCursor();
	}

}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);

	automaticScroll();
	
	showCursor();
}

void automaticScroll(){
	int point = (uint64_t)(currentVideo - video)/(width*2);
	while(point >= height){
		scrollDownWrap();
		point = (uint64_t)(currentVideo - video)/(width*2);
	}
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	blankScreen();
	currentVideo = video;
	if(screen == USER){
		indexBackupUser = 0;
	}else{
		indexBackupKernel = 0;
	}
}

void blankScreen(){
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
}

void colorScreen(int i, int j, char color){
	*(video + i*width*2 +(2*j +1)) = color;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
	

	return digits;
}

void showCursor(){
	int pos = (currentVideo - video)/2;
	toggleCursor(pos & 0XFF, (pos >> 8) & 0XFF );
}
