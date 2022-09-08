//This entire thing is a practice run with pointers.
#include "uart_uni.h"
#include "mailbox.h"
#include "gfx_frameBuffer.h"
#include "sys_cmdLine.h"
#include "sprite.h"
#include "gaem.h"
#include "back1.h"







//Command list:

void sus();     //Quick game of Among Us
void ping();    //Pings GPU for a reply
void mboxtest(); //Shows availability of mailbox buffers
void cpuinfo(); //Shows CPU clock speed
void raminfo(); //Shows available RAM
void statusled(); //Toggles the yellow status LED ON/OFF
void cls();
void help();
void setcolour();
void brdev();
void scrsize();
void hdmitest();
void gametest();





/*--------------------------------------------------------------------------------------------------
   _____ _    _ ______ _      _        _    _ _______ _____ _      _____ _______ _____ ______  _____ 
  / ____| |  | |  ____| |    | |      | |  | |__   __|_   _| |    |_   _|__   __|_   _|  ____|/ ____|
 | (___ | |__| | |__  | |    | |      | |  | |  | |    | | | |      | |    | |    | | | |__  | (___  
  \___ \|  __  |  __| | |    | |      | |  | |  | |    | | | |      | |    | |    | | |  __|  \___ \ 
  ____) | |  | | |____| |____| |____  | |__| |  | |   _| |_| |____ _| |_   | |   _| |_| |____ ____) |
 |_____/|_|  |_|______|______|______|  \____/   |_|  |_____|______|_____|  |_|  |_____|______|_____/ 
                                                                                                     
*/                                                                                                     


void cmdLine_SetDelCharacter(int asciiDecCode)
    {
        backspaceChar = asciiDecCode;       //ASCII DEL

    }

void cmdLine_InitInputBuffer()
    {
        for (int i = 0; i < 256; i++)       //Flush the command buffer
                {
                    inputBuffer[i] = '\0';
                } 
        inputBufferLen = 0;                   //INDEX of las nonzero, non-newline element in an array. INDEX!!!! Not a length!
        full_uart_puts(cmdLine_standbyPrompt); 
    }


void cmdLine_InitCommandBuffer()
    {
        for (int i = 0; i < 256; i++)       //Flush the input buffer
                {
                    cmdBuffer[i]  = '\0';
                } 
    }

void cmdLine_InitParamBuffer()
{
    for (int i = 0; i < cmdParamBufferMaxParams; i++) //Flush parameter list
        {
            for (int j = 0; j < 20; j++)
            {
                cmdParamBuffer[i][j] = '\0';
            }

        }

}

int cmdLine_GetInputBufferLength()       //should be called every time enter is pressed while system is in the shell state               
{
    char *operatedOnInputPointer;           //Temporary pointer to store the inputBuffer in so inputBuffer's original value remains unchanged.
    operatedOnInputPointer = inputBuffer;     //Equating the pointers, so original inputBuffer value is preserved (dont use asterisk lol.)
    int inputLength = 0;                    //Output variable

    while (*operatedOnInputPointer!='\0')         //check every character until space or null terminator show up.
    {
        inputLength++;
        operatedOnInputPointer++;           //move to the next char by incrementing the pointer. * is NOT necessary, compiler already knows it is a pointer.
    }

    return inputLength;                     //return the value of command length before space is encountered. 
}

int cmdLine_GetCommandBufferLength()       //should be called every time enter is pressed while system is in the shell state               
{
    char *operatedOnInputPointer;           //Temporary pointer to store the inputBuffer in so inputBuffer's original value remains unchanged.
    operatedOnInputPointer = cmdBuffer;     //Equating the pointers, so original inputBuffer value is preserved (dont use asterisk lol.)
    int cmdLength = 0;                    //Output variable

    while (*operatedOnInputPointer!='\0' && *operatedOnInputPointer!=' ')         //check every character until space or null terminator show up.
    {
        cmdLength++;
        operatedOnInputPointer++;           //move to the next char by incrementing the pointer. * is NOT necessary, compiler already knows it is a pointer.
    }

    return cmdLength;                     //return the value of command length before space is encountered. 
}

 

void cmdLine_LoadCommandList()
{
        //Create a lookup list of functions ("app list", lol)

        
        cmdList[0] = "sus";
        cmdList[1] = "ping";
        cmdList[2] = "cls";
        cmdList[3] = "help";
        cmdList[4] = "setcolour";
        cmdList[5] = "brdev";
        cmdList[6] = "scrsize";
        cmdList[7] = "cpuinfo";
        cmdList[8] = "raminfo";
        cmdList[9] = "statusled";
        cmdList[10] = "hdmitest";
        cmdList[11] = "gametest";


        fun_ptr[0] = &sus; //Address of the function "sus", basically a pointer, which makes fun_ptr a pointerlist
        fun_ptr[1] = &ping;
        fun_ptr[2] = &cls;
        fun_ptr[3] = &help;
        fun_ptr[4] = &setcolour;
        fun_ptr[5] = &brdev;
        fun_ptr[6] = &scrsize;
        fun_ptr[7] = &cpuinfo;
        fun_ptr[8] = &raminfo;
        fun_ptr[9] = &statusled;
        fun_ptr[10] = &hdmitest;
        fun_ptr[11] = &gametest;

}

