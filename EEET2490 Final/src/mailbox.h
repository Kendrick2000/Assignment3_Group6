/*--------------Mailbox defs-----------------------------------------*/
#include "gpio.h"

#define VIDEOCORE_MBOX ((MMIO_BASE + 0x0000B880)) //Base address of the GPU vCore, offset from the peripheral base

/* a properly aligned buffer */
//Announce valriable name for mBuf[36] array, which will be used as a buffer. 
//Announcing 'reserves and declares' the variable's NAME for all subsequent source files taht include mailbox.h
extern volatile unsigned int mBuf[36]; //36 is just an arbitrary number. In fact, the array size is only limited by available RAM.

//Create a conversion macro for casting 64-bit values (long) into 32-bit values suitable for the 32 bit buffer (int)
#define ADDR(X) (unsigned int)((unsigned long) X) //Will convert long X (64 bit) into int X (32 bit)


//mailbox register definitions:
#define MBOX0_READ ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x00))
#define MBOX0_PEEK ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x10))
#define MBOX0_SENDER ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x14))
#define MBOX0_STATUS ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x18))
#define MBOX0_CONFIG ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x1C))
#define MBOX1_WRITE ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x20))
#define MBOX1_PEEK ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x30))
#define MBOX1_SENDER ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x34))
#define MBOX1_STATUS ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x38)) //Bit 30 of this register can tell whether the Read register is empty, Bit 31 = full
#define MBOX1_CONFIG ((volatile unsigned int*)(VIDEOCORE_MBOX + 0x3C))



#define ARM_MS_EMPTY BIT(30)
/*Configuration register: Enable interrupts. */
#define ARM_MC_IHAVEDATAIRQEN BIT(0)



//Request/Response code in Buffer content
#define MBOX_RESPONSE 0x80000000
#define MBOX_ERROR 0x80000001
#define MBOX_REQUEST 0

//Status Value (from Status Register)
#define MBOX_FULL 0x80000000 //0b1000 0000 0000 0000 0000 0000 0000 0000
#define MBOX_EMPTY 0x40000000

/* channels */
#define MBOX_CH_POWER 0 //Power management
#define MBOX_CH_FB 1 //Frame buffer
#define MBOX_CH_VUART 2 //Virtual UART
#define MBOX_CH_VCHIQ 3 //VCHIQ
#define MBOX_CH_LEDS 4 //LEDs
#define MBOX_CH_BTNS 5 //Buttons
#define MBOX_CH_TOUCH 6 //Touch screen
#define MBOX_CH_PROP 8 //Property tags (ARM -> VC)


/* tags */
#define MBOX_TAG_GETSERIAL 0x00010004 //Get board serial
#define MBOX_TAG_GETMODEL 0x00010001 //Get board model
#define MBOX_TAG_SETCLKRATE 0x00038002
#define MBOX_TAG_LAST 0

//Graphics mode screen defines
#define MBOX_TAG_SETPHYWH   0x48003 
#define MBOX_TAG_SETVIRTWH  0x48004 
#define MBOX_TAG_SETVIRTOFF 0x48009 
#define MBOX_TAG_SETDEPTH   0x48005 
#define MBOX_TAG_SETPXLORDR 0x48006 
#define MBOX_TAG_GETFB      0x40001 
#define MBOX_TAG_GETPITCH   0x40008



/* Function Prototypes */
int mbox_call(unsigned int buffer_addr, unsigned char channel);