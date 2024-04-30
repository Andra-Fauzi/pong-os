#include <system.h>
#include <stdbool.h>

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
unsigned short inw(unsigned short port)
{
   unsigned short ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

void outw(unsigned short port, unsigned short data) {
    asm volatile ("outw %0, %1" : : "a" (data), "Nd" (port));
}

void io_wait(void)
{
    outportb(0x80, 0);
}

void dump_hex(const unsigned char *buffer, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        put_char(buffer[i]);
    }
}

unsigned char ata_get_status() {
    return inportb(0x1F7);
}

unsigned char ata_get_error() {
    return inportb(0x1F1);
}

void check_ata_error() {
    unsigned char status = ata_get_status();
    if (status & 0x01) {  // Check ERR bit
        unsigned char error = ata_get_error();
        put_string(error);
    }
}


void ata_check()
{
    void test_ata_operations() {
    // Initialize video or console output if necessary
    put_string("Starting ATA read/write test...\n");

    // Test data to write
    char *write_data = "andra\n"; // Assuming sector size of 512 bytes

    // Write to drive 0, sector 100
    
    ata_lba_write(0, 5743, write_data);
    put_string("First write complete.\n");
    

    // Read back from drive 0, sector 100
    char *read_data = ata_lba_read(0, 5743);
    put_string("First read data:\n");
    put_string(read_data);  // Dump the hex content of the buffer
    

    char *write_data1 = "andra\n"; // Assuming sector size of 512 bytes
    // Write to drive 0, sector 101 with different data
    ata_lba_write(0, 5742, write_data1);
    put_string("Second write complete.\n");

    // Read back from drive 0, sector 101
    char *read_data1 = ata_lba_read(0, 5742);
    put_string("Second read data:\n");
    put_string(read_data1);  // Dump the hex content of the buffer
}
test_ata_operations();
}


void _main()
{
    set_text_color(0xa,0x0);
    init_video();
    gdt_install();
    idt_install();
    isrs_install();
    __asm__ __volatile__ ("sti");
    irq_install();
    timer_install();
    keyboard_install();
    enable_paging();


    ata_check();
    
    /*fungsi utama*/
    for (;;);
}