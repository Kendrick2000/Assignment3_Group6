// -----------------------------------uart.h -------------------------------------
//UNIVERSAL UART DEF FILE
//Daniil Shlee s341475
//Version 0.3 MBOX
/* Auxilary mini UART (UART1) registers */ 
#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004)) 
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040)) 
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044)) 
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048)) 
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C)) 
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050)) 
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054)) 
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058)) 
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C)) 
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060)) 
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064)) 
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068)) 


/*----full uart defs (UART0 to UART5 excl. 1)-----*/

#define UART0_BASE 0xFE201000
#define UART2_BASE 0xFE201400
#define UART3_BASE 0xFE201600
#define UART4_BASE 0xFE201800
#define UART5_BASE 0xFE201a00
//Replace UART0 below with UART 2,3,4, or 5 to use those uarts
#define UART0_DR ((volatile unsigned int*)(UART0_BASE+0x00000000))
#define UART0_RSRECR ((volatile unsigned int*)(UART0_BASE+0x04))
#define UART0_FR ((volatile unsigned int*)(UART0_BASE+0x18))

//Integer and Fractional divisor parts
#define UART0_IBRD ((volatile unsigned int*)(UART0_BASE+0x24))
#define UART0_FBRD ((volatile unsigned int*)(UART0_BASE+0x28))

//MAIN CONTROL REGISTERS!
#define UART0_LCRH ((volatile unsigned int*)(UART0_BASE+0x2c))
#define UART0_CR ((volatile unsigned int*)(UART0_BASE+0x30)) //Control Register for the transmitter/receiver

//UART Interrupt control registers
#define UART0_IFLS ((volatile unsigned int*)(UART0_BASE+0x34))
#define UART0_IMSC ((volatile unsigned int*)(UART0_BASE+0x38)) //Interrupt mask set clear register
#define UART0_RIS ((volatile unsigned int*)(UART0_BASE+0x34))
#define UART0_MIS ((volatile unsigned int*)(UART0_BASE+0x38)) //Masked interrupt register
#define UART0_ICR ((volatile unsigned int*)(UART0_BASE+0x34)) //Interrupt clear register

#define UART0_DMACR ((volatile unsigned int*)(UART0_BASE+0x38)) //
#define UART0_ITCR ((volatile unsigned int*)(UART0_BASE+0x34))

//Integration registers
#define UART0_ITIP ((volatile unsigned int*)(UART0_BASE+0x38)) //Integration test input reg
#define UART0_ITOP ((volatile unsigned int*)(UART0_BASE+0x34)) //Integration test output reg
//Data test registers
#define UART0_TDR ((volatile unsigned int*)(UART0_BASE+0x38)) //Data test reg
 
/* FR = Flags Register () */
#define UART0_FR_TXFE (1<<7)	/* TXFE = Transmit FIFO empty */
#define UART0_FR_RXFF (1<<6)	/* RXFF = Receive FIFO full   */
#define UART0_FR_TXFF (1<<5)	/* TXFF = Transmit FIFO full */
#define UART0_FR_RXFE (1<<4)	/* RXFE = Receive FIFO empty */
#define UART0_FR_BUSY (1<<3)	/* BUSY = UART is busy transmitting */
								/*  2 = DCD  = Unsupported */
								/*  1 = DSR  = Unsupported */
#define UART0_FR_CTS (1<<0)	/* CTS  = inverted version of nUARTCTS value */





/* MINI UART Function prototypes */ 
void uart_init(); 
void uart_sendc(unsigned char c); 
char uart_getc(); 
void uart_puts(char *s);

/* FULL UART Function prototypes */ 
void full_uart_init(); 
void full_uart_sendc(unsigned char c); 
unsigned char full_uart_getc(); 
void full_uart_puts(char *s);

void full_uart_hex(unsigned int d);
void full_uart_dec(int num);
unsigned char full_uart_getc_t();