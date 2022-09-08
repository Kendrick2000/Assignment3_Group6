//Daniil's image.c

#include "mailbox.h"  //Adjust if necessary
#include "uart_uni.h" //Adjust if necessary




#define COLOR_DEPTH 32 //Use RGBA32 (32 bits for each pixel) 
#define PIXEL_ORDER 0 //Pixel Order: BGR in memory order (little endian --> RGB in byte order) 
unsigned int scrWidth, scrHeight, scrPitch; //Screen info 

/* Frame buffer address 
 * (declare as pointer of unsigned char to access each byte) */ 
unsigned char *fb; 

/** 
 * Set screen resolution to 1024x768 
 */ 
void gfx_InitFrameBuffer() 
{ 
    mBuf[0] = 35*4; // Length of message in bytes 
    mBuf[1] = MBOX_REQUEST; 
 
    mBuf[2] = MBOX_TAG_SETPHYWH; //Set physical width-height 
    mBuf[3] = 8; // Value size in bytes 
    mBuf[4] = 0; // REQUEST CODE = 0 
    mBuf[5] = 1024; // Value(width) 
    mBuf[6] = 768; // Value(height) 
 
    mBuf[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height 
    mBuf[8] = 8; 
    mBuf[9] = 0; 
    mBuf[10] = 1024; 
    mBuf[11] = 768; 
 
    mBuf[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset 
    mBuf[13] = 8; 
    mBuf[14] = 0; 
    mBuf[15] = 0; // x offset 
    mBuf[16] = 0; // y offset 
 
    mBuf[17] = MBOX_TAG_SETDEPTH; //Set color depth 
    mBuf[18] = 4; 
    mBuf[19] = 0; 
    mBuf[20] = COLOR_DEPTH; //Bits per pixel 
 
    mBuf[21] = MBOX_TAG_SETPXLORDR; //Set pixel order 
    mBuf[22] = 4; 
    mBuf[23] = 0; 
    mBuf[24] = PIXEL_ORDER; 
 
    mBuf[25] = MBOX_TAG_GETFB; //Get frame buffer 
    mBuf[26] = 8; 
    mBuf[27] = 0; 
    mBuf[28] = 16; //alignment in 16 bytes 
    mBuf[29] = 0;  //will return Frame Buffer size in bytes 
 
    mBuf[30] = MBOX_TAG_GETPITCH; //Get pitch 
    mBuf[31] = 4; 
    mBuf[32] = 0; 
    mBuf[33] = 0; //Will get pitch value here 
 
    mBuf[34] = MBOX_TAG_LAST; 
 
    // Call Mailbox 
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP) //mailbox call is successful ? 
     && mBuf[20] == COLOR_DEPTH //got correct color depth ? 
  && mBuf[24] == PIXEL_ORDER //got correct pixel order ? 
  && mBuf[28] != 0 //got a valid address for frame buffer ? 
  ) { 
 
     /* Convert GPU address to ARM address (clear higher address bits) 
      * Frame Buffer is located in RAM memory, which VideoCore MMU 
      * maps it to bus address space starting at 0xC0000000. 
      * Software accessing RAM directly use physical addresses 
      * (based at 0x00000000) 
     */ 
     mBuf[28] &= 0x3FFFFFFF; 
 
        // Access frame buffer as 1 byte per each address 
        fb = (unsigned char *)((unsigned long)mBuf[28]); 
        full_uart_puts("Got allocated Frame Buffer at RAM physical address: "); 
        full_uart_hex(mBuf[28]); 
        full_uart_puts("\n"); 
 
        full_uart_puts("Frame Buffer Size (bytes): "); 
        full_uart_dec(mBuf[29]); 
        full_uart_puts("\n"); 
 
        scrWidth = mBuf[5];      // Actual physical width 
        scrHeight = mBuf[6];      // Actual physical height 
        scrPitch = mBuf[33];       // Number of bytes per line 
 
    } else { 
     full_uart_puts("Unable to get a frame buffer with provided setting\n"); 

    } 
}