void cmdLine_ParseParameters(char *source)
{

    //Extract parameters from the raw input of the user.
        int cmdLen = strUtil_GetStringLength(source,1); //Get the full length of the string incl spaces
        int firstSpace = 0;
        int paramCount = 0;
        int paramCharCount = 0;

        cmdLine_InitParamBuffer();
        

        for (int i = 0; i < cmdLen; i++)
        {

            
            if (source[i]!=' ' && firstSpace>0)
            {
                cmdParamBuffer[paramCount][paramCharCount]=source[i];
                paramCharCount++;
            }

            if (source[i]==' ' && firstSpace>0)
            {
                paramCount++;
                paramCharCount=0;
            }

            if (source[i]==' ')
            {
                firstSpace = 1;
                //full_uart_puts("1st space");
            }
 

        }
        
        //Print all detected parameters:
        
        full_uart_puts("[DEBUG]Parameters detected:\n");
        for (int i = 0; i < cmdParamBufferMaxParams; i++) //Flush parameter list
        {
            
            full_uart_puts(cmdParamBuffer[i]);
            full_uart_sendc('\n');
        }
        

}


void cmdLine_RunCommand(char *source)
{
    
    strUtil_CopyString(source,cmdBuffer,' ',0,1,0); //Extract command syntax from the raw input of the user
    cmdLine_LoadCommandList();

    cmdLine_ParseParameters(source);
    


    //Find and execute the command from the list:
    for (int i = 0; i < cmdListSize; i++) //For every entry on the command list
    {

        if (strUtil_CompareStrings(cmdBuffer,cmdList[i])==1)
        {
                
                cmdLine_setRED_text;
                cmdLine_setGRN_bckgrnd;
                full_uart_puts("trying to execute command:");
                full_uart_sendc(' ');
                cmdLine_setBLK_bckgrnd;
                cmdLine_setWHT_text;

                full_uart_puts(cmdList[i]);
                (fun_ptr[i])(0,0,0,0,0);  //This runs function from the pointer list fun_ptr with the index of i 
                
        }
        

    }
    

} //runCommand END


/*----------------------------------------------------------------------------------------------------
   _____ _______ _____  _____ _   _  _____         _    _ _______ _____ _       _____ 
  / ____|__   __|  __ \|_   _| \ | |/ ____|       | |  | |__   __|_   _| |     / ____|
 | (___    | |  | |__) | | | |  \| | |  __        | |  | |  | |    | | | |    | (___  
  \___ \   | |  |  _  /  | | | . ` | | |_ |       | |  | |  | |    | | | |     \___ \ 
  ____) |  | |  | | \ \ _| |_| |\  | |__| |       | |__| |  | |   _| |_| |____ ____) |
 |_____/   |_|  |_|  \_\_____|_| \_|\_____|        \____/   |_|  |_____|______|_____/ 


*/


//Erases string by zeroing-out the memory (non-destructively, pointer value is not affected)
void strUtil_EraseString(char* string)
{
 
    int i =0;
    while (string[i]!='\0')
    {
        string[i]='\0';
        i++;
    }

}

//This function copies characters from source string to destination string. Destination string IS MODIFIED!
//Can also be used to extract commands from the source string if stopChar is set to value besides '\0', i.e. ' '
//source - source string copied FROM (pointer)
//dest - destination string copied TO (pointer)
//stopChar - character at which to stop copying process. Set to '\0' to copy the full string.
//accountForLengthDifferenceYN - copying will stop whenever the end of string is reached for dest. Ignored if overwriteDest is set.
//wipeDestYN - wipe the memory used for the destination BEFORE copying from source. If not set, data that was in the dest before may show up.
//wipeSourceYN -  wipe the memory used for the source AFTER copying has finished.
void strUtil_CopyString(char* source, char* dest, char stopChar, int accountForLengthDifferenceYN, int wipeDestYN, int wipeSourceYN)                    
{
      
    int i =0;
    int stop = 0;

    //Wipe destination string before copying if overwriteDest is set
    if (wipeDestYN==1) 
    {
        int destLength = strUtil_GetStringLength(dest,1);
        for (i = destLength; i < 0; i++)
        {
            dest[i]='\0';
        }
    }


    while (source[i]!='\0' && source[i]!=stopChar && stop==0)        //Iterate through every character until null terminator or stopChar defined by user show up.
    {
        
        if (wipeDestYN==0 &&accountForLengthDifferenceYN==1 && dest[i]=='\0') {stop=1;} // stop execution and return destination string immediately if dest<source in length
        
        if (stop ==0){
        dest[i] = source[i]; //Copy the character from one string to another, non-destructively (without affecting the pointer)
     
        if (wipeSourceYN==1) {source[i]='\0';} //Erase the source string character after copying.
        i++;}
        
    }


    
}

