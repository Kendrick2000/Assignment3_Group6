// ----------------------------------- framebf.c ------------------------------------- 
#include "mbox.h" 
#include "uart.h" 
#include "font.h"
 
//Use RGBA32 (32 bits for each pixel) 
#define COLOR_DEPTH 32 
 
//Pixel Order: BGR in memory order (little endian --> RGB in byte order) 
#define PIXEL_ORDER 0 
 
//Screen info 
unsigned int width, height, pitch; 
 
/* Frame buffer address 
 * (declare as pointer of unsigned char to access each byte) */ 
unsigned char *fb; 
 
 
/** 
 * Set screen resolution to 1024x768 
 */ 
void framebf_init() 
{ 
    mbox[0] = 35*4; // Length of message in bytes 
    mbox[1] = MBOX_REQUEST; 
 
    mbox[2] = MBOX_TAG_SETPHYWH; //Set physical width-height 
    mbox[3] = 8; // Value size in bytes 
    mbox[4] = 0; // REQUEST CODE = 0 
    mbox[5] = 1024; // Value(width) 
    mbox[6] = 768; // Value(height) 
 
    mbox[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height 
    mbox[8] = 8; 
    mbox[9] = 0; 
    mbox[10] = 1024; 
    mbox[11] = 768; 
 
    mbox[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset 
    mbox[13] = 8; 
    mbox[14] = 0; 
    mbox[15] = 0; // x offset 
    mbox[16] = 0; // y offset 
 
    mbox[17] = MBOX_TAG_SETDEPTH; //Set color depth 
    mbox[18] = 4; 
    mbox[19] = 0; 
    mbox[20] = COLOR_DEPTH; //Bits per pixel 
 
    mbox[21] = MBOX_TAG_SETPXLORDR; //Set pixel order 
    mbox[22] = 4; 
    mbox[23] = 0; 
    mbox[24] = PIXEL_ORDER; 
 
    mbox[25] = MBOX_TAG_GETFB; //Get frame buffer 
    mbox[26] = 8; 
    mbox[27] = 0; 
    mbox[28] = 16; //alignment in 16 bytes 
    mbox[29] = 0;  //will return Frame Buffer size in bytes 
 
    mbox[30] = MBOX_TAG_GETPITCH; //Get pitch 
    mbox[31] = 4; 
    mbox[32] = 0; 
    mbox[33] = 0; //Will get pitch value here 
 
    mbox[34] = MBOX_TAG_LAST; 
 
    // Call Mailbox 
    if (mbox_call(ADDR(mbox), MBOX_CH_PROP) //mailbox call is successful ? 
        && mbox[20] == COLOR_DEPTH //got correct color depth ? 
        && mbox[24] == PIXEL_ORDER //got correct pixel order ? 
        && mbox[28] != 0 //got a valid address for frame buffer ? 
  ) { 
 
     /* Convert GPU address to ARM address (clear higher address bits) 
      * Frame Buffer is located in RAM memory, which VideoCore MMU 
      * maps it to bus address space starting at 0xC0000000. 
      * Software accessing RAM directly use physical addresses 
      * (based at 0x00000000) 
     */ 
     mbox[28] &= 0x3FFFFFFF; 
 
        // Access frame buffer as 1 byte per each address 
        fb = (unsigned char *)((unsigned long)mbox[28]); 
        /*uart_puts("Got allocated Frame Buffer at RAM physical address: "); 
        uart_hex(mbox[28]); 
        uart_puts("\n"); 
 
        uart_puts("Frame Buffer Size (bytes): "); 
        uart_dec(mbox[29]); 
        uart_puts("\n"); 
        */
        width = mbox[5];      // Actual physical width 
        height = mbox[6];      // Actual physical height 
        pitch = mbox[33];       // Number of bytes per line 
 
    } else { 
     uart_puts("Unable to get a frame buffer with provided setting\n"); 
    } 
} 
 
 
void drawPixelARGB32(int x, int y, unsigned int attr) 
{ 
 int offs = (y * pitch) + (COLOR_DEPTH/8 * x); 
 
/* //Access and assign each byte 
    *(fb + offs    ) = (attr >> 0 ) & 0xFF; //BLUE 
    *(fb + offs + 1) = (attr >> 8 ) & 0xFF; //GREEN 
    *(fb + offs + 2) = (attr >> 16) & 0xFF; //RED 
    *(fb + offs + 3) = (attr >> 24) & 0xFF; //ALPHA 
*/ 
 
 //Access 32-bit together 
 *((unsigned int*)(fb + offs)) = attr; 
} 

void drawPixel(int x, int y, unsigned char attr)
{
    int offs = (y * pitch) + (x * 4);
    *((unsigned int*)(fb + offs)) = vgapal[attr & 0x0f];
}

 
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill) 
{ 
 for (int y = y1; y <= y2; y++ ) 
  for (int x = x1; x <= x2; x++) { 
   if ((x == x1 || x == x2) || (y == y1 || y == y2)) 
    drawPixelARGB32(x, y, attr); 
   else if (fill) 
    drawPixelARGB32(x, y, attr); 
   } 
} 


// Source code <https://github.com/isometimes/rpi4-osdev/tree/master/part5-framebuffer>
void drawChar(unsigned char ch, int x, int y, unsigned char attr)
{
    unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

    for (int i=0;i<FONT_HEIGHT;i++) {
	for (int j=0;j<FONT_WIDTH;j++) {
	    unsigned char mask = 1 << j;
	    unsigned char col = (*glyph & mask) ? attr & 0x0f : (attr & 0xf0) >> 4;

	    drawPixel(x+j, y+i, col);
	}
	glyph += FONT_BPL;
    }
}

void drawString(int x, int y, char *s, unsigned char attr)
{
    while (*s) {
       if (*s == '\r') {
          x = 0;
       } else if(*s == '\n') {
          x = 0; y += FONT_HEIGHT;
       } else {
	  drawChar(*s, x, y, attr);
          x += FONT_WIDTH;
       }
       s++;
    }
}
void drawChar32x32(unsigned char ch, int x, int y, unsigned char attr)
{
    unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

    for (int i=0;i<FONT_HEIGHT;i++) {
	for (int j=0;j<FONT_WIDTH;j++) {
	    unsigned char mask = 1 << j;
	    unsigned char col = (*glyph & mask) ? attr & 0x0f : 0x00;

	    drawPixel(x+2*j, y+2*i, col);
        drawPixel(x+2*j, y+2*i + 1, col);
       	drawPixel(x+2*j +1, y+2*i, col);
       	drawPixel(x+2*j+1, y+2*i+1, col);
	}
	glyph += FONT_BPL;
    }
}

void drawString32x32(int x, int y, char *s, unsigned char attr)
{
    while (*s) {
       if (*s == '\r') {
          x = 0;
       } else if(*s == '\n') {
          x = 0; y += FONT_HEIGHT*2;
       } else {
	  drawChar32x32(*s, x, y, attr);
          x += FONT_WIDTH*2;
       }
       s++;
    }
}

void drawCharWelcome(unsigned char ch, int x, int y, unsigned char attr)
{
    unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

    for (int i = 0; i < FONT_HEIGHT; i++) {
        for (int j = 0; j < FONT_WIDTH; j++) {
            unsigned char mask = 1 << j;
            unsigned char col = (*glyph & mask) ? attr & 0x0f : 0x00;

            drawPixel(x+8*j, y+8*i, col);
            drawPixel(x+8*j, y+8*i + 1, col);
            drawPixel(x+8*j + 1, y+8*i, col);
            drawPixel(x+8*j + 1, y+8*i + 1, col);

            //drawPixel(x+8*j, y+8*i, col);
            drawPixel(x+8*j, y+8*i + 2, col);
            drawPixel(x+8*j + 2, y+8*i, col);
            drawPixel(x+8*j + 2, y+8*i+2, col);
            drawPixel(x+8*j + 1, y+8*i + 2, col);
            drawPixel(x+8*j + 2, y+8*i + 1, col);
            

            drawPixel(x+8*j, y+8*i + 3, col);
            drawPixel(x+8*j + 3, y+8*i, col);
            drawPixel(x+8*j + 3, y+8*i+3, col);
            drawPixel(x+8*j + 1, y+8*i + 3, col);
            drawPixel(x+8*j + 3, y+8*i + 1, col);

            drawPixel(x+8*j, y+8*i + 4, col);
            drawPixel(x+8*j + 4, y+8*i, col);
            drawPixel(x+8*j + 4, y+8*i+4, col);
            drawPixel(x+8*j + 1, y+8*i + 4, col);
            drawPixel(x+8*j + 4, y+8*i + 1, col);

            drawPixel(x+8*j, y+8*i + 5, col);
            drawPixel(x+8*j + 5, y+8*i, col);
            drawPixel(x+8*j + 5, y+8*i+5, col);
            drawPixel(x+8*j + 8, y+8*i + 5, col);
            drawPixel(x+8*j + 5, y+8*i + 8, col);

            drawPixel(x+8*j + 5, y+8*i + 2, col);
            drawPixel(x+8*j + 2, y+8*i + 5, col);          

            drawPixel(x+8*j  + 5, y+8*i + 3, col);
            drawPixel(x+8*j + 3, y+8*i  + 5, col);            

            drawPixel(x+8*j + 5, y+8*i + 4, col);
            drawPixel(x+8*j + 4, y+8*i + 5, col);
            
            drawPixel(x+8*j, y+8*i + 5, col);
            drawPixel(x+8*j + 5, y+8*i, col);
            drawPixel(x+8*j + 5, y+8*i+5, col);
            drawPixel(x+8*j + 8, y+8*i + 5, col);
            drawPixel(x+8*j + 5, y+8*i + 8, col);

            drawPixel(x+8*j, y+8*i + 6, col);
            drawPixel(x+8*j + 6, y+8*i, col);
            drawPixel(x+8*j + 6, y+8*i+6, col);
            drawPixel(x+8*j + 8, y+8*i + 6, col);
            drawPixel(x+8*j + 6, y+8*i + 8, col);

            drawPixel(x+8*j, y+8*i + 7, col);
            drawPixel(x+8*j + 7, y+8*i, col);
            drawPixel(x+8*j + 7, y+8*i+7, col);
            drawPixel(x+8*j + 8, y+8*i + 7, col);
            drawPixel(x+8*j + 7, y+8*i + 8, col);

            drawPixel(x+8*j, y+8*i + 8, col);
            drawPixel(x+8*j + 8, y+8*i, col);
            drawPixel(x+8*j + 8, y+8*i+8, col);
            drawPixel(x+8*j + 8, y+8*i + 8, col);
            drawPixel(x+8*j + 8, y+8*i + 8, col);
        }
        glyph += FONT_BPL;
    }
}

void drawStringWelcome(int x, int y, char *s, unsigned char attr)
{
    while (*s) {
       if (*s == '\r') {
          x = 0;
       } else if(*s == '\n') {
          x = 0; y += FONT_HEIGHT*8;
       } else {
	  drawCharWelcome(*s, x, y, attr);
          x += FONT_WIDTH*8;
       }
       s++;
    }
}

void framebf_blank() {
    mbox[0] = 8 * 4;        // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    mbox[2] = 0x00040005;   // TAG Identifier: Frame buffer blank screen.
    mbox[3] = 32;           // Value buffer size in bytes (max of request and response lengths)
    mbox[4] = 0;            // REQUEST CODE = 0
    mbox[5] = 4;            // RESPOND LENGTH = 4
    mbox[6] = 0;            // clear output buffer (response data are mbox[5] & mbox[6])
    mbox[7] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mbox), MBOX_CH_PROP))
    {
        uart_puts("Clear screen");
        uart_puts("\n");
    } 
}

void framebf_release() {
    mbox[0] = 8 * 4;        // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    mbox[2] = 0x00048001;   // TAG Identifier: Frame buffer release.
    mbox[3] = 0;           // Value buffer size in bytes (max of request and response lengths)
    mbox[4] = 0;            // REQUEST CODE = 0
    mbox[5] = 0;            // RESPOND LENGTH = 4
    mbox[6] = 0;            // clear output buffer (response data are mbox[5] & mbox[6])
    mbox[7] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mbox), MBOX_CH_PROP))
    {
        uart_puts("Frame buffer release");
        uart_puts("\n");
    } 
}