void gfx_FrameBufferSetVirtualScreen(int w, int h) 
    { 
        mBuf[0] = 13*8; // Length of message in bytes 
        mBuf[1] = MBOX_REQUEST; 
    
        mBuf[2] = MBOX_TAG_SETVIRTWH; //Set virtual scrWidth-height 
        mBuf[3] = 8; 
        mBuf[4] = 0; 
        mBuf[5] = w; 
        mBuf[6] = h; 
    
        mBuf[7] = MBOX_TAG_SETVIRTOFF; //Set virtual offset 
        mBuf[8] = 8; 
        mBuf[9] = 0; 
        mBuf[10] = 0; // x offset 
        mBuf[11] = 0; // y offset 
        mBuf[12] = MBOX_TAG_LAST;
    
        // Call Mailbox 
        if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) //mailbox call is successful
        { 
            full_uart_puts("Virtual screen size set\n");
        } 
        else 
        { 
            full_uart_puts("Unable to get a virtual frame buffer with provided setting\n"); 

        } 
    } 
 
void gfx_FrameBufferSetPhysicalScreen(int w, int h) 
    { 
        mBuf[0] = 8*8; // Length of message in bytes 
        mBuf[1] = MBOX_REQUEST; 
    
        mBuf[2] = MBOX_TAG_SETPHYWH; //Set phys scrWidth-height 
        mBuf[3] = 8; 
        mBuf[4] = 0; 
        mBuf[5] = w; 
        mBuf[6] = h; 

    
        mBuf[7] = MBOX_TAG_LAST; 
    
        // Call Mailbox 
        if (mbox_call(ADDR(mBuf), MBOX_CH_PROP) ) //mailbox call is successful
            { 
                full_uart_puts("Physical screen size set\n");
            } 
        else 
            { 
                full_uart_puts("Unable to set  a physical frame buffer width/height with provided settings\n"); 
            } 
    }
 
void drawPixelARGB32(int x, int y, unsigned int attr) 
    { 
    int offs = (y * scrPitch) + (COLOR_DEPTH/8 * x); //Convert x, y numerical coords to the memory offset address for a particular pixel 
    
    /* //Access and assign each byte 
        *(fb + offs    ) = (attr >> 0 ) & 0xFF; //BLUE 
        *(fb + offs + 1) = (attr >> 8 ) & 0xFF; //GREEN 
        *(fb + offs + 2) = (attr >> 16) & 0xFF; //RED 
        *(fb + offs + 3) = (attr >> 24) & 0xFF; //ALPHA 
    */ 
    
    //Access 32-bit together 
    *((unsigned int*)(fb + offs)) = attr; 
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


void drawSpriteARGB32(int* sprite, int hRes, int vRes, int x, int y) 
{ 
        //DANIIL SHLEE. V 1.0. 30/08/2022
        int topLeftX = x-(hRes/2); //Top left corner of the sprite, x-coord
        int topLeftY = y-(vRes/2); //Top left corner of the sprite, y-coord. RPI follows the "Downwards Y-axis is positive" convention. I.e. to go up on the screen, you SUBTRACT from y coord values.
        int offsetX = 0; //X- Offset of the sprite pixel cunting from top left corner
        int offsetY = 0;
    
    for (int i = 0; i < (hRes*vRes); i++)                                           //For every pixel of the image,
    {
        
        if (sprite[i]!=0x00)                                                        //(bar fully black pixels)...
        {
            drawPixelARGB32(topLeftX+offsetX,topLeftY+offsetY,sprite[i]);           //Draw that pixel on the framebuffer
        }
        
        
        offsetX++;                                                                  //After drawing, step to the pixel to the right 
        
        if ((i%hRes==0) && (i!=0))                                                  //IF the horizontal offset = horisontal size of the image, then
        {
            offsetY+=1;                                                             //Step DOWN the pixel row                        
            offsetX=0;                                                              //Reset the horizontal offset (back to x=0 in the image)
        }
        
    }

}

void eraseSpriteARGB32(int* sprite, int spr_width, int spr_height, int* background, int bck_width, int bck_height, int x, int y) 
{
    int totalPixels = spr_width * spr_height;

        int topLeftX = x-(spr_width/2); //Top left corner of the sprite, x-coord
        int topLeftY = y-(spr_height/2); //Top left corner of the sprite, y-coord. RPI follows the "Downwards Y-axis is positive" convention. I.e. to go up on the screen, you SUBTRACT from y coord values.
        int offsetX = 0; //X- Offset of the sprite pixel cunting from top left corner
        int offsetY = 0;

    for (int i = 0; i < (totalPixels); i++)                                           //For every pixel of the image,
    {
        
        int xoffs = topLeftX+offsetX;
        int yoffs = topLeftY+offsetY;

        if (sprite[i]!=0x00)                                                        //(bar fully black pixels)...
        {
            drawPixelARGB32(xoffs,yoffs,background[(yoffs*bck_width)+xoffs]);           //Draw that pixel on the framebuffer
        }
        
        
        offsetX++;                                                                  //After drawing, step to the pixel to the right 
        
        if ((i%spr_width==0) && (i!=0))                                                  //IF the horizontal offset = horisontal size of the image, then
        {
            offsetY+=1;                                                             //Step DOWN the pixel row                        
            offsetX=0;                                                              //Reset the horizontal offset (back to x=0 in the image)
        }
        
    }

}

void drawLineRGB32(int x1, int y1, int x2, int y2, int colour, int amountOfSamples) 
{ 
        //DANIIL SHLEE. V 1.0. 30/08/2022
            int xDraw = x1;
            int gradient = (y2-y1)/(x2-x1);
            
            int yDraw = y1;
            int hDist = x2-x1;
            unsigned int jump = hDist/amountOfSamples;

            for (int i = 0; i < amountOfSamples; i++)
            {
                
                drawPixelARGB32(xDraw,yDraw,colour);
                xDraw +=jump;
                yDraw = gradient*(xDraw);
            }
            



}

void gfx_flushScreen(int state) 
    { 
        //DANIIL SHLEE. V 1.0. 30/08/2022
        //drawRectARGB32(0,0,1920,1080,0,1); //BLANK
        //gfx_InitFrameBuffer() ;

        
        mBuf[0] = 8*4;          // Message Buffer Size in bytes (8 elements * 8 bytes (32 bit) each) ( AS PER THE MANUAL "Request: length: 8")
        mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message) 
        mBuf[2] = 0x00048001;   // TAG Identifier: LED TOGGLE
        mBuf[3] = 0;            // Value buffer size in bytes (max of request and response lengths) 
        mBuf[4] = 0;            // 0 = request. Response:
        mBuf[5] = 0;            // enable/disable blanking 
        mBuf[6] = 0;
        mBuf[7] = MBOX_TAG_LAST;            
       
        
        
        
    
        // Call Mailbox 
        if (mbox_call(ADDR(mBuf), MBOX_CH_PROP) ) //mailbox call is successful
            { 
                full_uart_puts("Screen flushed.\n");
                full_uart_hex(mBuf[5]);
            } 
        else 
            { 
                full_uart_puts("Unable to flush the frame buffer width/heihgt with provided setting\n"); 
            } 
        

        
    }




