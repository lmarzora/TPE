#include <stdint.h>

void printTime();
int getHora();
int getMin();
int getSec();
void setHora(uint8_t);
void setMin(uint8_t);
void setSec(uint8_t);
int setTime(int hora, int min, int sec);
uint8_t intToTime(int num);
