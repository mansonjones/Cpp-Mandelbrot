
#include "ImageIO.h"
#include "ImageWriterPPM.h"

#include <iostream>

ImageWriter::ImageWriter(FileType fileType, std::string fileName, ImageBuffer<unsigned char> *imageBuffer) :
  _fileType(fileType), _fileName(fileName), _imageBuffer(imageBuffer)
{
} 

ImageWriter *ImageWriter::getImageWriter(FileType fileType, std::string fileName, ImageBuffer<unsigned char> *imageBuffer) 
{
   // Question: Would it be better if this threw an exception for the default case?
   switch (fileType) 
   {
      case PPM:
         return new ImageWriterPPM(fileType, fileName, imageBuffer);
         break;
      default:
         return new ImageWriterPPM(fileType, fileName, imageBuffer);
         break;
   }
   return new ImageWriterPPM(fileType, fileName, imageBuffer);
}
