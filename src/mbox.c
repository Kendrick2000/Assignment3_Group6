// -----------------------------------mbox.c ------------------------------------- 
#include "mbox.h" 
#include "gpio.h" 
#include "uart.h" 
 
/* Mailbox Data Buffer (each element is 32-bit)*/ 
/* 
 * The keyword attribute allows you to specify special attributes 
 * 
 * The aligned(N) attribute aligns the current data item on an address 
 * which is a multiple of N, by inserting padding bytes before the data item 
 * 
 * __attribute__((aligned(16)) : allocate the variable on a 16-byte boundary. 
 * 
 * 
 * We must ensure that our our buffer is located at a 16 byte aligned address, 
 * so only the high 28 bits contain the address 
 * (last 4 bits is ZERO due to 16 byte alignment) 
 * 
*/ 
 
volatile unsigned int  __attribute__((aligned(16))) mbox[36]; 
 
 
/** 
 * Read from the mailbox 
 */ 
uint32_t mailbox_read(unsigned char channel) 
{ 
 //Receiving message is buffer_addr & channel number 
 uint32_t res; 
 // Make sure that the message is from the right channel 
 do { 
  // Make sure there is mail to receive 
  do{ 
   asm volatile("nop"); 
  }while(*MBOX0_STATUS & MBOX_EMPTY); 
 
  // Get the message 
  res = *MBOX0_READ; 
 } while ( (res & 0xF) != channel); 
 
 return res; 
} 
 
 
/** 
 * Write to the mailbox 
 */ 
void mailbox_send(uint32_t msg, unsigned char channel) 
{ 
 //Sending message is buffer_addr & channel number 
 
 // Make sure you can send mail 
 do{ 
  asm volatile("nop"); 
 }while(*MBOX1_STATUS & MBOX_FULL); 
 
 // send the message 
 *MBOX1_WRITE = msg; 
} 
 
 
 
/** 
 * Make a mailbox call. Returns 0 on failure, non-zero on success 
 */ 
int mbox_call(unsigned int buffer_addr, unsigned char channel) 
{ 
 //Check Buffer Address 
 uart_puts("Buffer Address: "); 
 uart_hex(buffer_addr); 
 uart_sendc('\n'); 
  
 //Prepare Data (address of Message Buffer) 
 unsigned int msg = (buffer_addr & ~0xF) | (channel & 0xF); 
 mailbox_send(msg, channel); 
 
 /* now wait for the response */ 
 /* is it a response to our message (same address)? */ 
 if (msg == mailbox_read(channel)) { 
  /* is it a valid successful response (Response Code) ? */ 
  if (mbox[1] == MBOX_RESPONSE) 
   uart_puts("Got successful response \n"); 
  
  return (mbox[1] == MBOX_RESPONSE); 
 } 
 
    return 0; 
} 

