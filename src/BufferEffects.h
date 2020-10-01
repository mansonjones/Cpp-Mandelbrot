
#ifndef BUFFER_EFFECTS_H_
#define BUFFER_EFFECTS_H_

#include "ImageBuffer.h"

enum Color {
    red,
    green,
    blue,
    cyan,
    magenta,
    yellow
};

class BufferEffects 
{
public:
    static void setColor(Color color, ImageBuffer<unsigned char> &imageBuffer);
    static void setRGB(unsigned char r, unsigned char g, unsigned char b, ImageBuffer<unsigned char> &imageBuffer);
};

#endif // _BUFFER_EFFECTS_H_