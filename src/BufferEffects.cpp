

#include "BufferEffects.h"
#include <iostream>


void BufferEffects::setRGB(unsigned char r, unsigned char g, unsigned char b, ImageBuffer<unsigned char> &imageBuffer)
{
    for (int i = 0; i < imageBuffer.getWidth(); i++) {
        for (int j = 0; j < imageBuffer.getHeight(); j++) {
            imageBuffer.setRed(i,j, r);
            imageBuffer.setGreen(i,j, g);
            imageBuffer.setBlue(i,j, b);
        }
    }
}
void BufferEffects::setColor(Color color, ImageBuffer<unsigned char> &imageBuffer)
{
    switch (color) {
        case red: 
            {
                unsigned char redValue = static_cast<unsigned char>(255);
                unsigned char greenValue = static_cast<unsigned char>(0);
                unsigned char blueValue = static_cast<unsigned char>(0);
                BufferEffects::setRGB(redValue, greenValue, blueValue, imageBuffer);
            }
            break;
        case green:
                    {
                unsigned char redValue = static_cast<unsigned char>(0);
                unsigned char greenValue = static_cast<unsigned char>(255);
                unsigned char blueValue = static_cast<unsigned char>(0);
                BufferEffects::setRGB(redValue, greenValue, blueValue, imageBuffer);
            }
            break;
        case blue:
                    {
                unsigned char redValue = static_cast<unsigned char>(0);
                unsigned char greenValue = static_cast<unsigned char>(0);
                unsigned char blueValue = static_cast<unsigned char>(255);
                BufferEffects::setRGB(redValue, greenValue, blueValue, imageBuffer);
            }
            break;
        case cyan:
                    {
                unsigned char redValue = static_cast<unsigned char>(255);
                unsigned char greenValue = static_cast<unsigned char>(0);
                unsigned char blueValue = static_cast<unsigned char>(255);
                BufferEffects::setRGB(redValue, greenValue, blueValue, imageBuffer);
            }
            break;
        case magenta:
                    {
                unsigned char redValue = static_cast<unsigned char>(255);
                unsigned char greenValue = static_cast<unsigned char>(0);
                unsigned char blueValue = static_cast<unsigned char>(255);
                BufferEffects::setRGB(redValue, greenValue, blueValue, imageBuffer);
            }
            break;
        case yellow:
                    {
                unsigned char redValue = static_cast<unsigned char>(255);
                unsigned char greenValue = static_cast<unsigned char>(255);
                unsigned char blueValue = static_cast<unsigned char>(0);
                BufferEffects::setRGB(redValue, greenValue, blueValue, imageBuffer);
            }
            break;
        default:
            std::cout << "Invalid Color" << std::endl;
    }
}