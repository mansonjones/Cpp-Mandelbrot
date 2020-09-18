#ifndef IMAGE_WRITE_H
#define IMAGE_WRITE_H

#include "ImageBuffer.h"
#include <string>

enum FileType 
{
  PPM
};

class ImageWriter
{
public:
   ImageWriter(const FileType fileType, const std::string fileName, ImageBuffer<unsigned char> *imageBuffer);
   // pure virtual function
   virtual void write() = 0;
   static ImageWriter *getImageWriter(FileType fileType, std::string fileName, ImageBuffer<unsigned char> *imageBuffer);
protected:
   FileType _fileType;
   std::string _fileName;
   ImageBuffer<unsigned char> *_imageBuffer;
};


#endif
