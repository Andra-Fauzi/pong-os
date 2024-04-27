#include <system.h>

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

unsigned char *memset(unsigned char *dest, unsigned char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

size_t strlen(const char *str)
{
    size_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

/*untuk menerima byte ke dari I/O port*/
unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/*untuk mengirim byte ke dari I/O port*/
void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void _main()
{
    init_video();
    gdt_install();
    idt_install();
    put_string("halo kernel\n");
    put_string("halo kernel\n");
    put_string("halo kernel\n");
    put_string("halo kernel\n");
    /*fungsi utama*/
    for (;;);
}