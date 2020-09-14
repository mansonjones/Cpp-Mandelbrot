#include <iostream>
#include <fstream>
#include <complex>

using namespace std;

#include "mandelbrot.h"


Mandelbrot::Mandelbrot(float width, float height) : _width(width), _height(height) {}

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
            int val = value(i, j);
            my_Image << val << ' ' << 0 << ' ' << 0 << "\n";
         }
      }
   }
   else cout << " Could not open the file";



}
