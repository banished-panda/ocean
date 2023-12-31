#pragma once
#include <stdint.h>

typedef struct
{
    int WIDTH;
    int HEIGHT;

    uint8_t * buffer;

    int RED_OFFSET;
    int GREEN_OFFSET;
    int BLUE_OFFSET;
    int ALPHA_OFFSET;

    int PITCH;
} RenderContext;

typedef struct{
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
} Color;

// A point in screen space
typedef struct
{
    int x;
    int y;
} Point2D;

typedef struct
{
    float x;
    float y;
    float z;  
} Vec3;

typedef struct
{
    Vec3 pos;
} Vertex;

void render_setContext(RenderContext context);

void render_setHoizontalFOV(float degree);

void render_clear(Color color);

void render_pixel(Point2D point, Color color);

void render_line(Point2D A, Point2D B, Color color);

void render_triangleMesh_points(Point2D A, Point2D B, Point2D C, Color color);
void render_triangleMesh(Vertex A, Vertex B, Vertex C, Color color);
