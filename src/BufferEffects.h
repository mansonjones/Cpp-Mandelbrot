
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
    static void setColor(Color color, ImageBuffer<PixelType> &imageBuffer);
    static void setRGB(PixelType r, PixelType g, PixelType b, ImageBuffer<PixelType> &imageBuffer);
};

#endif // _BUFFER_EFFECTS_H_