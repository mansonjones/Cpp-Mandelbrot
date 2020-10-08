#ifndef MANDELBROT_H_
#define MANDELBROT_H_


#include <iostream>
#include <fstream>
#include <complex>

#include <string>

#include "ImageBuffer.h"
#include "ImageIO.h"

class Mandelbrot
{
public:
   Mandelbrot();
   Mandelbrot(float width, float height);
   ~Mandelbrot();
   void write(FileType fileType, std::string fileName);   // this should be improved using the ostream operator
   unsigned char *getBuffer();
   ImageBuffer<unsigned char> *getImageBuffer();
   
   void render(ImageBuffer<unsigned char> &imageBuffer);  // This could be turned into a static function

   void sayHello() { std::cout << " Mandelbrot Say Hello " << std::endl; }
   // This function should eventually be moved into the IO class;
   
   void setOffsets(float xOffset, float yOffset);
   void setScale(float scale);
private:
   int value(int x, int y); 
   float _width;
   float _height;
   float _xOffset;
   float _yOffset;
   float _scale;
   ImageBuffer<unsigned char> *_imageBuffer;
   
};

#endif  /* MANDELBROT_H_*/
  
