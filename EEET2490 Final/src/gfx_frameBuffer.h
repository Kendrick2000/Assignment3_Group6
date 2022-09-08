//INCLUDE THIS IN YOUR KERNEL!

void gfx_InitFrameBuffer(); 
void gfx_FrameBufferSetVirtualScreen(int w, int h); 
void gfx_FrameBufferSetPhysicalScreen(int w, int h);
void gfx_FrameBufferRunColourTest();
void gfx_flushScreen(int state);
void wait_msec(unsigned int n);
unsigned int uart_isReadByteReady();
unsigned char getFullUart();
void gfx_InitDoubleBuffer() ;
void gfx_setOffset(int x, int y);

void drawPixelARGB32(int x, int y, unsigned int attr); 
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill);
void drawSpriteARGB32(int* sprite, int hRes, int vRes, int x, int y);
void drawLineRGB32(int x1, int y1, int x2, int y2, int colour, int amountOfSamples);
void eraseSpriteARGB32(int* sprite, int spr_width, int spr_height, int* background, int bck_width, int bck_height, int x, int y) ;

