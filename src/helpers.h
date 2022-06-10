#define HELPERS_H

#include <iostream>
#include <cmath>

float map(float x, int in_min, int in_max, int out_min, int out_max) {
    const long run = in_max - in_min;
    if(run == 0){
        return -1; // AVR returns -1, SAM returns 0
    }
    const long rise = out_max - out_min;
    const float delta = x - in_min;
    return (delta * rise) / run + out_min;
}

int rgb(unsigned char r, unsigned char g, unsigned char b)
{   
    unsigned char red = r >> 3;
    unsigned char green = g >> 2;
    unsigned char blue = b >> 3;

    return (red << (5 + 6)) | (green << 5) | blue;
}

double findAngle(int x, int y, int cx, int cy, int radius){
    double angle = atan2(y - cy, x - cx) * 180 / M_PI;

    if (x <= cx && y >= cy) {
        return angle - 90;
    }

    else {
        return angle + 270;    
    }
}