void strUtil_CopyStringToArray(char* source, char* dest[], char stopChar, int accountForLengthDifferenceYN, int wipeDestYN, int wipeSourceYN)                    
{
      
    int i =0;
    int stop = 0;




    while (source[i]!='\0' && source[i]!=stopChar && stop==0)        //Iterate through every character until null terminator or stopChar defined by user show up.
    {
        
        
        
       
        (dest[0])[i] = source[i]; //Copy the character from one string to another, non-destructively (without affecting the pointer)
     
        
        
        
    }


    
}


int strUtil_GetStringLength(char *string, int countSpacesYN)                    //should be called every time enter is pressed while system is in the shell state               
{
    char *tempPointer;                      //Temporary pointer to store the inputBuffer in so inputBuffer's original value remains unchanged.
    tempPointer = string;                   //Equating the pointers, so original inputBuffer value is preserved (dont use asterisk lol.)
    int stringLength = 0;                   //Output variable

    if (countSpacesYN==0)                   //Do not count spaces, stop execution the moment space is encountered.
    {
        while (*tempPointer!='\0' && *tempPointer!=' ')         //check every character until space or null terminator show up.
        {
            stringLength++;
            tempPointer++;                  //move to the next char by incrementing the pointer. * is NOT necessary, compiler already knows it is a pointer.
        }
    }

    if (countSpacesYN==1)                   //Count spaces as characters. Check every character until space or null terminator show up.
    {
        while (*tempPointer!='\0')          
        {
            stringLength++;                 //Increment string length counter.
            tempPointer++;                  //move to the next char by incrementing the pointer. * is NOT necessary, compiler already knows it is a pointer.
        }
    }
    
    return stringLength;                    //return the value of command length before space is encountered. 
} 


int strUtil_CompareStrings(char *string1, char *string2)                 
{
    char *tempPointer1;         
    tempPointer1 = string1;     //Equating the pointers, so original inputBuffer value is preserved (dont use asterisk lol.)
    char *tempPointer2;           
    tempPointer2 = string2;     //Equating the pointers, so original inputBuffer value is preserved (dont use asterisk lol.)
    
    int string1Len = 0;
    int string2Len = 0;

    //int stringCompScore = 0;    //DEBUG: This must be equal to the length of both strings to register a match.
    int matchYN = 0;            //Output. 1 = strings match. 0 = strings dont match.

    string1Len = strUtil_GetStringLength(string1,1);
    string2Len = strUtil_GetStringLength(string2,1);


    //Get the easiest case out of the way: strings are of the different length, therefore they dont match.
    if (string1Len!=string2Len) //Lengths of the string are not equal, they cannot be the same. Return 0 (not matching)
    {
        matchYN=0;
        return matchYN;
    }//EndIF<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    
    //If the strings are of the same lengths, then do per-character comparison.
    if  (string1Len==string2Len)
    {
        while(*tempPointer1)
            {
                if (*tempPointer1 != *tempPointer2)     //Check every symbol of both strings against each-other. If the disrepancy is detected, return 0 (non matching).
                    {
                        matchYN=0;
                        return matchYN;
                    }
                    tempPointer1++;
                    tempPointer2++;
            }
        
        matchYN=1; //if the above while-loop did not result in return 0 (did not detect mismatch), return 1 (strings are matching).
        return matchYN;

    } //EndIF<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    
    //Error case: none of the above loops returned anything for some reason.
    matchYN = 999;
    full_uart_puts("FATAL ERROR: Strings could not be parsed (999). Check the input for unsupported characters.");
    return 999;             //return the value of command length before space is encountered. 
}


int strUtils_IsStringPresentInArray(char* string, char* array[],int arraySize)
{
    for (int i = 0; i < arraySize; i++)
    {
        if (strUtil_CompareStrings(string,array[i])==1)
        {
            return 1;


        }
    }
    return 0; //if function doesnt return from for loop, that means there was no match found.
    


}

int strUtils_StringToInt(char* string)
{
    int i=0,sum=0;

    while(string[i]!='\0'){
         if(string[i]< 48 || string[i] > 57){
            full_uart_puts("Unable to convert it into integer.\n");
             return 0;
         }
         else{
             sum = sum*10 + (string[i] - 48);
             i++;
         }

    }

    return sum;
}



/*----------------------------------------------------------------------------------------------------

   _____ _    _ ______ _      _         _____ ____  __  __ __  __          _   _ _____   _____ 
  / ____| |  | |  ____| |    | |       / ____/ __ \|  \/  |  \/  |   /\   | \ | |  __ \ / ____|
 | (___ | |__| | |__  | |    | |      | |   | |  | | \  / | \  / |  /  \  |  \| | |  | | (___  
  \___ \|  __  |  __| | |    | |      | |   | |  | | |\/| | |\/| | / /\ \ | . ` | |  | |\___ \ 
  ____) | |  | | |____| |____| |____  | |___| |__| | |  | | |  | |/ ____ \| |\  | |__| |____) |
 |_____/|_|  |_|______|______|______|  \_____\____/|_|  |_|_|  |_/_/    \_\_| \_|_____/|_____/  (or "apps", XD)
                                                                                            

*/


