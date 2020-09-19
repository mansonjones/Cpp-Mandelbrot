#include "ImageIO_PPM.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

ImageIO_PPM::ImageIO_PPM(FileType fileType, std::string fileName, ImageBuffer<unsigned char> *imageBuffer)
 : ImageIO(fileType, fileName, imageBuffer)
{
}

void ImageIO_PPM::write()
{
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
   std::ifstream fileStream(_fileName);
   std::string lineBuffer;

   if (fileStream.is_open())
   {
      // read first line
      std::getline(fileStream, lineBuffer);
      if (lineBuffer != "P3") 
      {
         std::cout << " error! " << std::endl; 
      }

      // read second line
      std::getline(fileStream,lineBuffer);
      std::istringstream lineStream(lineBuffer);
      int width, height, bitDepth;
      lineStream >> width >> height >> bitDepth;
      _imageBuffer = new ImageBuffer<unsigned char>(width, height);
      unsigned char red, green, blue;
      for (int i = 0; i < width; i++) 
      {
         for (int j = 0; j < height; j++) {
            std::getline(fileStream,lineBuffer);
            // std::istringstream lineStream2(lineBuffer);
            lineStream = std::istringstream(lineBuffer);
            lineStream >> red >> green >> blue;
            _imageBuffer->setRed(i,j,red);
            _imageBuffer->setGreen(i,j,green);
            _imageBuffer->setBlue(i,j,blue);
         }
      }
      _imageBuffer = new ImageBuffer<unsigned char>(100,100);
   }
   // read the file
   return _imageBuffer;
}