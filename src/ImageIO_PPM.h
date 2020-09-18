#ifndef IMAGE_WRITER_H
#define IMAGE_WRITER_H

#include "ImageIO.h"

#include <string>

class ImageIO_PPM : public ImageIO
{
public:
   ImageIO_PPM(const FileType fileType, const std::string fileName, ImageBuffer<unsigned char> *imageBuffer);
   virtual void write();
};

#endif

