/* Command line-related function prototypes */ 
void cmdLine_InitInputBuffer();
void cmdLine_InitCommandBuffer(); 
void cmdLine_SetDelCharacter(int asciiDecCode);
void cmdLine_LoadCommandList() ;
int cmdLine_GetInputBufferLength();
int cmdLine_GetCommandBufferLength();
void cmdLine_ParseParameters(char *source);
void cmdLine_InitParamBuffer();
void cmdLine_RunCommand(char *source);




//String utilities-related function prototypes
int strUtil_GetStringLength(char *string, int countSpacesYN);
int strUtil_CompareStrings(char *string1, char *string2);
void strUtil_EraseString(char* string);
void strUtil_CopyString(char* source, char* dest, char stopChar, int accountForLengthDifferenceYN, int overwriteDestYN, int eraseSourceYN);
int strUtils_IsStringPresentInArray(char* string, char* array[],int arraySize);
void strUtil_CopyStringToArray(char* source, char* dest[], char stopChar, int accountForLengthDifferenceYN, int wipeDestYN, int wipeSourceYN)  ;
int strUtils_StringToInt(char* string);

extern char inputBuffer[256];                          //Stores anything that was typed
extern char cmdBuffer[256];

extern int cmdParamBufferMaxParams;                            //maximum parameters allowed for ALL commands
extern char cmdParamBuffer[6][20];

extern int cmdListSize;                            //Total amount of commands "installed"
extern char *cmdList[12];
extern void (*fun_ptr[12])(int,int,int,int,int); 

extern char backspaceChar;
extern int inputBufferLen;


