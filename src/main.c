#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "display.h"
#include "render.h"

const int WIDTH = 1000;
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
    render_setHoizontalFOV(90);

    int N = 60, M = 40;
    float dx = 0.2;
    float dy = 0.2;
    float X_shift = (N/2) * dx;
    Vertex *vertices = (Vertex *)malloc(sizeof(Vertex) * M * N);

    while (!display_closeRequested())
    {

        int delTime = display_getDeltaTime();
        int time = display_getTime();
        printf("Time: %.2f s [del T = %d ms]\r", time/1000.0, delTime);
        
        render_clear((Color){0});

        //render the mesh
        for(int y = 0; y < M; y++){
            for(int x = 0; x < N; x++){
                int i = y*N + x;
                float X = -X_shift + x*dx;
                float Y = y*dy + 0.5;
                float d = sqrt(x*x + y*y);
                float h = 0.04 * sin(2 * 3.14 * 0.8 * time/1000.0 + (0.9) * x);
                h += 0.3 * sin(2 * 3.14 * 0.2 * time/1000.0 - (0.2) * d);
                vertices[i].pos = (Vec3){X, -0.5 + h + 0.07*y, Y};
            }
        }
        for(int m = 0; m < M-1; m++){
            for(int n = 0; n < N-1; n++){
                int i = m*N + n;
                render_triangleMesh(vertices[i], vertices[i+N], vertices[i+N+1], (Color){0, 255, 0, 0});
                render_triangleMesh(vertices[i+N+1], vertices[i+1], vertices[i], (Color){0, 255, 0, 0});
            }
        }

        display_update();
    }

    free(vertices);
    display_cleanup();

    return 0;
}