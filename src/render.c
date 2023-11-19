#include "render.h"

static RenderContext context;

void render_setContext(RenderContext ctx)
{
    context = ctx;
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
