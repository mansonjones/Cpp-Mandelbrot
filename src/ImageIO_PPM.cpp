#include "ImageIO_PPM.h"

#include <string>
#include <iostream>
#include <fstream>

ImageIO_PPM::ImageIO_PPM(FileType fileType, std::string fileName, ImageBuffer<unsigned char> *imageBuffer)
 : ImageIO(fileType, fileName, imageBuffer)
{
}

void ImageIO_PPM::write()
{
   std::cout << " ImageWriterPPM::write() " << std::endl;
   std::ofstream my_Image (_fileName);

   int _width = _imageBuffer->getWidth();
   int _height = _imageBuffer->getHeight();

   if (my_Image.is_open()) {
      my_Image << "P3\n" << _width << " " << _height << " 255\n";
      for (int i = 0; i < _width; i++) {
         for (int j = 0; j < _height; j++) {
            // int val = value(i, j);
            int valRed = static_cast<int>(_imageBuffer->getRed(i,j));
            int valGreen = static_cast<int>(_imageBuffer->getGreen(i,j));
            int valBlue = static_cast<int>(_imageBuffer->getBlue(i,j));
            my_Image << valRed << ' ' << valGreen << ' ' << valBlue << "\n";
         }
      }
   }
   else std::cout << " Could not open the file";
}

ImageBuffer<unsigned char> *ImageIO_PPM::read() 
{
   _imageBuffer = new ImageBuffer<unsigned char>(100,100);
   std::cout << " ImageIO_PPM::read()" << std::endl;
   return _imageBuffer;
}