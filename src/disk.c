#include <stddef.h>

#define ATA_DATA_PORT         0x1F0
#define ATA_FEATURES_PORT     0x1F1
#define ATA_SECTOR_COUNT_PORT 0x1F2
#define ATA_LBA_LOW_PORT      0x1F3
#define ATA_LBA_MID_PORT      0x1F4
#define ATA_LBA_HIGH_PORT     0x1F5
#define ATA_DEVICE_PORT       0x1F6
#define ATA_COMMAND_PORT      0x1F7
#define ATA_STATUS_PORT       0x1F7
#define PRIMARY_CTRL	0x3f6
#define STAT_ERR	(1 << 0)	/* set if error */
#define STAT_DRQ	(1 << 3)	/* pio data ready to read/write */
#define STAT_SRV	(1 << 4)	/* overlapped mode service request */
#define STAT_DF		(1 << 5)	/* drive fault error */
#define STAT_RDY	(1 << 6)	/* set if ready */
#define STAT_BSY	(1 << 7)	/* set if busy */

// Define ATA commands
#define ATA_COMMAND_READ      0x20
#define ATA_COMMAND_WRITE     0x30

char *ata_lba_read(unsigned int drive, unsigned int addr)
{
    while(inportb(0x1F7) & 0x80){}
    put_string("try reading\n");
    unsigned char c;
    char *buffer;
    outportb(0x1F2,0x01);
    outportb(0x1F3,(unsigned char)addr);
    outportb(0x1F4,(unsigned char)(addr >> 8));
    outportb(0x1F5,(unsigned char)(addr >> 16));
    outportb(0x1F6,0xE0 | (drive << 4) | ((addr >>24) & 0x0F));
    outportb(0x1F7,0x20);
    while (!(inportb(0x1F7) & 0x08)) {
    }
    put_string("reading\n");
		(void)inportb(PRIMARY_CTRL);
		(void)inportb(PRIMARY_CTRL);
		(void)inportb(PRIMARY_CTRL);
		(void)inportb(PRIMARY_CTRL);
		/* wait for command to complete */
		do {
			c = inportb(PRIMARY_CTRL);
			if ((c & STAT_ERR) || (c & STAT_DF))
				break;
		} while ((c & STAT_BSY) && !(c & STAT_DRQ));
		if ((c & STAT_ERR) || (c & STAT_DF))
			put_string("ata: drive error\n");
    for(int idx = 0;idx< 256; idx++)
    {
        unsigned short tmpword = inw(0x1F0);
        buffer[idx * 2] = (unsigned char)tmpword;
        buffer[idx * 2 + 1] = (unsigned char)(tmpword >> 8);
    }
    put_string("reading done\n");
    return buffer;
}

void ata_lba_write(unsigned char drive, unsigned int addr,char* buffer)
{
    while(inportb(0x1F7) & 0x80){}
    put_string("try writing\n");
    unsigned char c;
    outportb(0x1F2,0x01);
    outportb(0x1F3,(unsigned char)addr);
    outportb(0x1F4,(unsigned char)(addr >> 8));
    outportb(0x1F5,(unsigned char)(addr >> 16));
    outportb(0x1F6,0xE0 | (drive << 4) | ((addr >>24) & 0x0F));
    outportb(0x1F7,0x30);
    while (!(inportb(0x1F7) & 0x08)) {
    }
		(void)inportb(PRIMARY_CTRL);
		(void)inportb(PRIMARY_CTRL);
		(void)inportb(PRIMARY_CTRL);
		(void)inportb(PRIMARY_CTRL);
		/* wait for command to complete */
		do {
			c = inportb(PRIMARY_CTRL);
			if ((c & STAT_ERR) || (c & STAT_DF))
				break;
		} while ((c & STAT_BSY) && !(c & STAT_DRQ));
		if ((c & STAT_ERR) || (c & STAT_DF))
			put_string("ata: drive error\n");
    put_string("writing\n");
    for(int idx = 0;idx < 256; idx++)
    {
        unsigned short tmpword = (buffer[idx * 2] & 0xFF) | ((buffer[idx * 2 + 1] & 0xFF) << 8);
        outw(0x1F0,tmpword);
    }
    put_string("writing done\n");
    outportb(0x1F7,0xE7);
}
void ata_chs_read(unsigned char drive, unsigned int cylinder, unsigned char head, unsigned char sector, char* buffer, size_t count) {
    // Convert CHS to LBA (Logical Block Address)
    unsigned int lba = (cylinder * 16 * 63) + (head * 16) + sector - 1;

    // Select drive and wait for it to be ready
    outportb(ATA_DEVICE_PORT, 0xE0 | (drive << 4));
    while (inportb(ATA_STATUS_PORT) & 0x80); // Wait until drive is ready

    // Send read command
    outportb(ATA_COMMAND_PORT, ATA_COMMAND_READ);

    // Send LBA
    outportb(ATA_LBA_LOW_PORT, lba & 0xFF);
    outportb(ATA_LBA_MID_PORT, (lba >> 8) & 0xFF);
    outportb(ATA_LBA_HIGH_PORT, (lba >> 16) & 0xFF);
    outportb(ATA_SECTOR_COUNT_PORT, count);

    // Wait for drive to be ready
    while (!(inportb(ATA_STATUS_PORT) & 0x08)) // Wait until drive is ready to transfer data
    {
    }

    // Read data from ATA port
    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < 256; ++j) {
            *(buffer++) = inw(ATA_DATA_PORT);
        }
    }
}

void ata_chs_write(unsigned char drive, unsigned int cylinder, unsigned char head, unsigned char sector, const char* buffer, size_t count) {
    // Convert CHS to LBA (Logical Block Address)
    unsigned int lba = (cylinder * 16 * 63) + (head * 16) + sector - 1;

    // Select drive and wait for it to be ready
    outportb(ATA_DEVICE_PORT, 0xE0 | (drive << 4));
    while (inportb(ATA_STATUS_PORT) & 0x80); // Wait until drive is ready

    // Send write command
    outportb(ATA_COMMAND_PORT, ATA_COMMAND_WRITE);

    // Send LBA
    outportb(ATA_LBA_LOW_PORT, lba & 0xFF);
    outportb(ATA_LBA_MID_PORT, (lba >> 8) & 0xFF);
    outportb(ATA_LBA_HIGH_PORT, (lba >> 16) & 0xFF);
    outportb(ATA_SECTOR_COUNT_PORT, count);

    // Wait for drive to be ready
    while (!(inportb(ATA_STATUS_PORT) & 0x08)); // Wait until drive is ready to transfer data

    // Write data to ATA port
    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < 256; ++j) {
            outw(ATA_DATA_PORT, *(buffer++));
        }
    }
}

void ata_irq_handler()
{

}