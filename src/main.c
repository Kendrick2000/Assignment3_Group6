#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "printf.h"
#include "image.h"
#include "video.h"
#include "largeImage.h"
#include "game.h"
#include "timer.h"

struct Sprite {
    int x;
    int y;
    int direction;
};


void main()
{
    int count = 0;
    int score = 0;
    struct Sprite tiles[40];

    uart_init();
    framebf_init();

    uart_puts("\nEEET2490 - Embedded System: Operating System and Interfacing \n\n");
    uart_puts("######## ######## ######## ########   #######  ##         #######    #####   \n"
              "##       ##       ##          ##     ##     ## ##    ##  ##     ##  ##   ##  \n"
              "##       ##       ##          ##            ## ##    ##  ##     ## ##     ## \n"
              "######   ######   ######      ##      #######  ##    ##   ######## ##     ## \n"
              "##       ##       ##          ##     ##        #########        ## ##     ## \n"
              "##       ##       ##          ##     ##              ##  ##     ##  ##   ##  \n"
              "##       ##       ##          ##     ##              ##  ##     ##  ##   ##  \n"
              "######## ######## ########    ##     #########       ##   #######    #####   \n"
              "\n\n"
              "########     ###    ########  ########     #######   ######                 \n"
              "##     ##   ## ##   ##     ## ##          ##     ## ##    ##                \n"
              "##     ##  ##   ##  ##     ## ##          ##     ## ##                      \n"
              "########  ##     ## ########  ######      ##     ##  ######                 \n"
              "##     ## ######### ##   ##   ##          ##     ##       ##                \n"
              "##     ## ##     ## ##    ##  ##          ##     ## ##    ##                \n"
              "########  ##     ## ##     ## ########     #######   ######                 \n"
              "\n\n");

    drawStringLarge(50, 20, "EEET2490", 0x003498DB);
    drawStringLarge(50, 100, "Bare OS", 0x003498DB);

    drawString32x32(50, 200, "Uyen Nguyen - s3751882", 0x003498DB);
    drawString32x32(50, 220, "Khang Truong - s3751882", 0x00AF601A);
    drawString32x32(50, 240, "Daniil Shlee - s3751882", 0x00BB8FCE);
    drawString32x32(50, 260, "Kiet Park - s3751882", 0x00E74C3C);

    wait_msec(3000000);

    // Delete font letters by writing them again in black
    drawStringLarge(50, 20, "EEET2490", 0x0);
    drawStringLarge(50, 100, "Bare OS", 0x0);

    drawString32x32(50, 200, "Uyen Nguyen - s3751882", 0x0);
    drawString32x32(50, 220, "Khang Truong - s3751882", 0x0);
    drawString32x32(50, 240, "Daniil Shlee - s3751882", 0x0);
    drawString32x32(50, 260, "Kiet Park - s3751882", 0x0);

    framebf_release();
    framebf_init();

    // Print cursor onto terminal screen
    uart_puts("\n");
    uart_puts("MyBareOS> ");

    void draw_image()
    {
        // framebf_init(physicalWidth, physicalHeight, virtualWidth, virtualHeight);
        for (int j = 0; j < 177; j++)
        {
            for (int i = 0; i < 284; i++)
            {
                drawPixelARGB32(i, j, image[j * 284 + i]);
            }
        }
    }

    

    int collisionWithPaddle(int ballX, int ballY, int paddleX, int direction)
    {
        int dir = 0;
        dir = direction;
        if ((ballY + 50 == 700) && ((ballX >= paddleX) && (ballX <= paddleX + 127)))
        {
            // uart_puts("Hello from collision");
            // uart_puts("\n");
            if(((ballX >= paddleX) && (ballX <= paddleX + 30))){
                dir = 2;
            }else if(((ballX <= paddleX + 127) && (ballX >= paddleX + 97))){
                dir = 3;
            }
        }
        return dir;
    }

    void deleteTileCoordinate(int position){
        int tempX[40], tempY[40];
        int count = 0;
        for (int i = 0; i < 40; i++){
            if(i == position){
                i++;
            }else{
                tempX[count] = tiles[i].x;
                tempY[count] = tiles[i].y;
                tiles[count].x = tempX[count];
                tiles[count].y = tempY[count];
                count++;
            }
        }
    }

    int detectCollision(int x, int y, int direction){
        int count = 0;
        int distanceX = 0, distanceY = 0;
        int isCollision = 0;
        int dir = 0;
        dir = direction;
        for(int i = 0; i < 40; i++){
            if ((y >= tiles[i].y) && (y <= (tiles[i].y + 32))){
               if(((x >= tiles[i].x) && (x <= tiles[i].x + 150)) || (((x + 50) >= tiles[i].x) && ((x + 50) <= tiles[i].x))){
                   del_tile(tiles[i].x, tiles[i].y);
                   deleteTileCoordinate(i);
                   dir = 1;
               }
           }
           else if (((y + 50) >= tiles[i].y) && ((y + 50) <= (tiles[i].y + 32)))
           {
               if(((x >= tiles[i].x) && (x <= tiles[i].x + 150)) || (((x + 50) >= tiles[i].x) && ((x + 50) <= tiles[i].x)))
               {
                   del_tile(tiles[i].x, tiles[i].y);
                   deleteTileCoordinate(i);
                   dir = 3;
               }
           }
           else if ((x == tiles[i].x) || (x + 50 == tiles[i].x) || (x == tiles[i].x + 150) || (x + 50 == tiles[i].x + 150))
           {
               if ((y >= tiles[i].y) && (y <= tiles[i].y + 32))
               {
                   del_tile(tiles[i].x, tiles[i].y);
                   deleteTileCoordinate(i);
                   dir = 4;
               }

               if (((y + 50) >= tiles[i].y) && ((y + 50) <= tiles[i].y + 32))
               {
                   del_tile(tiles[i].x, tiles[i].y);
                   deleteTileCoordinate(i);
                   dir = 4;
               }
           }
           else if (((x >= tiles[i].x) && (x <= tiles[i].x + 150)) || (((x + 50) >= tiles[i].x) && ((x + 50) <= tiles[i].x)))
           {
                if (((y + 50) >= tiles[i].y) && ((y + 50) <= (tiles[i].y + 32)))
                {
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    dir = 3;
                }   
           }
           else if (((x >= tiles[i].x) && (x <= tiles[i].x + 150)) || (((x + 50) >= tiles[i].x) && ((x + 50) <= tiles[i].x)))
           {
                if ((y >= tiles[i].y) && (y <= (tiles[i].y + 32)))
                {
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    dir = 1;
                } 
           }
        }
        return dir;
    }

    void draw_game()
    {
        int barX = 500, barFlag = 0, count = 0;
        int ballX = 500, ballY = 650;
        int traceX = 0, traceY = 0;
        int direction = 0;
        int i = 0; 
        int isCollision = 0, isInitial = 0;
        char str[10000];
        draw_background();
        draw_paddle(barX, 700);

        // draw_pixelBall(traceX, traceY);
        while (1)
        {
            drawString32x32(800,10,"Score: ",0x00E74C3C);
            drawString32x32(930,10,score + "0",0x00E74C3C);
            // framebf_init(gamePhysicalWidth, gamePhysicalHeight, gameVirtualWidth, gameVirtualHeight);
            if(isInitial == 0){
                for (int x = 90; x < 900; x += 170)
                {
                    for (int y = 50; y < 280; y += 32)
                    {
                        if (y == 50)
                        {
                            draw_yellowTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 82)
                        {
                            draw_redTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 114)
                        {
                            draw_blueTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 146)
                        {
                            draw_greenTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 178)
                        {
                            draw_yellowTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 210)
                        {
                            draw_blueTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 242)
                        {
                            draw_greenTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 274)
                        {
                            draw_redTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                    }
                }
                isInitial = 1;
            }
            // draw_pixelBall(500, 650);
            // draw_paddle(450, 700);

            if ((ballX <= 955 && ballX >= 0) && (ballY <= 786 && ballY >= 0))
            {
                traceX = ballX + 51;
                traceY = ballY + 51;
                draw_pixelBall(ballX, ballY);
                direction = collisionWithPaddle(ballX, ballY, barX, direction);
                // printf(" %d %d d%d ", ballX, ballY,direction);
                // printf("\n");
                if (((ballY == 0) && (955 - ballX <= 477)) || (direction == 4))
                {
                    direction = 4;
                }
                else if (((ballY == 0) && (955 - ballX > 477)) || (direction == 1))
                {
                    direction = 1;
                }
                else if (((ballX == 0) && (736 - ballY > 368)) || (direction == 3))
                {
                    direction = 3;
                }
                else if (((ballX == 0) && (736 - ballY <= 368)) || (direction == 1))
                {
                    direction = 1;
                }
                else if (((ballX == 955) && (736 - ballY > 368)) || (direction == 2))
                {
                    direction = 2;
                }
                else if (((ballX == 955) && (736 - ballY <= 368)) || (direction == 4))
                {
                    direction = 4;
                }

                if (direction == 1)
                {
                    ballX++;
                    ballY++;
                    if (ballX == 955)
                    {
                        direction = 0;
                    }
                }
                else if (direction == 2)
                {
                    ballX--;
                    ballY--;
                    if (ballX == 0 || ballY == 0)
                    {
                        direction = 0;
                    }
                }
                else if (direction == 3)
                {
                    ballX++;
                    ballY--;
                    if (ballX == 955 || ballY == 0)
                    {
                        direction = 0;
                    }
                }
                else if (direction == 4)
                {
                    ballX--;
                    ballY++;
                    if (ballX == 0)
                    {
                        direction = 0;
                    }
                }
                else
                {
                    // ballX-=5;
                    ballY--;
                }
                direction = detectCollision(ballX, ballY, direction);
                // isCollision = detectCollision(ballX, ballY);                
                wait_msec(4000); 
            }
            else
            {
                ballX = 500;
                ballY = 650;
                direction = 0;
            }

            str[count] = getUart();
            if (str[count] != '\0'){
                if (str[count] == 'd'){
                    if( barX <= 900){
                        move_paddle(str, barX);
                        barX+=100;
                        draw_paddle(barX,700);
                        
                    }
                }

                if (str[count] == 'a'){
                    if(barX >= 0){
                        move_paddle(str, barX);
                        barX-=100;
                        draw_paddle(barX,700);
                    }
                }
            }

            if (str[count] == 'c')
            {
                count++;
                break;
            }
        }
    }

    void draw_video()
    {
        int countFrame = 0;
        // framebf_init(phWidth, phHeight, virWidth, virHeight);
        while (countFrame < 5)
        {
            for (int j = 0; j < 240; j++)
            {
                for (int i = 0; i < 320; i++)
                {
                    if (countFrame == 0)
                    {
                        drawPixelARGB32(i, j, video[j * 320 + i]);
                    }
                    else if (countFrame == 1)
                    {
                        drawPixelARGB32(i, j, video1[j * 320 + i]);
                    }
                    else if (countFrame == 2)
                    {
                        drawPixelARGB32(i, j, video2[j * 320 + i]);
                    }
                    else if (countFrame == 3)
                    {
                        drawPixelARGB32(i, j, video3[j * 320 + i]);
                    }
                    else if (countFrame == 4)
                    {
                        drawPixelARGB32(i, j, video4[j * 320 + i]);
                    }
                    else
                    {
                        uart_puts("No more frame");
                    }
                }
            }
            countFrame++;
        }
    }

    void drawLargeImage()
    {
        char str[10];
        int count = 0, countY = 0, flag = 0;
        // framebf_init(pWidth, pHeight, vWidth, vHeight);
        for (int j = 0; j < 1206; j++)
        {
            for (int i = 0; i < 1000; i++)
            {
                drawPixelARGB32(i, j, largeImage[j * 1000 + i]);
            }
        }

        while (1)
        {
            str[count] = uart_getc();

            if (str[count] == 's')
            {
                if (flag <= 500)
                {
                    countY = flag + 100;
                    flag = countY;
                    framebf_init(pWidth, pHeight, vWidth, vHeight);
                    for (int j = 0; j < 1206; j++)
                    {
                        countY++;
                        for (int i = 0; i < 1000; i++)
                        {
                            drawPixelARGB32(i, j, largeImage[countY * 1000 + i]);
                        }
                    }
                }
            }

            if (str[count] == 'w')
            {
                if (flag > 0)
                {
                    countY = flag - 100;
                    flag = countY;
                    framebf_init(pWidth, pHeight, vWidth, vHeight);
                    for (int j = 0; j < 1206; j++)
                    {
                        countY++;
                        for (int i = 0; i < 1000; i++)
                        {
                            drawPixelARGB32(i, j, largeImage[countY * 1000 + i]);
                        }
                    }
                }
            }
            count++;
        }
    }

    // Comparing string function
    int strCompare(char *arrayA, char *arrayB)
    {
        int count = 0, anw = 0;
        // Looping all character in arrayB
        while (arrayB[count] != '\0')
        {
            // Comparing character by character between arrayA and arrayB.
            if (arrayA[count] == arrayB[count])
            {
                // Set value anw = 0 as true.
                anw = 0;
                // continue checking until while loop condition reached.
                count++;
            }
            else
            {
                // Set value anw = 1 as false.
                anw = 1;
                // stop loop
                break;
            }
        }
        // return result/
        return anw;
    }

    // Function display all information of all command in CLI.
    void helpCommand(char *array)
    {
        // Condition to distinguish between help and help <command>.
        if (array[4] == '\n')
        {
            // print out all command exist in the Bare OS.
            uart_puts("\nFor more information on a specific command, type help command-name\n");
            uart_puts("help <command_name>      Show full information of the command.\n");
            uart_puts("help                     Show brief information of all commands.\n");
            uart_puts("setcolor                 Set  text  color,  and/or  background  color  of  the console  to  one  of  the  following  color:  BLACK,  RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE.\n");
            uart_puts("brdev                    Show board revision.\n");
            uart_puts("scrsize                  Set screen size. Must have options to set physical screen size (-p) or virtual screen size (-v), or both (by default).\n");
            uart_puts("clk                      Show clock rate.\n");
            uart_puts("ARM                      Show ARM Memory.\n");
            uart_puts("draw                     Draw 6 rectangle with 6 different color(RED, GREEN, BLUE, PRUPLE, ORANGE).\n");
            uart_puts("MAC                      Show MAC address.\n");
            uart_puts("clear                    Clear terminal console screen.\n");
        }
        else
        {
            int count = 4, i = 0;
            char command[50];
            // looping through each character in array.
            while (array[count] != '\n')
            {
                // uart_sendc(array[count]);
                // Get specific command that users want to learn more informations.
                command[i] = array[count];
                // uart_sendc(command[i]);
                i++;
                count++;
            }

            // Find correct information to a corresponding to command.
            if (strCompare(command, "setcolor") == 0)
            {
                uart_puts("Set text color only:                         setcolor -t <color>.\n");
                uart_puts("Set background color only:                   setcolor -b <color>.\n");
                uart_puts("Set color for both background and text:      setcolor -t <color> -b <color or setcolor -b <color> -t<color>.\n");
                uart_puts("Accepted color and writing format:  Black,  Red, Green, Yellow, Blue, Purple, Cyan, White.\n");
            }
            else if (strCompare(command, "brdev") == 0)
            {
                uart_puts("Show board revision:                         brdev\n");
            }
            else if (strCompare(command, "scrsize") == 0)
            {
                uart_puts("Set screen size for virtual screen:          scrsize -v <width> <height>.\n");
                uart_puts("Set screen size for physical screen:         scrsize -p <width> <height>.\n");
                uart_puts("Set screen size for both screens:            scrsize -v <width> <height> -p <width> <height> or scrsize -p <width> <height> -v <width> <height\n");
            }
            else if (strCompare(command, "clk") == 0)
            {
                uart_puts("Show clock rate:                             clk\n");
            }
            else if (strCompare(command, "ARM") == 0)
            {
                uart_puts("Show ARM memory:                             ARM\n");
            }
            else if (strCompare(command, "draw") == 0)
            {
                uart_puts("Show drawed image:                           draw.\n");
                uart_puts("Draw 6 rectangles with 6 different colors(RED, GREEN, BLUE, PRUPLE, ORANGE).\n");
            }
            else if (strCompare(command, "MAC") == 0)
            {
                uart_puts("Show MAC address:                            MAC\n");
            }
            else
            {
                uart_puts("Commnad name is not valid or not avaliable.\n");
            }
        }
    }

    // Clean CLI screen function.
    void clear()
    {
        uart_puts("\e[1;1H\e[2J");
    }

    // Show clock rate of the board.
    void showClockRate()
    {
        mbox[0] = 8 * 4;        // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
        mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
        mbox[2] = 0x00030002;   // TAG Identifier: Get clock rate
        mbox[3] = 8;            // Value buffer size in bytes (max of request and response lengths)
        mbox[4] = 0;            // REQUEST CODE = 0
        mbox[5] = 3;            // clock id: ARM system clock
        mbox[6] = 0;            // clear output buffer (response data are mbox[5] & mbox[6])
        mbox[7] = MBOX_TAG_LAST;

        if (mbox_call(ADDR(mbox), MBOX_CH_PROP))
        {
            uart_puts("DATA: ARM clock rate = ");
            uart_dec(mbox[6]);
            uart_puts("\n");
        }
    }

    // Show Board Revision.
    void showBoardRevision()
    {
        mbox[0] = 8 * 4;        // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
        mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
        mbox[2] = 0x00010002;   // TAG Identifier: Get board revision
        mbox[3] = 32;           // Value buffer size in bytes (max of request and response lengths)
        mbox[4] = 0;            // REQUEST CODE = 0
        mbox[5] = 4;            // RESPOND LENGTH = 4
        mbox[6] = 0;            // clear output buffer (response data are mbox[5] & mbox[6])
        mbox[7] = MBOX_TAG_LAST;

        if (mbox_call(ADDR(mbox), MBOX_CH_PROP))
        {
            uart_puts("DATA: Board Revision = ");
            uart_dec(mbox[6]);
            uart_puts("\n");
        }
    }

    // Show Board Model.
    void showARMMemory()
    {
        mbox[0] = 8 * 4;        // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
        mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
        mbox[2] = 0x00010005;   // TAG Identifier: Get ARM Memory.
        mbox[3] = 32;           // Value buffer size in bytes (max of request and response lengths)
        mbox[4] = 0;            // REQUEST CODE = 0
        mbox[5] = 8;            // RESPOND LENGTH = 4
        mbox[6] = 0;            // clear output buffer (response data are mbox[5] & mbox[6])
        mbox[7] = MBOX_TAG_LAST;

        if (mbox_call(ADDR(mbox), MBOX_CH_PROP))
        {
            uart_puts("DATA: ARM Memory = ");
            uart_dec(mbox[6]);
            uart_puts("\n");
        }
    }

    // Show MAC Address.
    void showMACAddress()
    {
        mbox[0] = 8 * 4;        // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
        mbox[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
        mbox[2] = 0x00010003;   // TAG Identifier: Get MAC Address.
        mbox[3] = 8;            // Value buffer size in bytes (max of request and response lengths)
        mbox[4] = 0;            // REQUEST CODE = 0
        mbox[5] = 6;            // RESPOND LENGTH = 6
        mbox[6] = 0;            // clear output buffer (response data are mbox[5] & mbox[6])
        mbox[7] = MBOX_TAG_LAST;

        if (mbox_call(ADDR(mbox), MBOX_CH_PROP))
        {
            uart_puts("DATA: MAC Address = ");
            uart_dec(mbox[6]);
            uart_puts("\n");
        }
    }

   

    // Draw rectangle on the screen
    void drawRectangle()
    {
        drawRectARGB32(100, 100, 400, 400, 0x00AA0000, 1); // RED
        drawRectARGB32(150, 150, 400, 400, 0x0000BB00, 1); // GREEN
        drawRectARGB32(200, 200, 400, 400, 0x000000CC, 1); // BLUE
        drawRectARGB32(250, 250, 400, 400, 0x00FFFF00, 1); // YELLOW
        drawRectARGB32(300, 300, 400, 400, 0x007F3FBF, 1); // PURPLE
        drawRectARGB32(350, 350, 400, 400, 0x00F09D0E, 1); // ORANGE
    }

    // Set colors for texts and background.
    void setColor(char *array)
    {
        int count = 0, i = 0, j = 0, check = 1;
        char text[50], bGround[50];
        // looping through each character in array.
        while (array[count] != '\0')
        {
            // uart_sendc(array[count]);
            // Conditions to check command set color for texts.
            if ((array[count] == '-' && array[count + 1] == 't') && (array[count + 2] != '\0'))
            {
                // Variable to check is the command syntax correct or not.
                check = 0;
                // looping through the rest of the array or until hit next set color command for background.
                while ((array[count] != '\0') && (array[count + 1] != '-'))
                {
                    // Get specific color from users command.
                    text[i] = array[count + 2];
                    // printf("%d ", count);
                    // uart_sendc(text[i]);
                    i++;
                    count++;
                }
                // uart_puts("\n");
                // Find correct color that users want to set for texts.
                if (strCompare(text, "Black") == 0)
                {
                    uart_puts("\033[30m");
                }
                else if (strCompare(text, "Red") == 0)
                {
                    uart_puts("\033[31m");
                }
                else if (strCompare(text, "Green") == 0)
                {
                    uart_puts("\033[32m");
                }
                else if (strCompare(text, "Yellow") == 0)
                {
                    uart_puts("\033[33m");
                }
                else if (strCompare(text, "Blue") == 0)
                {
                    uart_puts("\033[34m");
                }
                else if (strCompare(text, "Purple") == 0)
                {
                    uart_puts("\033[35m");
                }
                else if (strCompare(text, "Cyan") == 0)
                {
                    uart_puts("\033[36m");
                }
                else if (strCompare(text, "White") == 0)
                {
                    uart_puts("\033[37m");
                }
                else
                {
                    uart_puts("Color is not valid.\n");
                    break;
                }
            }

            // Conditions to check command set color for background.
            if ((array[count] == '-' && array[count + 1] == 'b') && (array[count + 2] != '\0'))
            {
                // Variable to check is the command syntax correct or not.
                check = 0;
                while ((array[count] != '\0') && (array[count + 1] != '-'))
                {
                    // Get specific color from users command.
                    bGround[j] = array[count + 2];
                    // uart_sendc(bGround[j]);
                    j++;
                    count++;
                }
                // uart_puts("\n");
                // Find correct color that users want to set for background.
                if (strCompare(bGround, "Black") == 0)
                {
                    uart_puts("\033[40m");
                }
                else if (strCompare(bGround, "Red") == 0)
                {
                    uart_puts("\033[41m");
                }
                else if (strCompare(bGround, "Green") == 0)
                {
                    uart_puts("\033[42m");
                }
                else if (strCompare(bGround, "Yellow") == 0)
                {
                    uart_puts("\033[43m");
                }
                else if (strCompare(bGround, "Blue") == 0)
                {
                    uart_puts("\033[44m");
                }
                else if (strCompare(bGround, "Purple") == 0)
                {
                    uart_puts("\033[45m");
                }
                else if (strCompare(bGround, "Cyan") == 0)
                {
                    uart_puts("\033[46m");
                }
                else if (strCompare(bGround, "White") == 0)
                {
                    uart_puts("\033[47m");
                }
                else
                {
                    uart_puts("Color is not valid.\n");
                    break;
                }
            }
            count++;
        }
        // Condition to display syntax error message to users.
        if (check == 1)
        {
            uart_puts("Syntax error.\n");
        }
    }

    // Direct Users to correct function corresponding to commands.
    int selectCommmand(char *array)
    {
        int checkScrSize = 0;
        // Condition to be taken to helpCommand function.
        if (strCompare(array, "help") == 0)
        {
            helpCommand(array);

            // Condition to be taken to clear function.
        }
        else if (strCompare(array, "clear") == 0)
        {
            // Extra condition to check syntax of users's input commands.
            if (array[5] == '\n')
            {
                clear();
            }
            else
            {
                uart_puts("Command line syntax error.\n");
            }

            // Condition to be taken to showBoardRevision function.
        }
        else if (strCompare(array, "brdev") == 0)
        {
            // Extra condition to check syntax of users's input commands.
            if (array[5] == '\n')
            {
                showBoardRevision();
            }
            else
            {
                uart_puts("Command line syntax error.\n");
            }

            // Condition to be taken to showClockRate function.
        }
        else if (strCompare(array, "clk") == 0)
        {
            // Extra condition to check syntax of users's input commands.
            if (array[3] == '\n')
            {
                showClockRate();
            }
            else
            {
                uart_puts("Command line syntax error.\n");
            }

            // Condition to be taken to showARMMemory function.
        }
        else if (strCompare(array, "ARM") == 0)
        {
            // Extra condition to check syntax of users's input commands.
            if (array[3] == '\n')
            {
                showARMMemory();
            }
            else
            {
                uart_puts("Command line syntax error.\n");
            }

            // Condition to be taken to showMACAddress function.
        }
        else if (strCompare(array, "MAC") == 0)
        {
            // Extra condition to check syntax of users's input commands.
            if (array[3] == '\n')
            {
                showMACAddress();
            }
            else
            {
                uart_puts("Command line syntax error.\n");
            }

            // Condition to be taken to drawRectangle function.
        }
        else if (strCompare(array, "draw") == 0)
        {
            // Extra condition to check syntax of users's input commands.
            if (array[4] == '\n')
            {
                drawRectangle();
            }
            else
            {
                uart_puts("Command line syntax error.\n");
            }

            // Condition to be taken to setColor function.
        }
        else if (strCompare(array, "setcolor") == 0)
        {
            setColor(array);

            // Condition to be taken to scrsize function.
        }
        else if (strCompare(array, "scrsize") == 0)
        {
            // return value checkScrSize when users call scrsize command.
            checkScrSize = 1;
            // setScreenSize(array);
        }
        else if (strCompare(array, "img") == 0)
        {
            draw_image();
        }
        else if (strCompare(array, "video") == 0)
        {
            while (1)
            {
                draw_video();
            }
        }
        else if (strCompare(array, "Image") == 0)
        {
            drawLargeImage();
        }
        else if (strCompare(array, "game") == 0)
        {
            draw_game();
            // draw_backGround();
            // draw_pixelBall();
            // draw_blueTile();
            // Note: greenTile turn to blue
            // draw_greenTile();
            // draw_yellowTile();
            // draw_redTile();
        }
        else
        {
            // Diplay error message to users.
            uart_puts("Can't understand the command.\n");
        }
        return checkScrSize;
    }

    // Clear white space within each command from users.
    void clearWhiteSpace(char *array)
    {
        int countArr = 0, countCl = 0, check = 0;
        char clearWhiteSpace[60];
        // Looping through array.
        while (array[countArr] != '\0')
        {
            // Check character by character for white-spaces character
            if (array[countArr] != ' ')
            {
                // Get each character from the command to clearWhiteSpace array.
                clearWhiteSpace[countCl] = array[countArr];
                // uart_sendc(clearWhiteSpace[countCl]);
                // continue task.
                countCl++;
                countArr++;
            }
            else
            {
                // If character is white-space, continue counting as skipping that character.
                countArr++;
            }
        }
        // Get checkScrSize value from selectCommand function.
        check = selectCommmand(clearWhiteSpace);
        // Check if users is calling scrsize.
        if (check == 1)
        {
            // Using array without white-space cleared for setScreenSize function.
            setScreenSize(array);
        }
    }

    // echo everything back
    while (1)
    {
        char str[60];
        // read each char
        str[count] = uart_getc();
        uart_sendc(str[count]);
        // Detect "Enter" key-press.
        if (str[count] == '\n')
        {
            count = 0;
            // Call function clearWhiteSpace to errase white-space in users input arrary.
            clearWhiteSpace(str);
            // print custom cursor.
            uart_puts("MyBareOS> ");
            // convertCharToInt();
        }
        else
        {
            // continue reading users input if users hasn't pressed enter.
            count++;
        }
    }
}