//This function is kinda sus.
void sus()
{
    //Help section:
    if (strUtil_CompareStrings(cmdBuffer,"help")==1)
    {
        cmdLine_setRED_text;
        cmdLine_setGRN_bckgrnd;
        full_uart_puts("\n-------------------------------");
        full_uart_puts("\nSHOWING HELP FOR: sus");
        full_uart_puts("\n-------------------------------");
        cmdLine_setBLK_bckgrnd;
        cmdLine_setWHT_text;
        full_uart_puts("\nsus : Play a quick game of Among Us.\nUsage: sus");
    }

    //Actual command actions:
    else
    {
            full_uart_puts("\nඞ ඞ  ඞ  ඞ AMOGUS \n");  
    }
    

}

//Tests basic IO functionality.
void ping()
{
        //Help section:
        if (strUtil_CompareStrings(cmdBuffer,"help")==1)
        {
            cmdLine_setRED_text;
            cmdLine_setGRN_bckgrnd;
            full_uart_puts("\n-------------------------------");
            full_uart_puts("\nSHOWING HELP FOR: ping");
            full_uart_puts("\n-------------------------------");
            cmdLine_setBLK_bckgrnd;
            cmdLine_setWHT_text;

            full_uart_puts("\nping : Test basic CLI I/O. Usage: ping");
        }
        //Actual command actions:
        else
        {
            full_uart_puts("\nPONG!\n");  
            gfx_InitFrameBuffer();
        }
    
    
}

//Clears terminal window (hopefully).
void cls()//DONE
{
        if (strUtil_CompareStrings(cmdBuffer,"help")==1)
        {
            cmdLine_setRED_text;
            cmdLine_setGRN_bckgrnd;
            full_uart_puts("\n-------------------------------");
            full_uart_puts("\nSHOWING HELP FOR: cls");
            full_uart_puts("\n-------------------------------");
            cmdLine_setBLK_bckgrnd;
            cmdLine_setWHT_text;            
            full_uart_puts("\ncls : Clears terminal window.\nUsage: cls");
        }
        //Actual command actions:
        else
        {
            full_uart_puts("\033[2J"); //Clear terminal window.  
        }


}

void help()//DONE
{
        //Help section:
        if (strUtil_CompareStrings(cmdParamBuffer[0],"help")==1)
        {
                cmdLine_setRED_text;
                cmdLine_setGRN_bckgrnd;
                full_uart_puts("\n-------------------------------");
                full_uart_puts("\nSHOWING HELP FOR: help");
                full_uart_puts("\n-------------------------------");
                cmdLine_setBLK_bckgrnd;
                cmdLine_setWHT_text;
            
            full_uart_puts("\nhelp : Shows general and command-specific help topics and hints.\nUsage: help <command>.\nIf no arguments are given, lists system-wide command list.");
            //return 0;

        }

        //Actual command actions:
        if (strUtil_CompareStrings(cmdParamBuffer[0],"help")!=1)
        {
            cmdLine_LoadCommandList();                          //Load all available commands
            if (strUtil_GetStringLength(cmdParamBuffer[0],0)>0) //Check if any parameter was given
                {
                    for (int i = 0; i < cmdListSize; i++)
                    {
                        if (strUtil_CompareStrings(cmdParamBuffer[0],cmdList[i])==1)
                        {
                            
                            (fun_ptr[i])(0,0,0,0,0); //Run shell command function in the help mode (cmdBuffer = "help"). In this mode functions will show the built-in help instead of acting normally.

                        }
                    }
        
                }
            else if (strUtil_GetStringLength(cmdParamBuffer[0],0)==0)
                {
                    cmdLine_setRED_text;
                    cmdLine_setGRN_bckgrnd;
                    full_uart_puts("\n-----------------------------------------");
                    full_uart_puts("\nSHOWING HELP FOR: SYSTEMWIDE COMMAND LIST");
                    full_uart_puts("\n-----------------------------------------");
                    cmdLine_setBLK_bckgrnd;
                    cmdLine_setWHT_text;
                    full_uart_puts("\nListing all available commands:\n\n");
                    cmdLine_LoadCommandList();
                    
                    for (int i = 0; i < cmdListSize; i++)
                    {
                        full_uart_puts(cmdList[i]);
                        full_uart_sendc('\n');
                    }
                    full_uart_puts("\nType:\n      help <command>\nto learn about a particular command.\n");
                }
        }
        
    
}

