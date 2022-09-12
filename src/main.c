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

    wait_msec(1500000);

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
        if ((ballY + 50 == 700) && (((ballX+25) >= paddleX) && ((ballX+25) <= paddleX + 127)))
        {
            // uart_puts("Hello from collision");
            // uart_puts("\n");
            if((((ballX+25) >= paddleX) && ((ballX+25) <= paddleX + 30))){
                dir = 2;
            }
            else if(((ballX <= paddleX + 97) && ((ballX+25) > paddleX + 30))){
                dir = 3;
            }
            else if(((ballX <= paddleX + 127) && ((ballX+25) > paddleX + 97))){
                dir = 3;
            }
        }
        return dir;
    }

    void deleteTileCoordinate(int position){
       int count = position;
        while(tiles[count].x != '\0'){
            tiles[count].x = tiles[count + 1].x;
            tiles[count].y = tiles[count + 1].y;
            count++;
        }
    }

    int detectCollision(int x, int y, int direction){
        int count = 0;
        //struct Sprite tiles[40];
        int distanceX = 0, distanceY = 0;
        int isCollision = 0;
        int dir = 0;
        dir = direction;
        for(int i = 0; i < 40; i++){
            if (((x > tiles[i].x) && (x < (tiles[i].x + 150))) || (((x + 50) > tiles[i].x) && ((x + 50) < (tiles[i].x + 150)))){
                if (y == (tiles[i].y + 32)){
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    dir = 1;
                }

                if ((y + 50) == tiles[i].y){
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    dir = 3;
                }   
           }
           else if (((x + 50) == tiles[i].x) && ((y + 50) == tiles[i].y)){
                del_tile(tiles[i].x, tiles[i].y);
                deleteTileCoordinate(i);
                dir = 2;
            }
            else if (((x + 50) == tiles[i].x) && (y == (tiles[i].y + 32))){
                del_tile(tiles[i].x, tiles[i].y);
                deleteTileCoordinate(i);
                dir = 4;
            }
            else if ((x == tiles[i].x) && ((y + 50) == tiles[i].y)){
                del_tile(tiles[i].x, tiles[i].y);
                deleteTileCoordinate(i);
                dir = 3;
            }
            else if ((x == (tiles[i].x + 150)) && (y == (tiles[i].y + 32))){
                del_tile(tiles[i].x, tiles[i].y);
                deleteTileCoordinate(i);
                dir = 1;
            }
            else if ((y <= tiles[i].y) && (y >= (tiles[i].y + 32))){
                if((x + 50) == tiles[i].x){
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                     dir = 2;
                }

                if(x == (tiles[i].x + 150)){
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    dir = 3;
                }
            }
            else if (((tiles[i].y >= y) && (tiles[i].y <= (y + 50)))){
                if (x + 50 == tiles[i].x){
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    dir = 2;
                }

                if (x == (tiles[i].x + 150)){
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    dir = 3;
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
                        barX+=50;
                        draw_paddle(barX,700);
                        
                    }
                }

                if (str[count] == 'a'){
                    if(barX >= 0){
                        move_paddle(str, barX);
                        barX-=50;
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

    // Direct Users to correct function corresponding to commands.
    int selectCommmand(char *array)
    {
        int checkScrSize = 0;
        if (strCompare(array, "img") == 0)
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