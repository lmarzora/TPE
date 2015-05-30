#include <stdint.h>
#include <idt.h>

idt_desc_t* idt = (idt_desc_t *) 0x0000000000000000;

void idt_set_gate(uint16_t i ,uint64_t dir , uint16_t sel , uint8_t type_attr) {

idt[i].l_offset = dir & 0xffff;
idt[i].selector = sel;
idt[i].cero1 = 0x0;
idt[i].type_attr = type_attr;
idt[i].m_offset= (dir >> 16) & 0xffff;
idt[i].h_offset= (dir >> 32) & 0xffffffff;
idt[i].cero2=0x0;

}
