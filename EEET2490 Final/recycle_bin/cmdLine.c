
void cmdLine_InitCommandBuffer()
    {
        
        for (int i = 0; i < 256; i++) //Flush the command buffer
                {
                    *(&cmdBuffer[0]+i)  = '\0';
                }   

        cmdBufferLen = 0; //INDEX of las nonzero, non-newline element in an array. INDEX!!!! Not a length!
        

    }
    
    
void cmdLine_SetDelCharacter(int asciiDecCode)
    {
        backspaceChar = asciiDecCode;  //ASCII DEL

    }

void cmdLine_FlushCommandBuffer()
    {
        for (int i = 0; i < 256; i++) //Flush the command buffer
                {
                    *(&cmdBuffer[0]+i)  = '\0';
                } 


    }