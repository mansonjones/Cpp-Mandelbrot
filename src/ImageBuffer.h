#ifndef IMAGE_BUFFER_H_
#define IMAGE_BUFFER_H_

#include <iostream>

// Question: Is there some interesting way to use a functor
// for this class?
// Reminder: Be sure to add const to the getters
// The definition of RGB in this buffer is chosen to work easily
// with WxWidgets, especially the wxImage class.  
// TODO: Implement rule of five for this class

template <typename T>
class ImageBuffer 
{
public:
   // Note that rule of five is applied.
   ImageBuffer(int width = 400, int height = 400);

   // Copy Constructor
   ImageBuffer(const ImageBuffer &source);
   
   // Copy Assignment Operator
   ImageBuffer &operator = (const ImageBuffer& source);

   // Move Constructor
   ImageBuffer(ImageBuffer &&source);

   // Move Assignment Operator
   ImageBuffer &operator = (ImageBuffer &&source);
   // 
   ~ImageBuffer();

   T getRed(int i, int j) const;
   T getGreen(int i, int j) const;
   T getBlue(int i, int j) const;

   void setRed(int i, int j, T r); 
   void setGreen(int i, int j, T g); 
   void setBlue(int i, int j, T b);

   int getWidth() const;
   int getHeight() const;

   void setWidth(int w);
   void setHeight(int h);

   // Question.  What should the function thats reallocate the
   // buffer be called?  Is that function even necessary?

   T *getBuffer() const; 
private:
  int getIndex(int i, int j) const;
  T *_bufferPtr;  // What is the appropriate smart pointer?  unique_ptr?
  int _width;
  int _height;
};


template <typename T>
ImageBuffer<T>::ImageBuffer(int width, int height) : _width(width), _height(height) 
{
   _bufferPtr = new T[_width*_height*3];  // Assume 3 channels : r, g, b
   for (int i = 0; i < _width*_height*3; i++) {
      _bufferPtr[i] = i;
   }
}

// Copy Constructor
template <typename T>
ImageBuffer<T>::ImageBuffer(const ImageBuffer &source) :
   _width(source._width), _height(source._height)
{
   // TODO: There is a bug where the copy constructor is getting called repeatedly.
   // This might be the cause of some of the IO problems.
   // std::cout << " Copy Constructor" << std::endl;
   _bufferPtr = new T[_width*_height*3];  // Assume 3 channels : r, g, b
   for (int i = 0; i < _width*_height*3; i++) {
      _bufferPtr[i] = source._bufferPtr[i];
   }
}

// Copy Assignment Operator
template <typename T>
ImageBuffer<T> &ImageBuffer<T>::operator = (const ImageBuffer<T> &source)
{
   std::cout << "Copy Assignment Operator" << std::endl;
   _width = source._width;
   _height = source._height;

   _bufferPtr = new T[_width*_height*3];  // Assume 3 channels : r, g, b
   for (int i = 0; i < _width*_height*3; i++) {
      _bufferPtr[i] = source._bufferPtr[i];
   }

   return *this;
}

// Move Constructor
template <typename T>
ImageBuffer<T>::ImageBuffer(ImageBuffer &&source)
{
   std::cout << "Move Constructor " << std::endl;
   _width = source._width;
   _height = source._height;
   _bufferPtr = source._bufferPtr;
}
// Move Assignment Operator
template <typename T>
ImageBuffer<T> &ImageBuffer<T>::operator = (ImageBuffer &&source)
{
   std::cout << "Move Assignement Operator" << std::endl;
   _width = source._width;
   _height = source._height;
   _bufferPtr = source._bufferPtr;

   return *this;
}

// Destructor
template <typename T>
ImageBuffer<T>::~ImageBuffer()
{
  // delete _bufferPtr;
}

template <typename T>
T ImageBuffer<T>::getRed(int i, int j) const
{
   return _bufferPtr[getIndex(i,j)];
}

template <typename T>
T ImageBuffer<T>::getGreen(int i, int j) const
{
   return _bufferPtr[getIndex(i,j) + 1];
}

template <typename T>
T ImageBuffer<T>::getBlue(int i, int j) const
{
   return _bufferPtr[getIndex(i,j) + 2];
}

template <typename T>
void ImageBuffer<T>::setRed(int i, int j, T r) 
{
   _bufferPtr[getIndex(i,j)] = r;  
}

template <typename T>
void ImageBuffer<T>::setGreen(int i, int j, T g) 
{
   _bufferPtr[getIndex(i,j) + 1] = g;  
}

template <typename T>
void ImageBuffer<T>::setBlue(int i, int j, T b)
{
   _bufferPtr[getIndex(i,j) + 2] = b;  
}

template <typename T>
int ImageBuffer<T>::getWidth() const
{
  return _width;
}

template <typename T>
int ImageBuffer<T>::getHeight() const
{
  return _height;
}

template <typename T>
void ImageBuffer<T>::setWidth(int w)
{
   _width = w;
}

template <typename T>
void ImageBuffer<T>::setHeight(int h)
{
  _height = h;
}

   // Question.  What should the function the reallocate the
   // buffer be called?
template <typename T>
T *ImageBuffer<T>::getBuffer() const
{
   return _bufferPtr;
}

template <typename T>
int ImageBuffer<T>::getIndex(int i, int j) const
{
    return 3*(i*_width + j);
}



#endif // IMAGE_BUFFER_H_