void setcolour()//DONE
{
        if (strUtil_CompareStrings(cmdBuffer,"help")==1)
        {
            cmdLine_setRED_text;
            cmdLine_setGRN_bckgrnd;
            full_uart_puts("\n-------------------------------");
            full_uart_puts("\nSHOWING HELP FOR: setcolour");
            full_uart_puts("\n-------------------------------");
            cmdLine_setBLK_bckgrnd;
            cmdLine_setWHT_text;            
            full_uart_puts("\nsetcolour : Changes colours of terminal window text and background.\nUsage: \n    setcolour -t <colour> -b <colour>\nWhere: -t sets text colour, and -b sets the background colour.");
            full_uart_puts("\nPossible colours (case-sensitive): black, red, green, yellow, blue, purple, cyan, white.");
            full_uart_puts("\nExample: setcolour -t white -b green");
        }
        //Actual command actions:
        else
        {
            if ((strUtil_CompareStrings(cmdParamBuffer[0],"-t")==1) && (strUtil_CompareStrings(cmdParamBuffer[2],"-b")==1))
            {

                //Text
               if (strUtil_CompareStrings(cmdParamBuffer[1],"black")==1)
               {
                cmdLine_setBLK_text;
               }

               if (strUtil_CompareStrings(cmdParamBuffer[1],"red")==1)
               {
                cmdLine_setRED_text;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[1],"green")==1)
               {
                cmdLine_setGRN_text;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[1],"yellow")==1)
               {
                cmdLine_setYLW_text;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[1],"blue")==1)
               {
                cmdLine_setBLU_text;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[1],"purple")==1)
               {
                cmdLine_setPRP_text;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[1],"cyan")==1)
               {
                cmdLine_setCYN_text;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[1],"white")==1)
               {
                cmdLine_setWHT_text;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[1],"black")==1)
               {
                cmdLine_setBLK_text;
               }
               else {full_uart_puts("\nIncorrect colour code. Type: help setcolour to see the list of available colours.");};


                //Background
               if (strUtil_CompareStrings(cmdParamBuffer[3],"red")==1)
               {
                cmdLine_setRED_bckgrnd;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[3],"green")==1)
               {
                cmdLine_setGRN_bckgrnd;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[3],"yellow")==1)
               {
                cmdLine_setYLW_bckgrnd;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[3],"blue")==1)
               {
                cmdLine_setBLU_bckgrnd;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[3],"purple")==1)
               {
                cmdLine_setPRP_bckgrnd;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[3],"cyan")==1)
               {
                cmdLine_setCYN_bckgrnd;
               }
               else if (strUtil_CompareStrings(cmdParamBuffer[3],"white")==1)
               {
                cmdLine_setWHT_bckgrnd;
               }
               else {full_uart_puts("\nIncorrect colour code. Type: help setcolour to see the list of available colours.");};
               
               //full_uart_puts("\nTerminal colours set.");

            }
            else {full_uart_puts("\nMissing or incorrect parameters. Usage: setcolour -t <colour> -b <colour>. Please re-check the syntax.");}

        }//Actual command actions END
}//Command definition end

void brdev() //DONE
{
    //help section
  if (strUtil_CompareStrings(cmdBuffer,"help")==1)
        {
            cmdLine_setRED_text;
            cmdLine_setGRN_bckgrnd;
            full_uart_puts("\n-------------------------------");
            full_uart_puts("\nSHOWING HELP FOR: brdev");
            full_uart_puts("\n-------------------------------");
            cmdLine_setBLK_bckgrnd;
            cmdLine_setWHT_text;            
            full_uart_puts("\nbrdev : displays RPY's hardware revision using the mailbox system.");
            full_uart_puts("\nUsage: \n    brdev");
            full_uart_puts("\nExample: brdev");
        }
        //Actual command actions:
        else
        {   
            {

                // mailbox data buffer: Read ARM frequency (arrays are ALWAYS pointers, wtf lol)
                mBuf[0] = 10*4;          // Message Buffer Size in bytes (8 elements * 8 bytes (32 bit) each) ( AS PER THE MANUAL "Request: length: 8")
                mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message) 
                mBuf[2] = 0x00010002;   // TAG Identifier: Get clock rate 
                mBuf[3] = 4;            // Value buffer size in bytes (max of request and response lengths) 
                mBuf[4] = 0;            // 0 = Request, anything else = Response
                mBuf[5] = 0;            // LED ID: 42 = status (yellow) LED, 130 = power (red) LED 
                mBuf[6] = 0;            // turn LED on/off command. 1 = on, 0 = off 
                mBuf[7] = 0;            // LED ID: 42 = status (yellow) LED, 130 = power (red) LED 
                mBuf[8] = 0;            // turn LED on/off command. 1 = on, 0 = off 
                mBuf[9] = MBOX_TAG_LAST;
                full_uart_sendc('\n');

                if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) //comma inside if: first statement evaluated, result discarded. Second statement is evaluated, result returned and affects outcome of IF
                { 
                                        
                    full_uart_puts("Board revision: "); 
                    full_uart_hex(mBuf[5]);
                }  
                
            }
            
        }  
}

