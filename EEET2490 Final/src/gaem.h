extern struct gameObject{
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

}gameObject;


extern int maxNumOfGameObjects;
extern struct gameObject inGameObjects[10];


//Function prototypes
int gme_createObject(char* name, int x, int y, int velX, int velY, int* sprite, int sprite_hSize, int sprite_vSize, int drawLayer, int cBox_x1, int cBox_y1, int cBox_x2, int cBox_y2);
void gme_destroyObject(int inGameObjectListID);

void gme_renderObjects();
void gme_motionUpdate();
void gme_eraseObjects(int* background);

int gme_collision();