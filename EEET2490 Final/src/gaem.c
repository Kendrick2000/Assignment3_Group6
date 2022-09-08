#include "uart_uni.h"
#include "mailbox.h"
#include "gfx_frameBuffer.h"
//#include "sys_cmdLine.h"
//#include "sprite.h"
#include "gaem.h"



//DANIIL SHLEE S3414755
int gme_createObject(
    char* name, int x, int y, int velX, int velY, 
    int* sprite, int sprite_hSize, int sprite_vSize, int drawLayer, 
    int cBox_x1, int cBox_y1, int cBox_x2, int cBox_y2)
{
//Iterate through list of in-game objects to find an empty spot:
    for (int i = 0; i < 10; i++)
    {
        if (inGameObjects[i].exists==0)
        {
            inGameObjects[i].exists=1;
            inGameObjects[i].name=name;

            inGameObjects[i].x=x;
            inGameObjects[i].y=y;
            inGameObjects[i].velocityX=velX;
            inGameObjects[i].velocityY=velY;


            inGameObjects[i].sprite=sprite;
            inGameObjects[i].sprite_hSize = sprite_hSize;
            inGameObjects[i].sprite_vSize = sprite_vSize;
            inGameObjects[i].drawLayer=drawLayer;

            inGameObjects[i].cBox_x1 = cBox_x1;
            inGameObjects[i].cBox_y1 = cBox_y1;
            inGameObjects[i].cBox_x2 = cBox_x2;
            inGameObjects[i].cBox_y2 = cBox_y2;



            return i; //Returns the in-game id of the created object

        }
        //else {full_uart_puts("\nUnable to add object to the list - list is full. \n");return 0;} //Object could not be created;
        
    }
    return 999; //FATAL ERROR: For-loop failed to complete for some reason
};


void gme_destroyObject(int inGameObjectListID)
{
//Erase all data at the specified array index:


            inGameObjects[inGameObjectListID].exists=0;
            inGameObjects[inGameObjectListID].name="EMPTY123";

            inGameObjects[inGameObjectListID].x=0;
            inGameObjects[inGameObjectListID].y=0;
            inGameObjects[inGameObjectListID].velocityX=0;
            inGameObjects[inGameObjectListID].velocityY=0;


            inGameObjects[inGameObjectListID].sprite=0;
            inGameObjects[inGameObjectListID].sprite_hSize = 0;
            inGameObjects[inGameObjectListID].sprite_vSize = 0;
            inGameObjects[inGameObjectListID].drawLayer=0;

            inGameObjects[inGameObjectListID].cBox_x1 = 0;
            inGameObjects[inGameObjectListID].cBox_y1 = 0;
            inGameObjects[inGameObjectListID].cBox_x2 = 0;
            inGameObjects[inGameObjectListID].cBox_y2 = 0;

        

};
    
void gme_eraseObjects(int* background)
{
    for (int i = 0; i < maxNumOfGameObjects; i++) 
    {
        //For every entry in the struct list:
        /*
            1. If game object exists (exist = 1), and its velocity is NOT zero in BOTH directions, then redraw that object's sprite.


        */
        if (inGameObjects[i].exists==1 && inGameObjects[i].velocityX!=0 && inGameObjects[i].velocityY!=0)
        {

                eraseSpriteARGB32(inGameObjects[i].sprite,inGameObjects[i].sprite_hSize,inGameObjects[i].sprite_vSize,background,1024,768,inGameObjects[i].x, inGameObjects[i].y);


        }
    }
    

}






void gme_renderObjects()
{
    for (int i = 0; i < maxNumOfGameObjects; i++) 
    {
        //For every entry in the struct list:
        /*
            1. If game object exists (exist = 1), and its velocity is NOT zero in BOTH directions, then redraw that object's sprite.


        */
        if (inGameObjects[i].exists==1 && inGameObjects[i].velocityX!=0 && inGameObjects[i].velocityY!=0)
        {

                drawSpriteARGB32(inGameObjects[i].sprite,inGameObjects[i].sprite_hSize,inGameObjects[i].sprite_vSize,inGameObjects[i].x,inGameObjects[i].y);
                

        }
    }
    

}

void gme_motionUpdate()
{

    for (int i = 0; i < maxNumOfGameObjects; i++) 
        {
                if (inGameObjects[i].exists==1 && inGameObjects[i].velocityX!=0 && inGameObjects[i].velocityY!=0)
                {
                    // if(inGameObjects 
                    inGameObjects[i].x += inGameObjects[i].velocityX; //Update x-coord of the object respective of its velocity in the x-direction
                    inGameObjects[i].y += inGameObjects[i].velocityY; //Update y-coord of the object respective of its velocity in the y-direction
                }
                
        }

}

int gme_collision(int inGameObjectListID){
    if((inGameObjects->cBox_x2>=inGameObjects->cBox_y1>=inGameObjects->cBox_x1 || inGameObjects->cBox_x2>=inGameObjects->cBox_y2>=inGameObjects->cBox_x1) || //bottom corner rectangle collision
    (inGameObjects->cBox_x2>=inGameObjects->cBox_y1==inGameObjects->cBox_y2 || inGameObjects->cBox_y2>=inGameObjects->cBox_x1 >= inGameObjects->cBox_x2 ) ||  //right corner rectangle collision
    (inGameObjects->cBox_x1>=inGameObjects->cBox_y2==inGameObjects->cBox_y1)|| inGameObjects->cBox_y1>=inGameObjects->cBox_x2 >= inGameObjects->cBox_x1 || //left corner rectangle collision
    (inGameObjects->cBox_y1>=inGameObjects->cBox_x1<=inGameObjects->cBox_y2 || inGameObjects->cBox_y2>=inGameObjects->cBox_x2>=inGameObjects->cBox_y2) //top corner rectangle 
    )

    {
        void gme_destroyObject(int inGameObjectListID);
    }

}