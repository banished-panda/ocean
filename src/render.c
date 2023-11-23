#include "render.h"
#include "math.h"
#include "stdio.h"

static RenderContext context;
static float half_hFOV, half_vFOV;
static int HALF_WIDTH, HALF_HEIGHT;

void render_setContext(RenderContext ctx)
{
    context = ctx;
}

void render_setHoizontalFOV(float degree)
{
    half_hFOV = degree / 2;
    HALF_HEIGHT = context.HEIGHT / 2;
    HALF_WIDTH = context.WIDTH / 2;
    half_vFOV = atan((HALF_WIDTH * tan((3.14159 / 180) * (degree / 2))) / HALF_HEIGHT) / 2;
    half_vFOV = 180 * half_vFOV / 3.14159;
    printf("%f %f %f \n", half_hFOV, half_vFOV, tan((3.14159 / 180) * (degree / 2)));
}

void render_clear(Color color)
{
    for(int i = 0; i < context.WIDTH * context.HEIGHT * context.PITCH; i+=context.PITCH){
        context.buffer[i + context.RED_OFFSET  ] = color.R;
        context.buffer[i + context.GREEN_OFFSET] = color.G;
        context.buffer[i + context.BLUE_OFFSET ] = color.B;
        context.buffer[i + context.ALPHA_OFFSET] = color.A;
    }
}

void render_pixel(Point2D point, Color color)
{
    if(point.x < 0 || point.x > context.WIDTH - 1 || point.y < 0 || point.y > context.HEIGHT - 1){
        return;
    }
    int i = point.y * context.WIDTH + point.x;
    i *= context.PITCH;
    context.buffer[i + context.RED_OFFSET  ] = color.R;
    context.buffer[i + context.GREEN_OFFSET] = color.G;
    context.buffer[i + context.BLUE_OFFSET ] = color.B;
    context.buffer[i + context.ALPHA_OFFSET] = color.A;
}

void render_line(Point2D A, Point2D B, Color color)
{
    Point2D UPPER, LOWER;
    if(A.y < B.y)
        UPPER = A, LOWER = B;
    else
        UPPER = B, LOWER = A;

    int delY = LOWER.y - UPPER.y;
    int delX = LOWER.x - UPPER.x;
    int dx = delX > 0 ? 1 : -1;

    if(UPPER.x == LOWER.x){
        // Vertical line
        for(int i = UPPER.y; i <= LOWER.y; i++)
            render_pixel((Point2D){UPPER.x,i}, color);
        return;
    }

    if(UPPER.y == LOWER.x){
        //Horizontal line
        for(int i = UPPER.x; i <= LOWER.x; i+=dx)
            render_pixel((Point2D){i, UPPER.y}, color);
        return;
    }

    if(abs(delX) <= delY){
        float dx_dy = ( (float)delX ) / delY;
        for(int i = 0; i <= delY; i++){
            int X = UPPER.x + dx_dy * i;
            render_pixel((Point2D){X,i+UPPER.y}, color);
        }
    }
    else{
        float dy_dx = ( (float)delY ) / delX;
        for(int i = 0; abs(i) <= abs(delX); i+=dx){
            int Y = UPPER.y + dy_dx * i;
            render_pixel((Point2D){i+UPPER.x, Y}, color);
        }
    }
}

void render_triangleMesh_points(Point2D A, Point2D B, Point2D C, Color color)
{
    render_line(A, B, color);
    render_line(B, C, color);
    render_line(C, A, color);
}

void render_triangleMesh(Vertex A, Vertex B, Vertex C, Color color)
{

    Point2D A_screen, B_screen, C_screen;
    A_screen.x =   (A.pos.x * HALF_WIDTH)  / (A.pos.z * tan((3.14 / 180)*half_hFOV)) + HALF_WIDTH;
    A_screen.y = - (A.pos.y * HALF_HEIGHT) / (A.pos.z * tan((3.14 / 180)*half_vFOV)) + HALF_HEIGHT;
    B_screen.x =   (B.pos.x * HALF_WIDTH)  / (B.pos.z * tan((3.14 / 180)*half_hFOV)) + HALF_WIDTH;
    B_screen.y = - (B.pos.y * HALF_HEIGHT) / (B.pos.z * tan((3.14 / 180)*half_vFOV)) + HALF_HEIGHT;
    C_screen.x =   (C.pos.x * HALF_WIDTH)  / (C.pos.z * tan((3.14 / 180)*half_hFOV)) + HALF_WIDTH;
    C_screen.y = - (C.pos.y * HALF_HEIGHT) / (C.pos.z * tan((3.14 / 180)*half_vFOV)) + HALF_HEIGHT;

    render_triangleMesh_points(A_screen, B_screen, C_screen, color);
}