void gfx_FrameBufferRunColourTest()
{

    //DANIIL SHLEE. V 1.0. 30/08/2022
    drawRectARGB32(100,100,400,400,0x00AA0000,1); //RED 
 drawRectARGB32(150,150,400,400,0x0000BB00,1); //GREEN 
 drawRectARGB32(200,200,400,400,0x000000CC,1); //BLUE 
 drawRectARGB32(250,250,400,400,0x00FFFF00,1); //YELLOW 
 drawPixelARGB32(300, 300, 0x00FF0000); //RED 
 drawRectARGB32(0,0,100,100,0x00AA0000,1); //RED
 //drawSpriteARGB32(spriteBall,100,100,400,400);




}


/* Function to wait for some msec: the program will stop there */
void wait_msec(unsigned int n)
{
    register unsigned long f, t, r, expiredTime;
    // Get the current counter frequency (Hz)
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    
    // Read the current counter value
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    
    // Calculate expire value for counter
    expiredTime = t + ( (f/1000)*n )/1000;
    do {
     asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
    } while(r < expiredTime);

    
}



//Check if the user has just inputted a new key
//DANIIL SHLEE (edited)
unsigned int uart_isReadByteReady(){
return (*UART0_FR & UART0_FR_RXFE);
}

/* New function: Check and return if no new character, don't wait */
unsigned char getFullUart(){
    unsigned char ch = 0;
    if (!(uart_isReadByteReady()))
     ch = full_uart_getc();
    return ch;
}




