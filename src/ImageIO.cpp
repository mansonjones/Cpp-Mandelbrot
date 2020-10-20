
#include "ImageIO.h"
#include "ImageIO_PPM.h"

#include <iostream>

ImageIO::ImageIO(FileType fileType, std::string fileName, ImageBuffer<PixelType> *imageBuffer) :
  _fileType(fileType), _fileName(fileName), _imageBuffer(imageBuffer)
{
} 

ImageIO *ImageIO::getImageWriter(FileType fileType, std::string fileName, ImageBuffer<PixelType> *imageBuffer) 
{

   // Question: Would it be better if this threw an exception for the default case?
   switch (fileType) 
   {
      case PPM:
         return new ImageIO_PPM(fileType, fileName, imageBuffer);
         break;
      default:
         return new ImageIO_PPM(fileType, fileName, imageBuffer);
         break;
   }
   return new ImageIO_PPM(fileType, fileName, imageBuffer);
}
