// -----------------------------------uart.c ------------------------------------- 
 
#include "uart_uni.h" 
#include "gpio.h" 
 
/** 
 * Set baud rate and characteristics (115200 8N1) and map to GPIO 
 */ 
void uart_init() 
{ 
    register unsigned int r; 
 
    /* initialize UART */ 
    *AUX_ENABLE |= 1;     //enable UART1, AUX mini uart (AUXENB) 
    *AUX_MU_CNTL = 0;    //stop transmitter and receiver 
    *AUX_MU_LCR  = 3;     //8-bit mode (also enable bit 1 to be used for RBP3) 
    *AUX_MU_MCR  = 0;    //RTS (request to send) 
    *AUX_MU_IER  = 0;    //disable interrupts 
    *AUX_MU_IIR  = 0xc6;  //clear FIFOs 
    *AUX_MU_BAUD = 270;   // 115200 baud   (system_clk_freq/(baud_rate*8) - 1) 
 
    /* map UART1 to GPIO pins */ 
    r = *GPFSEL1; 
 
 
    r &= ~( (7 << 12)|(7 << 15) ); //Clear bits 12-17 (gpio14, gpio15) 
    r |=    (2 << 12)|(2 << 15);   //Set value 2 (select ALT5: UART1) 
    *GPFSEL1 = r; 
 
    /* enable GPIO 14, 15 */ 
    *GPPUD = 0;            //No pull up/down control 
    
    r = 150; 
    while(r--) {asm volatile("nop");} //waiting 150 cycles 
    
    *GPPUDCLK0 = (1 << 14)|(1 << 15); //enable clock for GPIO 14, 15 
    
    r = 150; 
    while(r--) { asm volatile("nop"); } //waiting 150 cycles 
    *GPPUDCLK0 = 0;        // flush GPIO setup 
 
    *AUX_MU_CNTL = 3;      //Enable transmitter and receiver (Tx, Rx) 
} 
 
/** 
 * Send a character 
 */ 
void uart_sendc(unsigned char c) { 
    /* wait until transmitter is empty */ 
    do {
        asm volatile("nop");
    }while ( !(*AUX_MU_LSR & 0x20) ); 
 
    /* write the character to the buffer */ 
    *AUX_MU_IO = c; 
} 
 
/** 
 * Receive a character 
 */ 
char uart_getc() { 
    char c; 
 
    /* wait until data is ready (one symbol) */ 
    do { 
        asm volatile("nop"); 
    } while ( !(*AUX_MU_LSR & 0x01) ); 
 
    /* read it and return */ 
    c = (char)(*AUX_MU_IO); 
 
    /* convert carriage return to newline */ 
    return (c == '\r' ? '\n' : c); 
} 
 
/** 
 * Display a string 
 */ 
void uart_puts(char *s) { 
    while (*s) {  //While current array element is not 0 (or string terminator '\0', same int value), do:
        /* convert newline to carriage return + newline */ 
        if (*s == '\n') 
            uart_sendc('\r'); 
        uart_sendc(*s++); 
    } 
}

/* FULL UART CODE*/

