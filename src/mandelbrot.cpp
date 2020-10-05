#include <iostream>
#include <fstream>
#include <complex>
#include <memory>

using namespace std;

#include "mandelbrot.h"


Mandelbrot::Mandelbrot(float width, float height) : _width(width), _height(height) 
{
   _imageBuffer = new ImageBuffer<unsigned char>(_width, _height);
   for (int i = 0; i < _width; i++) {
      for (int j = 0; j < _height; j++) {
          unsigned char mandelbrotValue = static_cast<unsigned char>(value(i,j)); 
          _imageBuffer->setRed( i, j, mandelbrotValue); 
          _imageBuffer->setGreen( i, j, mandelbrotValue); 
          _imageBuffer->setBlue( i, j, mandelbrotValue); 
      }
   }
}

void Mandelbrot::compute() 
{
   for (int i = 0; i < _width; i++) {
      for (int j = 0; j < _height; j++) {
         unsigned char foo = static_cast<unsigned char>(value(i,j)); 
         _imageBuffer2.setRed( i, j, foo); 
         _imageBuffer2.setGreen( i, j, foo); 
         _imageBuffer2.setBlue( i, j, foo); 
      }
   }
}

Mandelbrot::~Mandelbrot()
{
   delete _imageBuffer;
} 

unsigned char *Mandelbrot::getBuffer() 
{
   // return raw _buffer;
   return _imageBuffer->getBuffer();
}

ImageBuffer<unsigned char> Mandelbrot::getImageBuffer2() 
{
   /*
   ImageBuffer<unsigned char> imageBuffer = std::move(_imageBuffer2);
   return imageBuffer;
   */
  return _imageBuffer2;
}

ImageBuffer<unsigned char> *Mandelbrot::getImageBuffer() 
{
   return _imageBuffer;
}

int Mandelbrot::value(int x, int y) {
  complex<float> point((float)x/_width - 1.5, (float)y / _height - 0.5);
  // we divide by the image dimensions to get values smaller than 1
  // then apply a translation
  complex<float> z(0,0);
  unsigned int nb_iter = 0;
  while (abs(z) < 2 && nb_iter <= 34) {
    z = z * z + point;
    nb_iter++;
  }
  if (nb_iter < 34) return (255*nb_iter)/33;
  else return 0;
}

void Mandelbrot::render(ImageBuffer<unsigned char> &imageBuffer)
{
   for (int i = 0; i < imageBuffer.getWidth(); i++) {
      for (int j = 0; j < imageBuffer.getHeight(); j++) {
          unsigned char mandelbrotValue = static_cast<unsigned char>(value(i,j)); 
          imageBuffer.setRed( i, j, mandelbrotValue); 
          imageBuffer.setGreen( i, j, mandelbrotValue); 
          imageBuffer.setBlue( i, j, mandelbrotValue); 
      }
   }
}
// Eventually this should be update to use the
// ImageIO library
void Mandelbrot::write(FileType fileType, std::string fileName) 
{  
   ImageIO::getImageWriter(fileType, fileName, _imageBuffer)->write();
}
