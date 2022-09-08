struct gameObject{
        int exists;
        char *name;
        int x;
        int y;
        int velocityX; //Amount of pixels to move the object in the X-direction every frame
        int velocityY; //Amount of pixels to move the object in the Y-direction every frame

        int *sprite; //Sprite to display at object's xy coords
        int sprite_hSize;
        int sprite_vSize;

        int drawLayer; //The order the object is drawn on the screen
        int cBox_x1;
        int cBox_y1;
        int cBox_x2;
        int cBox_y2;


};

int maxNumOfGameObjects = 10;
struct gameObject inGameObjects[10];