void full_uart_init() {

    *UART0_CR &= ~(1<<0);//Disable UART (for tweaking)
    //for (int i = 0; i < 150; i++) {asm volatile("nop");} //wait for whatever current transmission it is to finish
    
    /*UART LINE CONTROL CONFIGURATION*/
        *UART0_LCRH &= ~(1<<4); //Flush fifo buffer (by disabling FIFO)
        *UART0_LCRH &= ~(1<<7); //Disable stick parity (just in case)
        *UART0_LCRH |= (0b11<<5); //Set 8 bit data frame
        *UART0_LCRH &= ~(1<<3); //1 stop bit
        *UART0_LCRH &= ~(1<<1); //no parity 
        
    /*UART MAIN CONTROL REGISTER CONFIGURATION*/
        *UART0_CR &= ~(1<<15); //Disable CTS hardware control
        *UART0_CR &= ~(1<<14); //Disable RTS hardware control
        *UART0_CR |= (1<<9); //Enable RX
        *UART0_CR |= (1<<8); //Enable TX
        *UART0_CR &= ~(1<<15); //Disable hardware control

        /* Mask all interrupts. Clear all the interrupts */
	    *UART0_IMSC = 0;
	    /* Clear pending interrupts. Clear all the interrupts */
        *UART0_ICR = 0x7FF;

        /*Set baudrate (default 115200)*/
           // int baudrateDiv_int = (48*1000000)/(16*9600); //Integer
           // float baudrateDiv_frac =((48*1000000)/(16*9600))-baudrateDiv_int; //Fractional

            *UART0_IBRD = 26;
            *UART0_FBRD = 3;

        /*map pins*/
            /*GPFSEL0 - SoC (not rpi) pins 0-9, GPFSEL1 - pins 10-19, GPFSEL2 - pins 20-29, 
            GPFSEL3 -pins 30-39, GPFSEL4 - pins 40-49, 
            GPFSEL5  - pins 50-57*/

            //for UART0: map to GPIO 14 ( RPY tx), 15 ( RPY RX)
            
            *GPFSEL1 &=~(1<<12);
            *GPFSEL1 &=~(1<<13);
            *GPFSEL1 |=(1<<14);

            *GPFSEL1 &=~(1<<15);
            *GPFSEL1 &=~(1<<16);
            *GPFSEL1 |=(1<<17);

        *UART0_LCRH |= (1<<4); //Enable FIFO(ez sends)
        *UART0_LCRH &=~(1<<1); //Disable parity bit (no parity)
        *UART0_CR |= (1<<0); //ACTIVATE UART CIRCUITRY

    
}

/** 
 * Send a character on FULL UART 
 */ 
void full_uart_sendc(unsigned char c) { 
      
    /* Check Flags Register */
	/* And wait until transmitter is not full */
	do {
		asm volatile("nop");
	} while (*UART0_FR & (1<<5));
 
    /* write the character to the buffer */ 
    *UART0_DR = c; 
}

void full_uart_puts(char *s) {
    while (*s) {
        /* convert newline to carriage return + newline */
        if (*s == '\n') {full_uart_sendc('\r');};
            
        full_uart_sendc(*s++);
    }
}

/** 
 * Display a binary value in hexadecimal 
 */ 
void full_uart_hex(unsigned int d) { 
    unsigned int n; 
    int c; 
 
    full_uart_puts("0x"); 
    for (c = 28; c >= 0; c = c - 4) { 
        // Get highest 4-bit nibble 
        n = (d >> c) & 0xF; 
 
        // 0-9 => '0'-'9', 10-15 => 'A'-'F' 
        n += (n > 9) ? (- 10 + 'A') : '0'; 
        full_uart_sendc(n); 
    } 
} 
 
/** 
 * Display a value in decimal by 
 */ 
void full_uart_dec(int num) 
{ 
 char str[33] = ""; 
 int i, rem, len = 0, n; 
 
    n = num; 
    while (n != 0){ 
        len++; 
        n /= 10; 
    } 
 
    if (num == 0) 
     len = 1; 
 
    for (i = 0; i < len; i++){ 
        rem = num % 10; 
        num = num / 10; 
        str[len - (i + 1)] = rem + '0'; 
    } 
    str[len] = '\0'; 
 
    full_uart_puts(str); 
  
}

//Receiving characters:
unsigned char full_uart_getc() {
    char c = 0;

    /* Check Flags Register */
    /* Wait until Receiver is not empty
     * (at least one byte data in receive fifo)*/
	do {
		asm volatile("nop");
    } while (*UART0_FR & UART0_FR_RXFE  );

    /* read it and return */
    c = (unsigned char) (*UART0_DR);

    /* convert carriage return to newline */
    return (c == '\r' ? '\n' : c);
}
