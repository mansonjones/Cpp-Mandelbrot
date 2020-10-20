#ifndef IMAGE_IO_H_
#define IMAGE_IO_H_

#include "ImageBuffer.h"
#include <string>

enum FileType 
{
  PPM
};

class ImageIO
{
public:
   ImageIO(const FileType fileType, const std::string fileName, ImageBuffer<PixelType> *imageBuffer);
   // pure virtual function
   virtual void write() = 0;
   virtual ImageBuffer<PixelType> *read() = 0;
   static ImageIO *getImageWriter(FileType fileType, std::string fileName, ImageBuffer<PixelType> *imageBuffer);
protected:
   FileType _fileType;
   std::string _fileName;
   ImageBuffer<PixelType> *_imageBuffer;
};


#endif
