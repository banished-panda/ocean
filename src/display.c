#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"

static int WIDTH;
static int HEIGHT;

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static uint8_t *pixels;
static int texture_pitch = 0;
static void *texture_pixels = NULL;

static uint32_t deltaTime = 0;
static uint32_t currentTime = 0;

static bool should_quit = false;

static void display_pollEvents();

void display_test()
{
    display_init(19, 11, 40);

    // ball properties
    int x = 1, y = 1;
    int xvel = 1, yvel = 1;
    int slowness = 2;
    int counter = 0;
    // bg properties
    int blue_xtra = 1, blue_change = 1;

    // measure FPS
    int frames = 0;
    uint32_t now = SDL_GetTicks();
    uint32_t nextReset = now + 1000;

    // main loop

    while (!display_closeRequested())
    {

        // update ball pos
        if (counter == slowness)
        {
            if (x == 0 || x == WIDTH - 1)
                xvel *= -1;
            if (y == 0 || y == HEIGHT - 1)
                yvel *= -1;
            x += xvel;
            y += yvel;
            counter = 0;
        }
        counter++;
        if (blue_xtra == 0 || blue_xtra == 100)
            blue_change *= -1;
        blue_xtra += blue_change;

        // render onto pixels
        // first: background
        for (int i = 0; i < WIDTH * HEIGHT; i++)
        {
            pixels[i * 4 + 0] = 40 + i;               // r
            pixels[i * 4 + 1] = 80 + (i % WIDTH) * 5; // g
            pixels[i * 4 + 2] = 150 + blue_xtra;      // b
        }
        // then: ball
        int i = y * WIDTH + x;
        pixels[4 * i + 0] = 0;
        pixels[4 * i + 1] = 0;
        pixels[4 * i + 2] = 0;

        display_update();

        now = SDL_GetTicks();
        if (now >= nextReset)
        {
            printf("[ %dms ] FPS: %d\n", now, frames);
            frames = 0;
            nextReset = now + 1000;
        }
        frames++;
    }

    display_cleanup();
}

void display_init(int width, int height, int scale)
{

    // init
    WIDTH = width;
    HEIGHT = height;
    pixels = (uint8_t *)malloc(width * height * 4);

    // SDL init
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    // create SDL window
    window = SDL_CreateWindow("ocean",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WIDTH * scale,
                              HEIGHT * scale,
                              SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return;
    }

    // create renderer
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        return;
    }

    if (SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT) < 0)
    {
        SDL_Log("SDL_RenderSetLogicalSize failed : %s", SDL_GetError());
    }

    // create texture
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT);
    if (texture == NULL)
    {
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        return;
    }
}

void display_update()
{
    display_pollEvents();

    // render on screen
    if (SDL_LockTexture(texture, NULL, &texture_pixels, &texture_pitch) != 0)
    {
        SDL_Log("Unable to lock texture: %s", SDL_GetError());
    }
    else
    {
        memcpy(texture_pixels, pixels, texture_pitch * HEIGHT);
    }
    SDL_UnlockTexture(texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void display_cleanup()
{
    free(pixels);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void display_pollEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            should_quit = true;
        }
    }
}

bool display_closeRequested()
{
    return should_quit;
}

uint32_t display_getDeltaTime()
{
    uint32_t prv_time = currentTime;
    currentTime = SDL_GetTicks();
    deltaTime = currentTime - prv_time;
    return deltaTime;
}

uint8_t *display_getPixelBuffer()
{
    return pixels;
}
