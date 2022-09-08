// -----------------------------------gpio.h ------------------------------------- 
 
/*Peripheral physical address. It is 0xFE000000 in RBP4, 0x3F000000 in RBP3*/ 
//#define RBP3 //for emulation with QEMU RBP3 (disabled)
 
#ifdef RBP3 
    #define MMIO_BASE       0x3F000000  
#else //RBP4 
    #define MMIO_BASE       0xFE000000 
#endif 
 
#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE+0x00200000)) 
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004)) 
#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE+0x00200008)) 
#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE+0x0020000C)) 
#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE+0x00200010)) 
#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE+0x00200014)) 
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C)) 
#define GPSET1          ((volatile unsigned int*)(MMIO_BASE+0x00200020)) 
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028)) 
#define GPLEV0          ((volatile unsigned int*)(MMIO_BASE+0x00200034)) 
#define GPLEV1          ((volatile unsigned int*)(MMIO_BASE+0x00200038)) 
#define GPEDS0          ((volatile unsigned int*)(MMIO_BASE+0x00200040)) 
#define GPEDS1          ((volatile unsigned int*)(MMIO_BASE+0x00200044)) 
#define GPHEN0          ((volatile unsigned int*)(MMIO_BASE+0x00200064)) 
#define GPHEN1          ((volatile unsigned int*)(MMIO_BASE+0x00200068)) 
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094)) 
#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE+0x00200098)) 
#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE+0x0020009C))

#define GPIO_PUP_PDN_CNTRL_REG0 ((volatile unsigned int*)(MMIO_BASE+0x002000E4))
#define GPIO_PUP_PDN_CNTRL_REG1 ((volatile unsigned int*)(MMIO_BASE+0x002000E8))
#define GPIO_PUP_PDN_CNTRL_REG2 ((volatile unsigned int*)(MMIO_BASE+0x002000EC))
#define GPIO_PUP_PDN_CNTRL_REG3 ((volatile unsigned int*)(MMIO_BASE+0x002000F0))

//------------------TYPEDEFS------------------------
typedef unsigned char           uint8_t; 
typedef unsigned short int      uint16_t; 
typedef unsigned int            uint32_t; 
typedef unsigned long int       uint64_t;

#define cmdLine_setBLK_text full_uart_puts("\e[0;30m")
#define cmdLine_setRED_text full_uart_puts("\e[0;31m")
#define cmdLine_setGRN_text full_uart_puts("\e[0;32m")
#define cmdLine_setYLW_text full_uart_puts("\e[0;33m")
#define cmdLine_setBLU_text full_uart_puts("\e[0;34m")
#define cmdLine_setPRP_text full_uart_puts("\e[0;35m")
#define cmdLine_setCYN_text full_uart_puts("\e[0;36m")
#define cmdLine_setWHT_text full_uart_puts("\e[0;37m")

#define cmdLine_setBLK_bckgrnd full_uart_puts("\e[40m")
#define cmdLine_setRED_bckgrnd full_uart_puts("\e[41m")
#define cmdLine_setGRN_bckgrnd full_uart_puts("\e[42m")
#define cmdLine_setYLW_bckgrnd full_uart_puts("\e[43m")
#define cmdLine_setBLU_bckgrnd full_uart_puts("\e[44m")
#define cmdLine_setPRP_bckgrnd full_uart_puts("\e[45m")
#define cmdLine_setCYN_bckgrnd full_uart_puts("\e[46m")
#define cmdLine_setWHT_bckgrnd full_uart_puts("\e[47m")

#define cmdLine_standbyPrompt "user@METALOS> "