void gfx_InitDoubleBuffer() 
{ 
    mBuf[0] = 35*4; // Length of message in bytes 
    mBuf[1] = MBOX_REQUEST; 
 
    mBuf[2] = MBOX_TAG_SETPHYWH; //Set physical width-height 
    mBuf[3] = 8; // Value size in bytes 
    mBuf[4] = 0; // REQUEST CODE = 0 
    mBuf[5] = 1024; // Value(width) 
    mBuf[6] = 768; // Value(height) 
 
    mBuf[7] = MBOX_TAG_SETVIRTWH; //Set virtual width-height 
    mBuf[8] = 8; 
    mBuf[9] = 0; 
    mBuf[10] = 2048; 
    mBuf[11] = 768; 
 
    mBuf[12] = MBOX_TAG_SETVIRTOFF; //Set virtual offset 
    mBuf[13] = 8; 
    mBuf[14] = 0; 
    mBuf[15] = 0; // x offset 
    mBuf[16] = 0; // y offset 
 
    mBuf[17] = MBOX_TAG_SETDEPTH; //Set color depth 
    mBuf[18] = 4; 
    mBuf[19] = 0; 
    mBuf[20] = COLOR_DEPTH; //Bits per pixel 
 
    mBuf[21] = MBOX_TAG_SETPXLORDR; //Set pixel order 
    mBuf[22] = 4; 
    mBuf[23] = 0; 
    mBuf[24] = PIXEL_ORDER; 
 
    mBuf[25] = MBOX_TAG_GETFB; //Get frame buffer 
    mBuf[26] = 8; 
    mBuf[27] = 0; 
    mBuf[28] = 16; //alignment in 16 bytes 
    mBuf[29] = 0;  //will return Frame Buffer size in bytes 
 
    mBuf[30] = MBOX_TAG_GETPITCH; //Get pitch 
    mBuf[31] = 4; 
    mBuf[32] = 0; 
    mBuf[33] = 0; //Will get pitch value here 
 
    mBuf[34] = MBOX_TAG_LAST; 
 
    // Call Mailbox 
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP) //mailbox call is successful ? 
     && mBuf[20] == COLOR_DEPTH //got correct color depth ? 
  && mBuf[24] == PIXEL_ORDER //got correct pixel order ? 
  && mBuf[28] != 0 //got a valid address for frame buffer ? 
  ) { 
 
     /* Convert GPU address to ARM address (clear higher address bits) 
      * Frame Buffer is located in RAM memory, which VideoCore MMU 
      * maps it to bus address space starting at 0xC0000000. 
      * Software accessing RAM directly use physical addresses 
      * (based at 0x00000000) 
     */ 
     mBuf[28] &= 0x3FFFFFFF; 
 
        // Access frame buffer as 1 byte per each address 
        fb = (unsigned char *)((unsigned long)mBuf[28]); 
        full_uart_puts("Got allocated Frame Buffer at RAM physical address: "); 
        full_uart_hex(mBuf[28]); 
        full_uart_puts("\n"); 
 
        full_uart_puts("Frame Buffer Size (bytes): "); 
        full_uart_dec(mBuf[29]); 
        full_uart_puts("\n"); 
 
        scrWidth = mBuf[5];      // Actual physical width 
        scrHeight = mBuf[6];      // Actual physical height 
        scrPitch = mBuf[33];       // Number of bytes per line 
 
    } else { 
     full_uart_puts("Unable to get a frame buffer with provided setting\n"); 

    } 
}

void gfx_setOffset(int x, int y)
{

//DANIIL SHLEE. V 1.0. 4/09/2022


        
        mBuf[0] = 8*4;          // Message Buffer Size in bytes (8 elements * 8 bytes (32 bit) each) ( AS PER THE MANUAL "Request: length: 8")
        mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message) 
        mBuf[2] = 0x00048009;   // TAG Identifier: Set v-offset
        mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths) 
        mBuf[4] = 0;            // 0 = request. Response:
        mBuf[5] = x;            
        mBuf[6] = y;
        mBuf[7] = MBOX_TAG_LAST;            
       
        
        
        
    
        // Call Mailbox 
        if (mbox_call(ADDR(mBuf), MBOX_CH_PROP) ) //mailbox call is successful
            { 
                full_uart_puts("Offset changed.\n");
                full_uart_dec(mBuf[5]);
            } 
        else 
            { 
                full_uart_puts("Unable to set offset with the provided parameters\n"); 
            } 
        

        


}