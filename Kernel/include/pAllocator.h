#define TOTAL_MEMORY 0x100000000
#define PAGE 0x1000
#define CANT_PAGES TOTAL_MEMORY/PAGE
#define CMP_START 0x8000000000000000

#include <stdint.h>

uint64_t* alloc_page();
void free_page(uint64_t);

