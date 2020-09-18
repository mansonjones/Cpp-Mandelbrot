#ifndef IMAGE_WRITER_H
#define IMAGE_WRITER_H

#include "ImageIO.h"

#include <string>

class ImageWriterPPM : public ImageWriter
{
public:
   ImageWriterPPM(FileType fileType, std::string fileName, ImageBuffer<unsigned char> *imageBuffer);
   virtual void write();
};

#endif

