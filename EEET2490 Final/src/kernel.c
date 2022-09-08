// -----------------------------------main.c ------------------------------------- 
#include "mailbox.h"
#include "uart_uni.h"
#include "gfx_frameBuffer.h"
#include "sys_cmdLine.h"
//#include "sys_cmdLine.h"
//#include "sys_sysStates.h"



/*#include "../gcclib/stddef.h"
#include "../gcclib/stdint.h"
#include "../gcclib/stdarg.h"*/








void main() 
{ 
    // set up serial comms hardware (UART0):
    full_uart_init(); 
    //full_uart_puts("BOOTLOADER: Passing onto the main routine...");
    char *osLogo = "\n███╗░░░███╗███████╗████████╗░█████╗░██╗░░░░░░█████╗░░██████╗\n████╗░████║██╔════╝╚══██╔══╝██╔══██╗██║░░░░░██╔══██╗██╔════╝\n██╔████╔██║█████╗░░░░░██║░░░███████║██║░░░░░██║░░██║╚█████╗░\n██║╚██╔╝██║██╔══╝░░░░░██║░░░██╔══██║██║░░░░░██║░░██║░╚═══██╗\n██║░╚═╝░██║███████╗░░░██║░░░██║░░██║███████╗╚█████╔╝██████╔╝\n╚═╝░░░░░╚═╝╚══════╝░░░╚═╝░░░╚═╝░░╚═╝╚══════╝░╚════╝░╚═════╝░\n";
    

    full_uart_puts(osLogo);
    // "ඞ ඞ  ඞ  ඞ " sus

    full_uart_puts("\n");

    full_uart_puts("MetalOS, EEET 2490 bare-metal OS for Raspberry PI 4.\nDaniil Shlee s3414755\nREV. 0.82.1 GFX\nMOTD:");
    full_uart_puts("UART_t implemented and in testing.\n"); //MOTD

    cmdLine_setRED_text;
    full_uart_puts("----------------------INIT START--------------------------\n");
    // mailbox data buffer: Read ARM frequency (arrays are ALWAYS pointers, wtf lol)
    mBuf[0] = 8*4;          // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each) 
    mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message) 
    mBuf[2] = 0x00030002;   // TAG Identifier: Get clock rate 
    mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths) 
    mBuf[4] = 0;            // REQUEST CODE = 0 
    mBuf[5] = 3;            // clock id: ARM system clock 
    mBuf[6] = 0;            // clear output buffer (response data are mBuf[5] & mBuf[6]) 
    mBuf[7] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) //comma inside if: first statement evaluated, result discarded. Second statement is evaluated, result returned and affects outcome of IF
        { 
            full_uart_puts("Response Code for whole message: "); 
            full_uart_hex(mBuf[1]); 
            full_uart_puts("\n"); 
            
            full_uart_puts("Response Code in Message TAG: ");
            full_uart_hex(mBuf[4]); 
            full_uart_puts("\n"); 
            
            full_uart_puts("DATA: ARM clock rate = ");
            full_uart_dec(mBuf[6]); 
            full_uart_puts("\n");

            cmdLine_setGRN_bckgrnd;
            full_uart_puts("----------------------INIT OK--------------------------"); 
            
            cmdLine_setBLK_bckgrnd;
            full_uart_puts("\n");
            cmdLine_setWHT_text;
        
    } 

    else 
        { 
            cmdLine_setRED_bckgrnd;
            cmdLine_setBLK_text;
            full_uart_puts("Unable to query! Troubleshoot the kernel code for mailbox and/or init.\n"); 
        }
 
       
    // echo everything back 
    full_uart_puts("Now entering shell mode: \n");


    while(1)  //SHELL
    { 
     //Listen for user input on loop:
        int shell_initStatus = 0;
        cmdLine_InitInputBuffer();
        cmdLine_InitCommandBuffer();
        cmdLine_InitParamBuffer();
        
        shell_initStatus = 1;

        while(shell_initStatus==1){
        
            char c = full_uart_getc();
            
            //*(inputBuffer+inputBufferLen) = c; //Change the data being pointed to by inputBuffer (our command that is being typed)
            
            //Regular characters:
            if (c!='\n' && c!='\r' && c!=backspaceChar)
            {
                full_uart_sendc(c); //Echo the character back

                *(&inputBuffer[0]+inputBufferLen) = c; 
                //* "the value at". (THIS IS EVALUATION USE OF *, NOT DECLARATION. WHEN USED IN DECLARATIONS, * INDICATES THAT WE ARE DECLARING A POINTER. IT DOESNT PRODUCE A VALUE AS IT DOES IN THIS CASE!)
                //& "the address of". 
                //inputBuffer[0] "zeroeth element of arrayinputBuffer". 
                //+inputBufferLen "with offset of  inputBufferLen elements". 
                //= "set it  to". 
                //c "value of variable c"
                inputBufferLen += 1; //Increment last element index 
            }

            //Backspace erasing
            if (c==backspaceChar && inputBufferLen > 0) //Ctrl+? (Ascii 127). Only works if inputBuffer has something in there besides \0
            {
                full_uart_sendc(c);
                //*(&inputBuffer[0]+inputBufferLen) = '\0'; //erase the character in the command buffer
                inputBufferLen -= 1; //take a step back
                *(&inputBuffer[0]+inputBufferLen) = '\0';  
            }

            //Newline character (enter pressed)
            if (c=='\n' || c=='\r') // \n character received (which means Enter was pressed)
            {
                cmdLine_setBLK_text;
                cmdLine_setWHT_bckgrnd;
                
                
                full_uart_puts("\n[DEBUG]Parsed input length is:");
                full_uart_dec(cmdLine_GetInputBufferLength());
                full_uart_puts("\n[DEBUG]inputBuffer size is:");
                full_uart_dec(inputBufferLen);

                full_uart_puts("\n[DEBUG]Parsed command is:");
                strUtil_CopyString(inputBuffer,cmdBuffer,' ',0,1,0);
                full_uart_puts(cmdBuffer);
                full_uart_puts("\n[DEBUG]cmdBuffer size is:");
                full_uart_dec(cmdLine_GetCommandBufferLength());
                full_uart_puts("\n[DEBUG]cmdBuffer contents are:");
                full_uart_puts(cmdBuffer);
                cmdLine_setBLK_bckgrnd;
                cmdLine_setWHT_text;
                full_uart_sendc('\n');


                //full_uart_puts(inputBuffer); //Echo back the string
                //inputBufferLen = 0; //Reset char index in inputBuffer
                cmdLine_RunCommand(inputBuffer);

                full_uart_sendc(c); //Send the newline character to the PC terminal so it advances.
                shell_initStatus = 0;
            }
        } //End of input loop code

    } //End of SHELL CODE


} //End of main
