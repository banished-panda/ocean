#pragma once
#include <stdint.h>
#include <stdbool.h>

/*
    quick way to test if things compile and run
    call display_test() without calling any other
    display functions

    example:
        int main(int argc, char *argv[]){
            display_test();
            return 0;
        }
*/
void display_test();

/*
    initializes a display
    logical size:
        width x height
    on screen size:
        (width * scale) x (height * scale)
*/
void display_init(int width, int height, int scale);

/*
    returns a buffer
    pitch: 4 bytes
    order: R, G, B, A
*/
uint8_t * display_getPixelBuffer();

/*
    returns time in miliseconds since last time
    this function was called
*/
uint32_t display_getDeltaTime();

/*
    draws the pixelBuffer onto the screen
*/
void display_update();

/*
    frees up resources used by display
*/
void display_cleanup();

/*
    returns whether user requested the display to be closed
*/
bool display_closeRequested();