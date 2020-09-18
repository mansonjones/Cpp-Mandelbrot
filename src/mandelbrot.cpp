#include <iostream>
#include <fstream>
#include <complex>

using namespace std;

#include "mandelbrot.h"


Mandelbrot::Mandelbrot(float width, float height) : _width(width), _height(height) 
{
   _imageBuffer = new ImageBuffer<unsigned char>(_width, _height);
   for (int i = 0; i < _width; i++) {
      for (int j = 0; j < height; j++) {
          unsigned char foo = static_cast<unsigned char>(value(i,j)); 
          _imageBuffer->setRed( i, j, foo); 
          _imageBuffer->setGreen( i, j, foo); 
          _imageBuffer->setBlue( i, j, foo); 
      }
   }
}

Mandelbrot::~Mandelbrot()
{
   delete _imageBuffer;
} 

unsigned char *Mandelbrot::getBuffer() 
{
   // return _buffer;
   return _imageBuffer->getBuffer();
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

void Mandelbrot::write(std::string filename) 
{  
   ofstream my_Image (filename);
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
   else cout << " Could not open the file";



}
