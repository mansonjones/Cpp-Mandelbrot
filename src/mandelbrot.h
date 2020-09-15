#ifndef MANDELBROT_H
#define MANDELBROT_H


#include <iostream>
#include <fstream>
#include <complex>

#include <string>

class Mandelbrot
{
public:
   Mandelbrot(float width, float height);
   void write(std::string fileName);   // this should be improved using the ostream operator
   unsigned char *getBuffer();
private:
   int value(int x, int y); 
   float _width;
   float _height;
   unsigned char *_buffer;
};

#endif  /* MANDELBROT_H */
  
