#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "display.h"
#include "render.h"

const int WIDTH = 800;
const int HEIGHT = 600;

#define NUM_STARS 4000
float posX[NUM_STARS];
float posY[NUM_STARS];
float posZ[NUM_STARS];

void initStar(int index){
    float x = rand() - RAND_MAX/2; x/= (RAND_MAX/2);
    float y = rand() - RAND_MAX/2; y/= (RAND_MAX/2);
    float z = rand() + 1; z/= RAND_MAX;
    posX[index] = x;
    posY[index] = y;
    posZ[index] = z;
}

void initStarfield(){
    for(int i =0; i < NUM_STARS; i++){
        initStar(i);
    }
}

void updateStarfield(int delt){
    float velocity = 0.3 + 0.05 * sin(display_getTime() / 1000.0);
    for(int i = 0; i < NUM_STARS; i++){
        float delPos = velocity * (delt / 1000.0);
        posZ[i] -= delPos;
        if(posZ[i] <= 0.001){
            posZ[i] = 1;
        }
    }
}

void renderStarfield(){
    render_clear((Color){0, 0, 0, 0});
    for(int i = 0; i < NUM_STARS; i++){
        int X = (posX[i] / posZ[i]) * (WIDTH / 2) + (WIDTH /2);
        int Y = (posY[i] / posZ[i]) * (HEIGHT/ 2) + (HEIGHT/2);
        uint8_t shade = (1 - posZ[i]) * 200 + 50;
        render_pixel((Point2D){X, Y}, (Color) {shade, shade, shade, 0});
    }
}

int main(int argc, char *argv[])
{

    display_init(WIDTH, HEIGHT, 1);

    uint8_t *pixelBuffer = display_getPixelBuffer();

    RenderContext context = {0};
    context.HEIGHT = HEIGHT;
    context.WIDTH = WIDTH;
    context.buffer = pixelBuffer;
    context.PITCH = 4;
    context.RED_OFFSET = 0;
    context.GREEN_OFFSET = 1;
    context.BLUE_OFFSET = 2;
    context.ALPHA_OFFSET = 3;

    render_setContext(context);
    initStarfield();

    while (!display_closeRequested())
    {

        int delTime = display_getDeltaTime();
        //printf("Time: %.2f s [del T = %d ms]\r", time/1000.0, delTime);

        updateStarfield(delTime);

        /* do rendering stuufs here*/
        renderStarfield();

        display_update();
    }

    display_cleanup();

    return 0;
}