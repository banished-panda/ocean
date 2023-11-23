#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "display.h"
#include "render.h"

const int WIDTH = 800;
const int HEIGHT = 600;


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

    while (!display_closeRequested())
    {

        int delTime = display_getDeltaTime();
        int time = display_getTime();
        printf("Time: %.2f s [del T = %d ms]\r", time/1000.0, delTime);
        
        render_clear((Color){0});
        float freq = 0.1;
        float phase = 2 * 3.14 * freq * time /1000.0;
        int x = 200 * sin(phase);
        int y = 200 * cos(phase);
        int x_off = WIDTH / 2;
        int y_off = HEIGHT / 2;
        render_line((Point2D){x_off, y_off}, (Point2D){x + x_off, y+y_off}, (Color){255, 0, 255, 0});
        render_line((Point2D){0,0},(Point2D){x + x_off, y+y_off},(Color){255, 255, 0, 0});

        display_update();
    }

    display_cleanup();

    return 0;
}