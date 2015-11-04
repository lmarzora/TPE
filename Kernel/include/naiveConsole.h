#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();

void toggleCursor(uint16_t x, uint16_t y);
void showCursor();
void pushUpBackup();

void erase();
void scrollDownWrap();
void scrollUpWrap();
void scrollDown(char * backup, int indexBackup);
void scrollUp(char * backup, int indexBackup);
void automaticScroll();
void ncPrintKey(char c);

void backupScreenWrap();
void restoreScreenWrap();
void backupScreen(char * originalScreen);
void restoreScreen(char * originalScreen);
void blankScreen();
void colorScreen(int i, int j, char color);

void printKernel(char * s);

#endif