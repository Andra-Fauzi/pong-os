#include <system.h>
/*panjang dan lebar dari layar 80x25 */
unsigned short *textmemptr;
int attribute = 0x0f;
int cursor_x = 0;
int cursor_y = 0;

void scroll()
{
    unsigned blank, temp;

    blank = 0x20 | (attribute << 8);

    if(cursor_y >= 25)
    {
        temp = cursor_y + 1;
        memcpy(textmemptr, textmemptr + temp * 80, (25 -temp) * 80 * 2);

        memsetw(textmemptr + (25 - temp) * 80, blank, 80);
        cursor_y = 25 - 1;
    }
}

void move_cursor()
{
    unsigned temp;
    /* index = y * 80 + x */ 
    temp = cursor_y * 80 + cursor_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void cls()
{
    unsigned blank;
    int i;

    blank = 0x20 | (attribute << 8);

    for(i = 0; i< 25; i++)
    {
        memsetw(textmemptr + i * 80, blank, 80);
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void put_char(unsigned char c)
{
    unsigned short *where;
    unsigned att = attribute << 8;

    if(c == 0x08)
    {
        if(cursor_x != 0) cursor_x--;
    }
    else if(c == 0x09)
    {
        cursor_x = (cursor_x + 8) & ~(8-1);
    }
    else if(c == '\r')
    {
        cursor_x = 0;
    }
    else if(c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    /*apapun character yang melebihi dari spasi bisa di print*/
    else if(c >= ' ')
    {
        where = textmemptr + (cursor_y * 80 + cursor_x);
        *where = c | att;
        cursor_x++;
    }
    if(cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y++;
    }
    scroll();
    move_cursor();
}

void put_string(unsigned char *text)
{
    int i;
    for(i = 0;i<strlen(text); i++)
    {
        put_char(text[i]);
    }
}

void set_text_color(unsigned char forecolor, unsigned char backcolor)
{
    attribute = (backcolor << 4) | (forecolor & 0x0f);
}

void init_video()
{
    textmemptr = (unsigned short*)0xB8000;
    cls();
}