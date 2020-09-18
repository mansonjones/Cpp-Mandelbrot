#ifndef IMAGE_IO_PPM_H
#define IMAGE_IO_PPM_H

#include "ImageIO.h"

#include <string>

class ImageIO_PPM : public ImageIO
{
public:
   ImageIO_PPM(const FileType fileType, const std::string fileName, ImageBuffer<unsigned char> *imageBuffer);
   virtual void write();
   virtual ImageBuffer<unsigned char> *read();
};

#endif

