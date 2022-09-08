
//Command line buffer and parsing variables

char inputBuffer[256];                          //Stores anything that was typed
char cmdBuffer[256];

int cmdParamBufferMaxParams = 6;                            //maximum parameters allowed for ALL commands
char cmdParamBuffer[6][20];

int cmdListSize = 12;                            //Total amount of commands "installed"
char *cmdList[12];
void (*fun_ptr[12])(int,int,int,int,int); 

char backspaceChar = 127;
int inputBufferLen = 0;


