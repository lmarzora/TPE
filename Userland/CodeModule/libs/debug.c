#include <debug.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;
static char backup[8000];
static int indexBackup = 0;

static char originalScreen[4000];
static uint8_t * originalVideo;

//Para screensaver
void backupScreen(){
	int i;
	for(i=0; i<height*width*2; i++){
		originalScreen[i] = *(video+i);
	}
	originalVideo = currentVideo;
	currentVideo = video;
}

void restoreScreen(){
	int i;
	for(i=0; i<height*width*2; i++){
		*(video+i) = originalScreen[i];
		
	}
	currentVideo = originalVideo;
}
////////////////////

void pushUpBackup(){
	int i;
	int j;
	for(i=15; i<100; i++){
		for(j=0; j<width; j++){
			backup[(i-15)*width + j] = backup[i*width + j];
		}
	}
	indexBackup -=15;
}

void scrollDown(){
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

void scrollUp(){
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
		scrollDown();
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
	indexBackup = 0;
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