void scrsize()
{
    //Help section
    if (strUtil_CompareStrings(cmdBuffer,"help")==1)
        {
            cmdLine_setRED_text;
            cmdLine_setGRN_bckgrnd;
            full_uart_puts("\n-------------------------------");
            full_uart_puts("\nSHOWING HELP FOR: scrsize");
            full_uart_puts("\n-------------------------------");
            cmdLine_setBLK_bckgrnd;
            cmdLine_setWHT_text;            
            full_uart_puts("\n scrsize : sets the physical and/or virtual screen size.\nUsage:\n    scrsize <screen> <width> <height> <init>\nWhere: <screen>: -p for physical, -v for virtual, -b for both.\n       <width> - width in pixels\n       <height> - height in pixels.\n       <init> - add -init to reinitialize the screen BEFORE setting width/heigth.\n");
            full_uart_puts("\n Example : scrsize -v 1024 768 -init\n");

        }
        //Command actions
        else
        {
            int paramWidth=0,paramHeigth=0;
            paramWidth = strUtils_StringToInt(cmdParamBuffer[1]);
            paramHeigth = strUtils_StringToInt(cmdParamBuffer[2]);

            if ((strUtil_CompareStrings(cmdParamBuffer[3],"-init")==1)) 
            {
                full_uart_puts("\nReceived -init signal. Re-initializing the framebuffer...");
                gfx_InitFrameBuffer();
                full_uart_puts("\nFramebuffer was reset.\n");  

            }
            else {full_uart_puts("\nFramebuffer will not be re-set.\n");};



            if ((strUtil_CompareStrings(cmdParamBuffer[0],"-v")==1)) 
            {

                full_uart_puts("\nWidth:\n");
                full_uart_dec(paramWidth);
                full_uart_puts("\nHeight:\n");
                full_uart_dec(paramHeigth);
                full_uart_puts("\n");
                //gfx_InitFrameBuffer();
                gfx_FrameBufferSetVirtualScreen(paramWidth, paramHeigth); 
                //gfx_FrameBufferSetPhysicalScreen(int w, int h);
 
            }
            else if ((strUtil_CompareStrings(cmdParamBuffer[0],"-p")==1)) 
            {


                //gfx_InitFrameBuffer();
                //gfx_FrameBufferSetVirtualScreen(paramWidth, paramHeigth); 
                gfx_FrameBufferSetPhysicalScreen( paramWidth, paramHeigth);
 
            }
            else if ((strUtil_CompareStrings(cmdParamBuffer[0],"-b")==1)) 
            {


                //gfx_InitFrameBuffer();
                gfx_FrameBufferSetVirtualScreen(paramWidth, paramHeigth); 
                gfx_FrameBufferSetPhysicalScreen(paramWidth, paramHeigth);
 
            }
            else {full_uart_puts("\nMissing or incorrect parameters. Usage: scrsize <screen> <width> <height> <init>. Please re-check the syntax.\n");};


            
            

        }    
} //scrsize end


void mboxtest()
{
    // mailbox data buffer: Read ARM frequency (arrays are ALWAYS pointers, wtf lol)
    mBuf[0] = 8*4;          // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each) 
    mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message) 
    mBuf[2] = 0x00030002;   // TAG Identifier: Get clock rate 
    mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths) 
    mBuf[4] = 0;            // REQUEST CODE = 0 
    mBuf[5] = 3;            // clock id: ARM system clock 
    mBuf[6] = 0;            // clear output buffer (response data are mBuf[5] & mBuf[6]) 
    mBuf[7] = MBOX_TAG_LAST;

    full_uart_sendc('\n');
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) //comma inside if: first statement evaluated, result discarded. Second statement is evaluated, result returned and affects outcome of IF
        { 
            full_uart_puts("Response Code for whole message: "); 
            full_uart_hex(mBuf[1]); 
            full_uart_puts("\n"); 
            
            full_uart_puts("Response Code in Message TAG: ");
            full_uart_hex(mBuf[4]); 
            full_uart_puts("\n"); 
            full_uart_puts("Responce received. Mailbox code working properly.");
        }
}

void cpuinfo() //DONE
{

    if (strUtil_CompareStrings(cmdBuffer,"help")==1)
        {
            cmdLine_setRED_text;
            cmdLine_setGRN_bckgrnd;
            full_uart_puts("\n-------------------------------");
            full_uart_puts("\nSHOWING HELP FOR: cpuinfo");
            full_uart_puts("\n-------------------------------");
            cmdLine_setBLK_bckgrnd;
            cmdLine_setWHT_text;            
            full_uart_puts("\ncpuinfo : Displays main CPU clock frequency.\nUsage: cpuinfo");

        }
        else
        {
        // mailbox data buffer: Read ARM frequency (arrays are ALWAYS pointers, wtf lol)
            mBuf[0] = 8*4;          // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each) 
            mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message) 
            mBuf[2] = 0x00030002;   // TAG Identifier: Get clock rate 
            mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths) 
            mBuf[4] = 0;            // REQUEST CODE = 0 
            mBuf[5] = 3;            // clock id: ARM system clock 
            mBuf[6] = 0;            // clear output buffer (response data are mBuf[5] & mBuf[6]) 

            mBuf[7] = MBOX_TAG_LAST;
            full_uart_sendc('\n');
            if (mbox_call(ADDR(mBuf),MBOX_CH_PROP)) //comma inside if: first statement evaluated, result discarded. Second statement is evaluated, result returned and affects outcome of IF
                { 
                    full_uart_puts("\nRequesting clock reading from GPU...");
                    full_uart_puts("\nARM mainclock rate = ");
                    int a = (mBuf[6])/1000000;
                    full_uart_dec(a); 
                    full_uart_puts("MHz\n");  
                }
        }
}

