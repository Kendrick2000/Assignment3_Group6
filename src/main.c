#include "uart.h"
#include "mbox.h"
#include "framebf.h"
#include "printf.h"
#include "image.h"
#include "video.h"
#include "largeImage.h"
#include "game.h"
#include "timer.h"

#define MAX_SIZE 20
//struct for tiles
struct Sprite {
    int x;
    int y;
    int direction;
};


void main()
{
    //Declare variables int for count, score and array of tiles with type "Sprite". 
    int count = 0;
    int score = 0;
    int lives = 3;
    struct Sprite tiles[40];

    //initial uart and framebf.
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

    //Display all members name. 
    drawStringWelcome(250, 200, "EEET2490", 0x003498DB);
    drawStringWelcome(250, 280, "Bare OS", 0x003498DB);

    drawString32x32(250, 400, "Uyen Nguyen - s3751882", 0x003498DB);
    drawString32x32(250, 420, "Khang Truong - s3814172", 0x00AF601A);
    drawString32x32(250, 440, "Daniil Shlee - s3414755", 0x00BB8FCE);
    drawString32x32(250, 460, "Kiet Park - s3681475", 0x00E74C3C);

    //Make the program stop for 1500 seconds.
    wait_msec(1500000);

    // Delete font letters by writing them again in black
    drawStringWelcome(250, 200, "EEET2490", 0x0);
    drawStringWelcome(250, 280, "Bare OS", 0x0);

    drawString32x32(250, 400, "Uyen Nguyen - s3751882", 0x0);
    drawString32x32(250, 420, "Khang Truong - s3814172", 0x0);
    drawString32x32(250, 440, "Daniil Shlee - s3414755", 0x0);
    drawString32x32(250, 460, "Kiet Park - s3681475", 0x0);

    framebf_release();
    framebf_init();

    // Print cursor onto terminal screen
    uart_puts("\n");
    uart_puts("MyBareOS> ");

    //Function draw image 
    void draw_image()
    {
        //Looping through image array line by line.
        for (int j = 0; j < 177; j++)
        {
            //Looping through image array pixel by pixel of line j.
            for (int i = 0; i < 284; i++)
            {
                //Printing each pixel in correct order of the array and lines, columns.
                drawPixelARGB32(i, j, image[j * 284 + i]);
            }
        }
    }

    //Function detect collision between the ball and the paddle.
    /*
    direction 1: x++ y++ -> South East
    direction 2: x-- y-- -> North West
    direction 3: x++ y-- -> North East
    direction 4: x-- y++ -> South West
    Other direction is based on the these four direction. However, the increasement/ drecreasement step is 2 instead
    of 1*/
    int collisionWithPaddle(int ballX, int ballY, int paddleX, int direction)
    {
        //Delare variable dir, pre_dir.
        int dir = 0;
        int pre_dir = 0;
        //set dir to be equal with direction.
        /*In case the function is dectect no collision between the ball and the paddle, it will return the same current
        direction of the ball.*/
        dir = direction;
        /*Condition to check if the bottom of the ball is on the same y axis with the top of the paddle and stay within
        the length of the paddle.*/
        if (((ballX >= paddleX) && (ballX <= paddleX + 127)) || (((ballX + 50) >= paddleX) && (ballX + 50 <= paddleX + 127)))
        {
            // uart_dec(dir);
            if (ballY + 50 == 700){
                //Condition to decide direction whether the ball collide with which part of the paddle will bounce back.
                //If the ball hit the first 40 pixels of the paddle.   
                if((((ballX+50) >= paddleX) && ((ballX+50) <= (paddleX + 40)))){
                    //Direction the ball will bounce back.
                    if (dir == 1)
                        dir = 10;                                        
                    else if (dir == 4)
                        dir = 9; 
                    else dir = 10;

                    
                }
                //If the ball hit the middle part of the paddle (from pixle 30th - 97th).
                else if(((ballX < (paddleX + 87)) && ((ballX+50) > (paddleX + 40)))){
                    //Direction the ball will bounce back. 
                    if (dir == 1)
                        dir = 3;                                        
                    else if (dir == 4)
                        dir = 2;
                    else 
                        dir = 3;

                    
                }
                //If the ball hit the rightmost pixels of the paddle.
                else if(((ballX <= (paddleX + 127)) && ((ballX+50) >= (paddleX + 87)))){
                    //Direction the ball will bounce back.
                   
                    if (dir == 1)
                        dir = 9;                                          
                    else if (dir == 4)
                        dir = 10;
                    else 
                        dir = 9;
                }
            }
            // pre_dir = dir; 
        }
        else if ((ballY + 50 >= 700) && (ballY + 50 <= 730))
        {
            //left side
            if (((ballX + 50) >= paddleX) && ((ballX + 50) <= paddleX + 127))
            {
                if (dir == 1)
                    dir = 10;                                        
                else if (dir == 4)
                    dir = 9; 
                else dir = 10;
            }

            if ((ballX <= (paddleX + 127)) && (ballX >= (paddleX + 127)))
            {
                if (dir == 1)
                    dir = 9;                                          
                else if (dir == 4)
                    dir = 10;
                else 
                    dir = 9;
            }
        }
        else if ((ballX + 50 == paddleX) && (ballY + 50 == 700))
        {
            if (dir == 1)
                dir = 10;                                        
            else if (dir == 4)
                dir = 9; 
            else dir = 10;
        }
        else if ((ballX == paddleX) && (ballY + 50 == 700))
        {
            if (dir == 1)
                dir = 9;                                          
            else if (dir == 4)
                dir = 10;
            else 
                dir = 9;
        }
        return dir;
    }

    int collisionWithBrick(int ballX, int ballY, int brickX, int brickY, int direction)
    {
        //Delare variable dir, pre_dir.
        int dir = 0;
        int pre_dir = 0;
        //set dir to be equal with direction.
        /*In case the function is dectect no collision between the ball and the paddle, it will return the same current
        direction of the ball.*/
        dir = direction;
        /*Condition to check if the bottom of the ball is on the same y axis with the top of the brick and stay within
        the length of the brick.*/
        if (((ballX > brickX) && (ballX < brickX + 240)) || (((ballX + 50) > brickX) && (ballX + 50 < brickX + 240)))
        {
            // uart_dec(dir);
            if (ballY + 50 == brickY){
                //Condition to decide direction whether the ball collide with which part of the paddle will bounce back.
                //If the ball hit the first 60 pixels of the brick.   
                if((((ballX+50) >= brickX) && ((ballX+50) <= (brickX + 80)))){
                    //Direction the ball will bounce back.
                    if (dir == 1)
                        dir = 3;                                        
                    else if (dir == 4)
                        dir = 10; 
                    else dir = 10;                   
                }
                //If the ball hit the middle part of the brick (from pixle 80 - 160).
                else if(((ballX < (brickX + 160)) && ((ballX+50) > (brickX + 80)))){
                    //Direction the ball will bounce back.                      
                    if (dir == 1)
                        dir = 3;                                        
                    else if (dir == 4)
                        dir = 2;
                    else 
                        dir = 3;                    
                }
                //If the ball hit the rightmost pixels of the brick.
                else if(((ballX <= (brickX + 240)) && ((ballX+50) >= (brickX + 160)))){
                    //Direction the ball will bounce back.
                    if (dir == 1)
                        dir = 9;                                          
                    else if (dir == 4)
                        dir = 10;
                    else 
                        dir = 9;
                }
            }
            /*Condition to check if the bottom of the ball is on the same y axis with the bottom of the brick and stay within
            the length of the brick.*/
            // uart_dec(dir);
               
            if (ballY == (brickY + 20)){
                //Condition to decide direction whether the ball collide with which part of the paddle will bounce back.
                //If the ball hit the first 100 pixels of the brick.   
                if((((ballX+50) >= brickX) && ((ballX+50) <= (brickX + 80)))){
                    //Direction the ball will bounce back.
                    //uart_puts("Far Left \n");
                    if (dir == 10)
                        dir = 7;                                        
                    else if (dir == 9)
                        dir = 5; 
                    else dir = 7;                   
                }
                //If the ball hit the middle part of the paddle (from pixle 100 - 200).
                else if(((ballX < (brickX + 160)) && ((ballX+50) > (brickX + 80)))){
                    // Direction the ball will bounce back. 
                    // uart_puts("Middle Part"); 
                    if (dir == 2)
                        dir = 4;                                        
                    else if (dir == 3)
                        dir = 1;
                    else 
                        dir = 4;                   
                }
                //If the ball hit the rightmost pixels of the brick.
                else if(((ballX <= (brickX + 240)) && ((ballX+50) >= (brickX + 160)))){
                    //Direction the ball will bounce back.
                    //uart_puts("Far Right");
                    if (dir == 9)
                        dir = 5;                                          
                    else if (dir == 3)
                        dir = 1;
                    else 
                        dir = 5;
                }
            }
            
            // pre_dir = dir; 
        }
        //Checking if the ball hit tile/s with partial of the ball from side to side.
        else if ((ballY <= (brickY + 20)) && (ballY >= brickY)){
            //Checking if partial of the ball hit brick from the left side of the tile.
            if((ballX + 50) == brickX) {
                if (dir == 9) 
                    dir = 10;                                          
                else if (dir == 1) 
                    dir = 4;
                else if (dir == 3) 
                    dir = 2;  
                else if (dir == 5) 
                    dir = 7;                                        
                else
                    dir = 2;                  
            }

            //Checking if partial of the ball hit brick from the right side of the tile.
            if(ballX == (brickX + 240)) {
                if (dir == 2) 
                    dir = 3;                                           
                else if (dir == 10) 
                    dir = 9;   
                else if (dir == 7) 
                    dir = 5;  
                else if (dir == 4) 
                    dir = 1;                                      
                else
                    dir = 3;                    
            }
        }
        //Checking if the ball hit brick fully side to side.
        else if (((brickY >= ballY) && (brickY <= (ballY + 50)))){
            if (ballX + 50 == brickX) {
                if (dir == 9) 
                    dir = 10;                                          
                else if (dir == 1) 
                    dir = 4;
                else if (dir == 3) 
                    dir = 2;  
                else if (dir == 5) 
                    dir = 7;                                        
                else
                    dir = 2;                   
            }

            if (ballX == (brickX + 240)){
                if (dir == 2) 
                    dir = 3;                                           
                else if (dir == 10) 
                    dir = 9;   
                else if (dir == 7) 
                    dir = 5;  
                else if (dir == 4) 
                    dir = 1;                                      
                else
                    dir = 3;                   
            }
        }
        return dir;
    }
    //Function delete tiles coordinate in tiles array create in global. 
    void deleteTileCoordinate(int position){
        /*Declare variable count and set equal to position of a specific tiles that has been dectected as collided in 
        function dectectCollision.*/
        int count = position;
        //Looping through tiles array to the end of the array.
        while(tiles[count].x != '\0'){
            //Get to the posistion of the tile in the array.
            //Override that tile's coordinate with following tile's coordinate.
            tiles[count].x = tiles[count + 1].x;
            tiles[count].y = tiles[count + 1].y;
            //Continue doing so until the last element.
            count++;
        }
    }

    //Function detect collision between the ball and tiles.
    int detectCollision(int x, int y, int direction){
        //Declare variables: count, dir, pre_dir.
        int count = 0;
        int dir = 0; 
        int pre_dir = 0;
        /*Set dir to be equal with direction, so that if the function dectect no collision between the ball and the tiles
        it will maintance current direction of the ball.*/
        dir = direction;
        pre_dir = dir;
        for(int i = 0; i < 40; i++){
            // Collision on the top and bottom of the tiles
            if (((x > tiles[i].x) && (x < (tiles[i].x + 150))) || (((x + 50) > tiles[i].x) && ((x + 50) < (tiles[i].x + 150)))){
                /*Check if the ball hits a tile from bottom up.*/ 
                if (y == (tiles[i].y + 30)){   
                    //Call function delete tile for visually and in the tiles array.         
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    score += 5;
                    if (pre_dir == 9) 
                        dir = 5;                                         
                    else if (pre_dir == 3) 
                        dir = 1;                                         
                    else if (pre_dir == 2) 
                        dir = 4;                                         
                    else if (pre_dir == 10) 
                        dir = 7;                                          
                    else
                        dir = 1;                    
                }

                /*Check is the ball hit a tile from top down.*/
                if ((y + 50) == tiles[i].y){
                    //Call function delete tile for visually and in the tiles array.
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    score += 5;
                    if (pre_dir == 1) 
                        dir = 3;                       
                    else if (pre_dir == 5) 
                        dir = 9;                                          
                    else if (pre_dir == 4) 
                        dir = 2;                                          
                    else if (pre_dir == 7) 
                        dir = 10;                                          
                    else
                        dir = 3;
                    
                }   
            }
            // Collision in the top left corner of the tile
            else if (((x + 50) == tiles[i].x) && ((y + 50) == tiles[i].y)){
                //Call function delete tile for visually and in the tiles array.
                del_tile(tiles[i].x, tiles[i].y);
                deleteTileCoordinate(i);
                score += 5;
                if (pre_dir == 1) 
                    dir = 3;            
                else if (pre_dir == 5) 
                    dir = 9;                                      
                else if (pre_dir == 4)
                    dir = 2;                       
                else if (pre_dir == 7) 
                    dir = 10;                                      
                else
                    dir = 2;                
            }
            // Collision in the bottom left corner of the tile
            else if (((x + 50) == tiles[i].x) && (y == (tiles[i].y + 30))){
                //Call function delete tile for visually and in the tiles array.
                del_tile(tiles[i].x, tiles[i].y);
                deleteTileCoordinate(i);
                score += 5;
                if (pre_dir == 9) 
                    dir = 5;                                         
                else if (pre_dir == 3) 
                    dir = 1;                                         
                else if (pre_dir == 2) 
                    dir = 4;                                         
                else if (pre_dir == 10) 
                    dir = 7;                                          
                else
                    dir = 4;       
            }
            // Collision in the top left corner of the tile
            else if ((x == tiles[i].x) && ((y + 50) == tiles[i].y)){
                //Call function delete tile for visually and in the tiles array.
                del_tile(tiles[i].x, tiles[i].y);
                deleteTileCoordinate(i);
                score += 5;
                if (pre_dir == 1) 
                    dir = 3;            
                else if (pre_dir == 5) 
                    dir = 9;                                      
                else if (pre_dir == 4)
                    dir = 2;                       
                else if (pre_dir == 7) 
                    dir = 10;                                      
                else
                    dir = 3;               
            }
            // Collision in the bottom right corner of the tile
            else if ((x == (tiles[i].x + 150)) && (y == (tiles[i].y + 30))){
                //Call function delete tile for visually and in the tiles array.
                del_tile(tiles[i].x, tiles[i].y);
                deleteTileCoordinate(i);
                score += 5;
                if (pre_dir == 9) 
                    dir = 5;                                         
                else if (pre_dir == 3) 
                    dir = 1;                                         
                else if (pre_dir == 2) 
                    dir = 4;                                         
                else if (pre_dir == 10) 
                    dir = 7;                                          
                else
                    dir = 1;              
            }
            //Checking if the ball hit tile/s with partial of the ball from side to side.
            else if ((y <= tiles[i].y) && (y >= (tiles[i].y + 30))){
                //Checking if partial of the ball hit tile/s from the left side of the tile.
                if((x + 50) == tiles[i].x){
                    //Call function delete tile for visually and in the tiles array.
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    score += 5;
                    if (pre_dir == 9) 
                        dir = 10;                                          
                    else if (pre_dir == 1) 
                        dir = 4;
                    else if (pre_dir == 3) 
                        dir = 2;  
                    else if (pre_dir == 5) 
                        dir = 7;                                        
                    else
                        dir = 2;                  
                }

                //Checking if partial of the ball hit tile/s from the right side of the tile.
                if(x == (tiles[i].x + 150)){
                    //Call function delete tile for visually and in the tiles array.
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    score += 5;
                    if (pre_dir == 2) 
                        dir = 3;                                           
                    else if (pre_dir == 10) 
                        dir = 9;   
                    else if (pre_dir == 7) 
                        dir = 5;  
                    else if (pre_dir == 4) 
                        dir = 1;                                      
                    else
                        dir = 3;                    
                }
            }
            //Checking if the ball hit tile/s fully side to side.
            else if (((tiles[i].y >= y) && (tiles[i].y <= (y + 50)))){
                if (x + 50 == tiles[i].x) {
                    //Call function delete tile for visually and in the tiles array.
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    score += 5;
                    if (pre_dir == 9) 
                        dir = 10;                                          
                    else if (pre_dir == 1) 
                        dir = 4;
                    else if (pre_dir == 3) 
                        dir = 2;  
                    else if (pre_dir == 5) 
                        dir = 7;                                        
                    else
                        dir = 2;                   
                }

                if (x == (tiles[i].x + 150)){
                    //Call function delete tile for visually and in the tiles array.
                    del_tile(tiles[i].x, tiles[i].y);
                    deleteTileCoordinate(i);
                    score += 5;
                    
                    if (pre_dir == 2) 
                        dir = 3;                                           
                    else if (pre_dir == 10) 
                        dir = 9;   
                    else if (pre_dir == 7) 
                        dir = 5;  
                    else if (pre_dir == 4) 
                        dir = 1;                                      
                    else
                        dir = 3;                   
                }
            }
            pre_dir = dir;
        }
        //return any value dir have.
        return dir;
    }
    
    void draw_level2() {
        //Declare variables: barX, ballX, ballY, traceX, traceY, direction, i, isCollision, isInitial and str array. 
        int barX = 500, count = 0;
        int ballX = 500, ballY = 650;
        int leftbrickX = 130, rightbrickX = 690;
        int leftbrickY = 340, rightbrickY = 340;
        int traceX = 0, traceY = 0;
        int direction = 0;
        int i = 0; 
        int pre_dir = 0;
        int isInitial = 0;
        char str[10000];
        //Draw image background.
        draw_background();
        //Draw initial paddle with possition 500, 700.
        draw_paddle(barX, 700);
        draw_greyBrick(leftbrickX,leftbrickY);
        draw_greyBrick(rightbrickX,rightbrickY);

        //Run the game.
        while ((lives > 0))
        {
            //Display score that player currently has on top right corrner.
            drawString32x32(800,10,"Score: ",0x00E74C3C);
            displayDec(score, 930, 10);
            drawString32x32(10 , 10, "Lives: ", 0x00E74C3C);
            displayDec(lives, 130, 10);

            //Condition to make sure that all tiles only drew once at the begining of the game.
            if(isInitial == 0){
                //Using loop as pointer, point to to exact coordinate to draw tiles.
                //x value as value of x-axis and y value as value of y-axis.
                //+=170 or +=32 as step to get to the exact location faster then ++ or having more condition.
                for (int x = 90; x < 400; x += 170)
                {
                    for (int y = 50; y < 280; y += 32)
                    {
                        //Condition to draw all tiles in a column at correct position.
                        if (y == 50)
                        {
                            //Draw yellow tile at position any x, and y = 50.
                            draw_yellowTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 82)
                        {
                            //Draw red tile at position any x, and y = 82.
                            draw_redTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 114)
                        {
                            //Draw blue tile at position any x, and y = 114.
                            draw_blueTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 146)
                        {
                            //Draw green tile at position any x, and y = 146.
                            draw_greenTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 178)
                        {
                            //Draw yellow tile at position any x, and y = 178.
                            draw_yellowTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 210)
                        {
                            //Draw blue tile at position any x, and y = 210.
                            draw_blueTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 242)
                        {
                            //Draw green tile at position any x, and y = 242.
                            draw_greenTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 274)
                        {
                            //Draw red tile at position any x, and y = 274.
                            draw_redTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                    }
                }
                // Draw the tiles on the right column
                for (int x = 650; x < 950; x += 170)
                {
                    for (int y = 50; y < 280; y += 32)
                    {
                        //Condition to draw all tiles in a column at correct position.
                        if (y == 50)
                        {
                            //Draw yellow tile at position any x, and y = 50.
                            draw_yellowTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 82)
                        {
                            //Draw red tile at position any x, and y = 82.
                            draw_redTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 114)
                        {
                            //Draw blue tile at position any x, and y = 114.
                            draw_blueTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 146)
                        {
                            //Draw green tile at position any x, and y = 146.
                            draw_greenTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 178)
                        {
                            //Draw yellow tile at position any x, and y = 178.
                            draw_yellowTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 210)
                        {
                            //Draw blue tile at position any x, and y = 210.
                            draw_blueTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 242)
                        {
                            //Draw green tile at position any x, and y = 242.
                            draw_greenTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 274)
                        {
                            //Draw red tile at position any x, and y = 274.
                            draw_redTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                    }
                }
                draw_pixelBall(ballX, ballY);
                uart_getc();
                /*Set isinitial as 1. It's mean that initial run has been completed and the program is not allowed to
                run this loop again during game play.*/
                isInitial = 1;
            }

            //Condition to maintance the ball stay with the frame screen. 
            if ((ballX <= 955 && ballX >= 0) && (ballY <= 786 && ballY >= 0))
            {
                //Called draw_pixelBall constantly with coordinate value of ballX and ballY.
                draw_pixelBall(ballX, ballY);
                /*Get direction from function collisionWithPaddle function for the ball change direction when it hit 
                the paddle.*/
                direction = collisionWithPaddle(ballX, ballY, barX, direction);

                //divide all three side of the screen frame into half(the screen frame will be called as wall).
                /*Check if the ball hit the top wall, and the ball hit left side of the wall.*/
                if ((ballY == 0)) {                  
                    if (direction == 2)                    
                        direction = 4;                    
                    else if (direction == 3)                   
                        direction = 1;
                    else if (direction == 10)                   
                        direction = 7;
                    else if (direction == 9)                   
                        direction = 5;   
                    else 
                        direction = 1;                
                }
                else if ((ballX == 0) || (ballX == 1))
                {
                    if (direction == 4)                    
                        direction = 1;                   
                    else if (direction == 2)                   
                        direction = 3;                    
                    else if (direction == 10)                   
                        direction = 9;  
                    else if (direction == 7)                   
                        direction = 5;
                    else 
                        direction = 1;                        
                }
                else if ((ballX == 955) || (ballX == 954))
                {
                    if (direction == 1)                   
                        direction = 4;                    
                    else if (direction == 3)                   
                        direction = 2;                   
                    else if (direction == 9)                   
                        direction = 10;
                    else if (direction == 5)                   
                        direction = 7;
                    else 
                        direction = 2;
                    
                }
                
                        
                //Condition to let the ball fly in direction 1.
                if (direction == 1)
                {
                    ballX++;
                    ballY++;
                    // isMidAir = 1;
                    if (ballX == 955)
                    {
                        continue;
                    }
                }
                //Condition to let the ball fly in direction 2.
                else if (direction == 2)
                {
                    ballX--;
                    ballY--;
                    // isMidAir = 1;
                    if ((ballX == 0) || (ballY == 0))
                    {
                        continue;
                    }
                }
                //Condition to let the ball fly in direction 3.
                else if (direction == 3)
                {
                    ballX++;
                    ballY--;
                    // isMidAir = 1;
                    if (ballX == 955 || ballY == 0)
                    {
                        continue;
                    }
                }
                //Condition to let the ball fly in direction 4.
                else if (direction == 4)
                {
                    ballX--;
                    ballY++;
                    // isMidAir = 1;
                    if (ballX == 0)
                    {
                        continue;
                    }
                }
                else if (direction == 5)
                {
                    ballX = ballX + 2;
                    ballY++;
                    if (ballX == 956)
                    {
                        continue;
                    }
                }
                // Fall straight downwards
                else if (direction == 6)
                {
                    ballY++;
                    if (ballY == 955)
                    {
                        continue;
                    }
                }
                else if (direction == 7)
                {
                    ballX = ballX - 2;
                    ballY++;
                    if (ballX == 0)
                    {
                        continue;
                    }
                }
                else if (direction == 8)
                {
                    ballX++;
                    ballY = ballY - 2;
                    if (ballX == 955 || ballY == 0)
                    {
                        continue;
                    }
                }
                else if (direction == 9)
                {
                    ballX += 2;
                    ballY--;
                    if (ballX == 955 || ballY == 0)
                    {
                        continue;
                    }
                }
                else if (direction == 10)
                {
                    ballX -= 2;
                    ballY--;
                    if (ballX == 0 || ballY == 0)
                    {
                        continue;
                    }
                }
                /*the ball fly directly upward as begining of the game, or after returned when players faild to catch 
                the ball with the paddle.*/ 
                else
                {
                    ballY--;
                    if (ballY == 0)
                    {
                        continue;
                    }
                }
                // uart_dec(ballX);
                // uart_puts(" ");
                direction = collisionWithBrick(ballX, ballY, leftbrickX, leftbrickY, direction);
                direction = collisionWithBrick(ballX, ballY, rightbrickX, rightbrickY, direction);
                //Get direction from function detectCollision if the ball hit tiles.
                direction = detectCollision(ballX, ballY, direction);    
                //Slow down the running speed of the program to allow hummand can see the ball.
                wait_msec(3000); 
            }
            else
            {
                lives--;
                //reset ballX to 500, ballY to 650 and direction to 0 when the ball drop of from the screen.
                ballX = 500;
                ballY = 650;
                barX = 500;
                draw_background(barX, 690);
                draw_paddle(barX, 700);
                direction = 0;
            }

            //Get keyboard input with function getUart.
            str[count] = getUart();
            if (str[count] != '\0'){
                //Checking if the keyboard input at the moment is d, then the paddle will move right.
                if (str[count] == 'd'){
                    if( barX <= 900){
                        //Call move_paddle function to cover the trace of the paddle when it move.
                        move_paddle(str, barX);
                        //Pluse barX to 50 pixels as step of the paddle.
                        barX+=50;
                        //Re-draw paddle at new location, 50 pixels to the right.
                        draw_paddle(barX,700);
                        
                    }
                }

                //Checking if the keyboard input at the moment is a, then the paddle will move left.
                if (str[count] == 'a'){
                    if(barX >= 0){
                        //Call move_paddle function to cover the trace of the paddle when it move.
                        move_paddle(str, barX);
                        //Pluse barX to 50 pixels as step of the paddle.
                        barX-=50;
                        //Re-draw paddle at new location, 50 pixels to the left.
                        draw_paddle(barX,700);
                    }
                }
            }

            //Stop the game.
            if (str[count] == 'c')
            {
                count++;
                break;
            }
            
            if (lives == 0)
            {
                draw_gameOver(score);
            }

            if (score == 200)
            {
                winGame(score);
            }
        }
    }

    //Function start game
    void draw_game()
    {
        //Declare variables: barX, ballX, ballY, traceX, traceY, direction, i, isCollision, isInitial and str array. 
        int barX = 500, barFlag = 0, count = 0;
        int ballX = 500, ballY = 650;
        int traceX = 0, traceY = 0;
        int direction = 0;
        int i = 0; 
        int pre_dir = 0;
        int isCollision = 0, isInitial = 0, isMidAir = 0;
        char str[10000];
        //Draw image background.
        draw_background();
        //Draw initial paddle with possition 500, 700.
        draw_paddle(barX, 700);

        //Run the game.
        while ((score != 200) && (lives > 0))
        {
            //Display score that player currently have on top right corrner.
            drawString32x32(800,10,"Score: ",0x00E74C3C);
            displayDec(score, 930, 10);
            drawString32x32(10 , 10, "Lives: ", 0x00E74C3C);
            displayDec(lives, 130, 10);
            //uart_dec(myscore);
            //drawChar32x32(930,10,myscore,0x00E74C3C);
            //Condition to make sure that all tiles only drew once at the begining of the game.
            if(isInitial == 0){
                //Using loop as pointer, point to to exact coordinate to draw tiles.
                //x value as value of x-axis and y value as value of y-axis.
                //+=170 or +=32 as step to get to the exact location faster then ++ or having more condition.
                for (int x = 90; x < 900; x += 170)
                {
                    for (int y = 50; y < 280; y += 32)
                    {
                        //Condition to draw all tiles in a column at correct position.
                        if (y == 50)
                        {
                            //Draw yellow tile at position any x, and y = 50.
                            draw_yellowTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 82)
                        {
                            //Draw red tile at position any x, and y = 82.
                            draw_redTile(x, y);
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 114)
                        {
                            //Draw blue tile at position any x, and y = 114.
                            draw_blueTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 146)
                        {
                            //Draw green tile at position any x, and y = 146.
                            draw_greenTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 178)
                        {
                            //Draw yellow tile at position any x, and y = 178.
                            draw_yellowTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 210)
                        {
                            //Draw blue tile at position any x, and y = 210.
                            draw_blueTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 242)
                        {
                            //Draw green tile at position any x, and y = 242.
                            draw_greenTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                        else if (y == 274)
                        {
                            //Draw red tile at position any x, and y = 274.
                            draw_redTile(x, y);
                            //saving the tile corrdinate to tiles array.
                            tiles[i].x = x;
                            tiles[i].y = y;
                            i++;
                        }
                    }
                }
                draw_pixelBall(ballX, ballY);
                uart_getc();
                /*Set isinitial as 1. It's mean that initial run has been completed and the program is not allowed to
                run this loop again during game play.*/
                isInitial = 1;
            }

            //Condition to maintance the ball stay with the frame screen. 
            if ((ballX <= 955 && ballX >= 0) && (ballY <= 786 && ballY >= 0))
            {
                //Called draw_pixelBall constantly with coordinate value of ballX and ballY.
                draw_pixelBall(ballX, ballY);
                /*Get direction from function collisionWithPaddle function for the ball change direction when it hit 
                the paddle.*/
                direction = collisionWithPaddle(ballX, ballY, barX, direction);

                //divide all three side of the screen frame into half(the screen frame will be called as wall).
                /*Check if the ball hit the top wall, and the ball hit left side of the wall.*/
                if ((ballY == 0)) {                  
                    if (direction == 2)                    
                        direction = 4;                    
                    else if (direction == 3)                   
                        direction = 1;
                    else if (direction == 10)                   
                        direction = 7;
                    else if (direction == 9)                   
                        direction = 5;   
                    else 
                        direction = 1;                
                }
                else if ((ballX == 0) || (ballX == 1))
                {
                    if (direction == 4)                    
                        direction = 1;                   
                    else if (direction == 2)                   
                        direction = 3;                    
                    else if (direction == 10)                   
                        direction = 9;  
                    else if (direction == 7)                   
                        direction = 5;
                    else 
                        direction = 1;                        
                }
                else if ((ballX == 955) || (ballX == 954))
                {
                    if (direction == 1)                   
                        direction = 4;                    
                    else if (direction == 3)                   
                        direction = 2;                   
                    else if (direction == 9)                   
                        direction = 10;
                    else if (direction == 5)                   
                        direction = 7;
                    else 
                        direction = 2;
                    
                }
                             
                //Condition to let the ball fly in direction 1.
                if (direction == 1)
                {
                    ballX++;
                    ballY++;
                    // isMidAir = 1;
                    if (ballX == 955)
                    {
                        continue;
                    }
                }
                //Condition to let the ball fly in direction 2.
                else if (direction == 2)
                {
                    ballX--;
                    ballY--;
                    // isMidAir = 1;
                    if ((ballX == 0) || (ballY == 0))
                    {
                        continue;
                    }
                }
                //Condition to let the ball fly in direction 3.
                else if (direction == 3)
                {
                    ballX++;
                    ballY--;
                    // isMidAir = 1;
                    if (ballX == 955 || ballY == 0)
                    {
                        continue;
                    }
                }
                //Condition to let the ball fly in direction 4.
                else if (direction == 4)
                {
                    ballX--;
                    ballY++;
                    // isMidAir = 1;
                    if (ballX == 0)
                    {
                        continue;
                    }
                }
                else if (direction == 5)
                {
                    ballX = ballX + 2;
                    ballY++;
                    if (ballX == 956)
                    {
                        continue;
                    }
                }
                // Fall straight downwards
                else if (direction == 6)
                {
                    ballY++;
                    if (ballY == 955)
                    {
                        continue;
                    }
                }
                else if (direction == 7)
                {
                    ballX = ballX - 2;
                    ballY++;
                    if (ballX == 0)
                    {
                        continue;
                    }
                }
                else if (direction == 8)
                {
                    ballX++;
                    ballY = ballY - 2;
                    if (ballX == 955 || ballY == 0)
                    {
                        continue;
                    }
                }
                else if (direction == 9)
                {
                    ballX += 2;
                    ballY--;
                    if (ballX == 955 || ballY == 0)
                    {
                        continue;
                    }
                }
                else if (direction == 10)
                {
                    ballX -= 2;
                    ballY--;
                    if (ballX == 0 || ballY == 0)
                    {
                        continue;
                    }
                }
                /*the ball fly directly upward as begining of the game, or after returned when players faild to catch 
                the ball with the paddle.*/ 
                else
                {
                    ballY--;
                    if (ballY == 0)
                    {
                        continue;
                    }
                }
                //Get direction from function detectCollision if the ball hit tiles.
                direction = detectCollision(ballX, ballY, direction);    
                //Slow down the running speed of the program to allow hummand to see the ball.
                wait_msec(4000); 
            }
            else
            {
                lives--;
                //reset ballX to 500, ballY to 650 and direction to 0 when the ball drop of from the screen.
                ballX = 500;
                ballY = 650;
                barX = 500;
                draw_background(barX, 690);
                draw_paddle(barX, 700);
                direction = 0;
            }

            //Get keyboard input with function getUart.
            str[count] = getUart();
            if (str[count] != '\0'){
                //Checking if the keyboard input at the moment is d, then the paddle will move right.
                if (str[count] == 'd'){
                    if( barX <= 900){
                        //Call move_paddle function to cover the trace of the paddle when it move.
                        move_paddle(str, barX);
                        //Pluse barX to 50 pixels as step of the paddle.
                        barX+=50;
                        //Re-draw paddle at new location, 50 pixels to the right.
                        draw_paddle(barX,700);
                        
                    }
                }

                //Checking if the keyboard input at the moment is a, then the paddle will move left.
                if (str[count] == 'a'){
                    if(barX >= 0){
                        //Call move_paddle function to cover the trace of the paddle when it move.
                        move_paddle(str, barX);
                        //Pluse barX to 50 pixels as step of the paddle.
                        barX-=50;
                        //Re-draw paddle at new location, 50 pixels to the left.
                        draw_paddle(barX,700);
                    }
                }
            }

            //Stop the game.
            if (str[count] == 'c')
            {
                count++;
                break;
            }

            if (lives == 0)
            {
                clear_screen();
                draw_gameOver(score);
                if (uart_getc() == 'r')
                {
                    score = 0;
                    draw_game();
                }
                
            }

            if (score == 200)
            {
                clear_screen();
                winGame(score);
                score = 0;
                if (uart_getc())
                {
                    draw_level2();
                }
                
            }
        }
    }

    //Function display video. 
    void draw_video()
    {
        //Declare variable countFrame
        int countFrame = 0;
        //Looping through all 5 frames
        while (countFrame < 5)
        {
            //Draw each frame as an image (draw pixels by pixels). 
            for (int j = 0; j < 240; j++)
            {
                for (int i = 0; i < 320; i++)
                {
                    //Draw frame accordingly to countFrame value.
                    if (countFrame == 0)
                    {
                        //Draw first frame.
                        drawPixelARGB32(i, j, video[j * 320 + i]);
                    }
                    else if (countFrame == 1)
                    {
                        //Draw second frame.
                        drawPixelARGB32(i, j, video1[j * 320 + i]);
                    }
                    else if (countFrame == 2)
                    {
                        //Draw third frame.
                        drawPixelARGB32(i, j, video2[j * 320 + i]);
                    }
                    else if (countFrame == 3)
                    {
                        //Draw fourth frame.
                        drawPixelARGB32(i, j, video3[j * 320 + i]);
                    }
                    else if (countFrame == 4)
                    {
                        //Draw fifth frame.
                        drawPixelARGB32(i, j, video4[j * 320 + i]);
                    }
                    else
                    {
                        uart_puts("No more frame");
                    }
                }
            }
            //Continue loop until out of range
            countFrame++;
        }
    }

    //Function scrolling up/down of an image.
    void drawLargeImage()
    {
        //Declare variable: count, countY, flag
        //str array to recieve keyboard input.
        char str[10000];
        int count = 0, countY = 0, flag = 0;
        //Draw image pixel by pixel.
        for (int j = 0; j < 1206; j++)
        {
            for (int i = 0; i < 1000; i++)
            {
                drawPixelARGB32(i, j, largeImage[j * 1000 + i]);
            }
        }

        while (1)
        {
            //Get keyboard pressed.
            str[count] = uart_getc();

            //Checking if key pressed is 's'.
            if (str[count] == 's')
            {
                if (flag <= 500)
                {
                    //Increasing value for countY by 100
                    countY = flag + 100;
                    //Storing latest value for countY
                    flag = countY;
                    //Re-draw the same image with different starting point in the array.
                    for (int j = 0; j < 1206; j++)
                    {
                        //increasing countY by one for every time j increase
                        countY++;
                        for (int i = 0; i < 1000; i++)
                        {
                            //Draw image pixel by pixel with different position in the largeImage array.(100 line down every time)
                            drawPixelARGB32(i, j, largeImage[countY * 1000 + i]);
                        }
                    }
                }
                //increase count only when correct keyboard is pressed
                //This avoid array str get out of range due to count constantly increase without any key board pressed.
                count++;
            }

            if (str[count] == 'w')
            {
                if (flag > 0)
                {
                    //Decreasing value for countY by 100
                    countY = flag - 100;
                    //Storing latest value for countY
                    flag = countY;
                    //Re-draw the same image with different starting point in the array.
                    framebf_init(pWidth, pHeight, vWidth, vHeight);
                    for (int j = 0; j < 1206; j++)
                    {
                        //increasing countY by one for every time j increase
                        countY++;
                        for (int i = 0; i < 1000; i++)
                        {
                            //Draw image pixel by pixel with different position in the largeImage array.(100 line down every time)
                            drawPixelARGB32(i, j, largeImage[countY * 1000 + i]);
                        }
                    }
                }
                //increase count only when correct keyboard is pressed
                //This avoid array str get out of range due to count constantly increase without any key board pressed.
                count++;
            }  
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
            uart_puts("img                      Display a small image.\n");
            uart_puts("video                    Display a small video.\n");
            uart_puts("image                    Display a big image.\n");
            uart_puts("game                     Play ARKANOID game.\n");
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
            else if (strCompare(command, "img"))
            {
                uart_puts("Display an image.                            img\n");
            }
            else if (strCompare(command, "video"))
            {
                uart_puts("Display a video.                             video\n");
            }
            else if (strCompare(command, "image"))
            {
                uart_puts("Display a big image                          image");
                uart_puts("users can scroll by pressing 'w' for scroll up and 's' for scroll down.");
            }
            else if (strCompare(command, "game"))
            {
                uart_puts("Playing ARKANOID game on BareOS");
            }
            else
            {
                uart_puts("Commnad name is not valid or not avaliable.\n");
            }
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
        else if (strCompare(array, "image") == 0)
        {
            drawLargeImage();
        }
        else if (strCompare(array, "game") == 0)
        {
           welcomeGame();
           uart_getc();
           draw_game();
        }
        else if (strCompare(array, "help") == 0)
        {
            helpCommand(array);
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

        /*/ Check for backspace
        if (str[count] != '\b') {
            uart_sendc(str[count]);
            count++;
        } else if (str[count] == '\b' && count > 0) {
            uart_sendc(str[count]);
        }
        */

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
            if ( (str[count] == '\b') ) {            // If user types backspace, delete a character
				
                if(count > 0){
                    uart_puts("\x20\b");
                    count--;
                } 
            }
            else 
                // continue reading users input if users hasn't pressed enter.
                count++;
        }
    }
}