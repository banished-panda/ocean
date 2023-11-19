#include <stdio.h>
#include "display.h"
#include "render.h"

int main(int argc, char *argv[])
{
    const int WIDTH = 800;
    const int HEIGHT = 600;

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

        int time = display_getTime();
        int delTime = display_getDeltaTime();
        printf("Time: %.2f s [del T = %d ms]\r", time/1000.0, delTime);

        /* do rendering stuufs here*/
        Color bg_color = {0};
        bg_color.R = 90;
        bg_color.G = 90;
        bg_color.B = 90;
        render_clear(bg_color);

        display_update();
    }

    display_cleanup();

    return 0;
}