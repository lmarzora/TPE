#include <stdint.h>


void init_idt();
void idt_set_gate(uint16_t i , uint64_t dir , uint16_t sel , uint8_t type_attr);


typedef struct
{
	uint16_t limit;
	uint64_t base;
} idt_ptr_t ;

typedef struct
{
	//primeros 64
	uint16_t l_offset;
	uint16_t selector;
	uint8_t cero1;
	uint8_t type_attr;
	uint16_t m_offset;
	//segundos 64
	uint32_t h_offset;
	uint32_t cero2;
} idt_desc_t;

extern void isr0 ();




