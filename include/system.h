#ifndef __SYSTEM_H
#define __SYSTEM_H
typedef int size_t;

/* KERNEL.C */
extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern size_t strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

/* SCREEN.C */
extern void cls();
extern void put_char(unsigned char c);
extern void put_string(unsigned char *text);
extern void set_text_color(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

/* GDT.C */
extern void _gdt_flush();
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void gdt_install();

/* IDT.C */
extern void _idt_load();
extern void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void idt_install();

#endif