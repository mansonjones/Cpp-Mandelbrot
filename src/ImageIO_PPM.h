#ifndef IMAGE_IO_PPM_H_
#define IMAGE_IO_PPM_H_

#include "ImageIO.h"

#include <future>
#include <string>


class ImageIO_PPM : public ImageIO
{
public:
   ImageIO_PPM(const FileType fileType, const std::string fileName, ImageBuffer<PixelType> *imageBuffer);
   virtual void write();
   virtual ImageBuffer<PixelType> *read();
   static void writeImage(std::promise<std::string> && promise, std::string fileName, ImageBuffer<PixelType> imageBuffer);
   static void writeTest(std::string fileName, ImageBuffer<PixelType> imageBuffer);
};

#endif