// Function allow users to set physical and virtual screen size.
void setScreenSize(char *array)
{
    // Decleare all variables.
    // assistance variables.
    int count = 0, check = 1, i = 0, j = 0;

    // Variable to store all 4 values of both physical and virtual
    int phWidth = 0, phHeight = 0;
    int virWidth = 0, virHeight = 0;

    // Identify width or height value for virtual and physical.
    int positionPhysical = 0, positionVirtual = 0;

    // Arrays to store all 4 values of both virtual and physical.
    char physicalWidth[10], physicalHeight[10];
    char virtualWidth[10], virtualHeight[10];

    // Looping through array
    while (array[count] != '\0')
    {
        // checking conditions to identify values for physical or virtual screen also checking for syntax correctness.
        if ((array[count] == '-' && array[count + 1] == 'p') && (array[count + 2] != '\0' || array[count + 2] != ' '))
        {
            // Set check value = 0 to identify code readable
            check = 0;
            // uart_puts("\n");
            // Continue looping through array until it reach the end or next set command
            while ((array[count] != '\0') && (array[count + 1] != '-'))
            {
                // Conditions to get width and height only
                if ((array[count] != ' ' && positionPhysical == 0) && (array[count] != '-' && array[count] != 'p'))
                {
                    // stores physical width value.
                    physicalWidth[i] = array[count];
                    // uart_sendc(physicalWidth[i]);
                    // Convert characters in physicalWidth array into number.
                    for (int k = 48; k <= 57; k++)
                    {
                        // Using ASCII table value to converts characters into number.
                        if (physicalWidth[i] == k)
                        {
                            phWidth = phWidth * 10 + (k - 48);
                        }
                    }
                    // Continue looping.
                    i++;
                    count++;
                    // Condition to identify finished getting width value.
                    if (array[count] == ' ')
                    {
                        positionPhysical = 1;
                    }
                }
                // Conditions to get height value only from the array.
                else if (array[count] != ' ' && positionPhysical == 1)
                {
                    // Store physical height value.
                    physicalHeight[i] = array[count];
                    // uart_sendc(physicalHeight[i]);
                    // Convert characters in physicalHeight array into number.
                    for (int h = 48; h <= 57; h++)
                    {
                        // Using ASCII table value to converts characters into number.
                        if (physicalHeight[i] == h)
                        {
                            phHeight = phHeight * 10 + (h - 48);
                        }
                    }
                    // Continue looping.
                    i++;
                    count++;
                }
                else
                {
                    // Continue looping until the end.
                    i++;
                    count++;
                }
            }
        }

        // checking conditions to identify values for physical or virtual screen also checking for syntax correctness.
        if ((array[count] == '-' && array[count + 1] == 'v') && (array[count + 2] != '\0' | array[count + 2] != ' '))
        {
            // Set check value = 0 to identify code readable
            check = 0;
            // uart_puts("\n");
            // Continue looping through array until it reach the end or next set command
            while ((array[count] != '\0') && array[count + 1] != '-')
            {
                // Conditions to get width and height only
                if ((array[count] != ' ' && positionVirtual == 0) && (array[count] != '-' && array[count] != 'v'))
                {
                    // stores virtual width value.
                    virtualWidth[j] = array[count];
                    // uart_sendc(virtualWidth[j]);
                    // Convert characters in virtualWidth array into number.
                    for (int l = 48; l <= 57; l++)
                    {
                        // Using ASCII table value to converts characters into number.
                        if (virtualWidth[j] == l)
                        {
                            virWidth = virWidth * 10 + (l - 48);
                        }
                    }
                    // Continue looping.
                    j++;
                    count++;
                    // Condition to identify finished getting width value.
                    if (array[count] == ' ')
                    {
                        positionVirtual = 1;
                    }
                }
                // Conditions to get height value only from the array.
                else if (array[count] != ' ' && positionVirtual == 1)
                {
                    // Store virtual height value.
                    virtualHeight[j] = array[count];
                    // uart_sendc(virtualHeight[j]);
                    // Convert characters in virtualHeight array into number.
                    for (int m = 48; m <= 57; m++)
                    {
                        // Using ASCII table value to converts characters into number.
                        if (virtualHeight[j] == m)
                        {
                            virHeight = virHeight * 10 + (m - 48);
                        }
                    }
                    // Continue looping.
                    j++;
                    count++;
                }
                else
                {
                    // Continue looping until the end.
                    j++;
                    count++;
                }
            }
            // uart_puts("\n");
        }
        // Continue looping until the end.
        count++;
    }

    // Condition to display error message
    if (check == 1)
    {
        uart_puts("Syntax error.\n");
    }

    // Condition to set physical width and height screen.
    if (positionPhysical == 1)
    {
        mbox[0] = 35 * 4;            // Length of message in bytes
        mbox[1] = MBOX_REQUEST;      // Message Request Code (this is a request message)
        mbox[2] = MBOX_TAG_SETPHYWH; // TAG Identifier: Set physical width-height
        mbox[3] = 8;                 // Value size in bytes
        mbox[4] = 0;                 // REQUEST CODE = 0
        mbox[5] = phWidth;           // New width for physical screen
        mbox[6] = phHeight;          // New height for physical screen
        mbox[7] = MBOX_TAG_LAST;
        mbox_call(ADDR(mbox), MBOX_CH_PROP);
    }
    // Condition to set virtual width and height screeen.
    else if (positionVirtual == 1)
    {
        mbox[0] = 35 * 4;             // Length of message in bytes
        mbox[1] = MBOX_REQUEST;       // Message Request Code (this is a request message)
        mbox[2] = MBOX_TAG_SETVIRTWH; // TAG Identifier: set virtual width-height
        mbox[3] = 8;                  // Value size in bytes
        mbox[4] = 0;                  // REQUEST CODE = 0
        mbox[5] = virWidth;           // New width for virtual screen
        mbox[6] = virHeight;          /// New height for virtual screen
        mbox[7] = MBOX_TAG_LAST;
        mbox_call(ADDR(mbox), MBOX_CH_PROP);
    }
    // Display error message when value empty.
    else
    {
        uart_puts("Command missing arguments.\n");
    }
}