void raminfo()
{
    // mailbox data buffer: Read ARM frequency (arrays are ALWAYS pointers, wtf lol)
    mBuf[0] = 8*4;          // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each) 
    mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message) 
    mBuf[2] = 0x00010006;   // TAG Identifier: Get RAM 
    mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths) 
    mBuf[4] = 0;            // 0 = request. Response:
    mBuf[5] = 0;            // clock id: ARM system clock 
    mBuf[6] = 0;            // clear output buffer (response data are mBuf[5] & mBuf[6]) 
    mBuf[7] = MBOX_TAG_LAST;
    full_uart_sendc('\n'); 
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) //comma inside if: first statement evaluated, result discarded. Second statement is evaluated, result returned and affects outcome of IF
        { 
            full_uart_puts("DATA: RAM [Address, size] = ");
            full_uart_hex(mBuf[4]); 
            full_uart_puts("\n");
            full_uart_dec(mBuf[5]); 
            full_uart_puts("\n");
            full_uart_dec(mBuf[6]); 
            full_uart_puts("\n");  
        }
}

void statusled() //DONE
{

        if (strUtil_CompareStrings(cmdBuffer,"help")==1)
        {
            cmdLine_setRED_text;
            cmdLine_setGRN_bckgrnd;
            full_uart_puts("\n-------------------------------");
            full_uart_puts("\nSHOWING HELP FOR: statusled");
            full_uart_puts("\n-------------------------------");
            cmdLine_setBLK_bckgrnd;
            cmdLine_setWHT_text;            
            full_uart_puts("\nstatusled : Enables manual control of the RPY's yellow status LED using the mailbox system.");
            full_uart_puts("\nUsage: \n    statusled <on/off> \nWhere: <on/off> is an integer 1 or 0 corresponding to ON or OFF state of the LED respectively.");
            full_uart_puts("\nExample: statusled on");
        }
        //Actual command actions:
        else
        {   
            //parameter "on" is parsed
            if ((strUtil_CompareStrings(cmdParamBuffer[0],"on")==1)) 
            {

                // mailbox data buffer: Read ARM frequency (arrays are ALWAYS pointers, wtf lol)
                mBuf[0] = 8*8;          // Message Buffer Size in bytes (8 elements * 8 bytes (32 bit) each) ( AS PER THE MANUAL "Request: length: 8")
                mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message) 
                mBuf[2] = 0x00038041;   // TAG Identifier: LED TOGGLE
                mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths) 
                mBuf[4] = 0;            // 0 = request. Response:
                mBuf[5] = 42;            // LED ID: 42 = status (yellow) LED, 130 = power (red) LED 
                mBuf[6] = 1;            // turn LED on/off command. 1 = on, 0 = off 
                mBuf[7] = MBOX_TAG_LAST;
                full_uart_sendc('\n');

                if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) //comma inside if: first statement evaluated, result discarded. Second statement is evaluated, result returned and affects outcome of IF
                { 
                    full_uart_puts("DATA: response code = ");
                    full_uart_hex(mBuf[4]); 
                    full_uart_puts("\n");
                    full_uart_puts("DATA: led status = ");
                    full_uart_dec(mBuf[6]); 
                    full_uart_puts("\n"); 
                    full_uart_puts("YELLOW LED on."); 
                }  
                
            }
            //parameter "off" is parsed
            else if ((strUtil_CompareStrings(cmdParamBuffer[0],"off")==1)) 
            {

                // mailbox data buffer: Read ARM frequency (arrays are ALWAYS pointers, wtf lol)
                mBuf[0] = 8*8;          // Message Buffer Size in bytes (8 elements * 8 bytes (32 bit) each) ( AS PER THE MANUAL "Request: length: 8")
                mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message) 
                mBuf[2] = 0x00038041;   // TAG Identifier: LED TOGGLE
                mBuf[3] = 8;            // Value buffer size in bytes (max of request and response lengths) 
                mBuf[4] = 0;            // 0 = request. Response:
                mBuf[5] = 42;            // LED ID: 42 = status (yellow) LED, 130 = power (red) LED 
                mBuf[6] = 0;            // turn LED on/off command. 1 = on, 0 = off 
                mBuf[7] = MBOX_TAG_LAST;
                full_uart_sendc('\n'); 

                if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) //comma inside if: first statement evaluated, result discarded. Second statement is evaluated, result returned and affects outcome of IF
                { 
                    full_uart_puts("DATA: response code = ");
                    full_uart_hex(mBuf[4]); 
                    full_uart_puts("\n");
                    full_uart_puts("DATA: led status = ");
                    full_uart_dec(mBuf[6]); 
                    full_uart_puts("\n");  
                    full_uart_puts("YELLOW LED off.");
                }  

            } //Wrong parameters given
            else {full_uart_puts("\nMissing or incorrect parameters. Usage: statusled on. Please re-check the syntax.");}
        }

    
}


