#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include "ImageBuffer.h"
#include <string>

enum FileType 
{
  PPM
};

class ImageIO
{
public:
   ImageIO(const FileType fileType, const std::string fileName, ImageBuffer<unsigned char> *imageBuffer);
   // pure virtual function
   virtual void write() = 0;
   virtual ImageBuffer<unsigned char> *read() = 0;
   static ImageIO *getImageWriter(FileType fileType, std::string fileName, ImageBuffer<unsigned char> *imageBuffer);
protected:
   FileType _fileType;
   std::string _fileName;
   ImageBuffer<unsigned char> *_imageBuffer;
};


#endif
