// ----------------------------------- framebf.h ------------------------------------- 
void framebf_init(); 
void framebf_blank();
void framebf_release();
void drawPixelARGB32(int x, int y, unsigned int attr); 
void drawRectARGB32(int x1, int y1, int x2, int y2, unsigned int attr, int fill); 
void drawChar(int x, int y, char c, unsigned char attr);
void drawString(int x, int y, char *s, unsigned int attr);
void drawChar32x32(int x, int y, char c, unsigned char attr);
void drawString32x32(int x, int y, char *s, unsigned int attr);
void drawCharWelcome(int x, int y, char c, unsigned char attr);
void drawStringWelcome(int x, int y, char *s, unsigned int attr);