void hdmitest() //DONE
{
        if (strUtil_CompareStrings(cmdBuffer,"help")==1)
        {
            cmdLine_setRED_text;
            cmdLine_setGRN_bckgrnd;
            full_uart_puts("\n-------------------------------");
            full_uart_puts("\nSHOWING HELP FOR: hdmitest");
            full_uart_puts("\n-------------------------------");
            cmdLine_setBLK_bckgrnd;
            cmdLine_setWHT_text;            
            full_uart_puts("\nhdmitest : Display coloured rectangles on the connected HDMI screen.");

        }
        //Actual command actions:
        else
        { 
            full_uart_puts("\n");
             gfx_InitFrameBuffer(); 
             gfx_FrameBufferSetVirtualScreen(1920, 1080); 
             gfx_FrameBufferSetPhysicalScreen(1920, 1080);
            gfx_FrameBufferRunColourTest();
            full_uart_puts("\nConnected HDMI should display colours.");
        }



}


void gametest() //DONE
{
        if (strUtil_CompareStrings(cmdBuffer,"help")==1)
        {
            cmdLine_setRED_text;
            cmdLine_setGRN_bckgrnd;
            full_uart_puts("\n-------------------------------");
            full_uart_puts("\nSHOWING HELP FOR: gametest");
            full_uart_puts("\n-------------------------------");
            cmdLine_setBLK_bckgrnd;
            cmdLine_setWHT_text;            
            full_uart_puts("\ngametest : Run a test game build. Ver 0.02R.");

        }
        //Actual command actions:
        else
        { 
            full_uart_puts("\n");
            full_uart_puts("\n Initializing dual framebuffer...\n");
            gfx_InitDoubleBuffer(); //Double buffering 

             full_uart_puts("\n Setting display resolution to 768p...\n");
             gfx_FrameBufferSetVirtualScreen(1024, 768); 
             gfx_FrameBufferSetPhysicalScreen(1024, 768);

            int x = 240;

  
            
            full_uart_puts("\n Executing the game loop... \n");
            full_uart_puts("Game is running. Input log string: \n");
            
            char kb_press;

            for (int i = 0; i < 10; i++) {
                    
                        inGameObjects[i].exists=0;
                        inGameObjects[i].name="EMPTY";

                        inGameObjects[i].x=0;
                        inGameObjects[i].y=0;
                        inGameObjects[i].velocityX=0;
                        inGameObjects[i].velocityY=0;


                        inGameObjects[i].sprite=0;
                        inGameObjects[i].sprite_hSize = 0;
                        inGameObjects[i].sprite_vSize = 0;
                        inGameObjects[i].drawLayer=0;

                        inGameObjects[i].cBox_x1 = 0;
                        inGameObjects[i].cBox_y1 = 0;
                        inGameObjects[i].cBox_x2 = 0;
                        inGameObjects[i].cBox_y2 = 0;
                    

            };
            
            drawSpriteARGB32(back1,1024,768,512,384);
            drawSpriteARGB32(back1,1024,768,1536,384);
            drawSpriteARGB32(spriteBall,100,100,1535,384);
            while(1)
            {
                
                
                kb_press = getFullUart();
                
                if (kb_press=='e') //Create test obj
                {
                    gfx_setOffset(0, 0);
                    kb_press = 0;
                    
                };





                if (kb_press=='d')
                {
                    gme_createObject("testBall",500,300,-1,-1,spriteBall,100,100,0,0,0,0,0);
                    kb_press = 0;
                    
                };


                if (kb_press=='f')
                {
                    gfx_setOffset(1024, 0);
                    kb_press = 0;
                    
                };

                kb_press = 0;


                //gfx_flushScreen(1);
                //gfx_InitFrameBuffer();

                gme_eraseObjects(back1);
                
               
                gme_motionUpdate();
                

                
                
                gme_renderObjects();

                 //Show the screen to the viewer for 60 ms
                
                
                //full_uart_puts("\n Screen re-drawn ");

                /*
                full_uart_puts("object 0: ");
                full_uart_dec(inGameObjects[0].exists);
                full_uart_sendc('\n');
                full_uart_puts("object 1: ");
                full_uart_dec(inGameObjects[1].exists);
                full_uart_sendc('\n');
                full_uart_puts("object 2: ");
                full_uart_dec(inGameObjects[2].exists);
                full_uart_sendc('\n');
                */


                
            }
  
            
        }

}
