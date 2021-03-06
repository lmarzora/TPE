#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char * string);
void ncPrintCol(const char * string, int col);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintDecCol(uint64_t value, int col);
void ncPrintHexCol(uint64_t value, int col);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncPrintBaseCol(uint64_t value, uint32_t base, int col);
void ncClear();

void toggleCursor(uint16_t x, uint16_t y);
void showCursor();
void pushUpBackup();

void erase();
void scrollDown();
void scrollUp();
void automaticScroll();
void ncPrintKey(char c);

void backupScreen();
void restoreScreen();
void blankScreen();
void colorScreen(int i, int j, char color);

#endif