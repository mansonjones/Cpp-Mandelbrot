#ifndef MANDELBROT_H
#define MANDELBROT_H


#include <iostream>
#include <fstream>
#include <complex>

#include <string>

#include "ImageBuffer.h"

class Mandelbrot
{
public:
   Mandelbrot(float width, float height);
   ~Mandelbrot();
   void write(std::string fileName);   // this should be improved using the ostream operator
   unsigned char *getBuffer();
   ImageBuffer<unsigned char> *getImageBuffer();
private:
   int value(int x, int y); 
   float _width;
   float _height;
   ImageBuffer<unsigned char> *_imageBuffer;
   
};

#endif  /* MANDELBROT_H */
  
