#ifndef IMAGE_BUFFER_H_
#define IMAGE_BUFFER_H_

// Question: Is there some interesting way to use a functor
// for this class?
// Reminder: Be sure to add const to the getters

// TODO: Implement rule of five for this class

template <typename T>
class ImageBuffer 
{
public:
   ImageBuffer(int width = 400, int height = 400);
   ~ImageBuffer();

   T getRed(int i, int j);
   T getGreen(int i, int j);
   T getBlue(int i, int j);

   void setRed(int i, int j, T r); 
   void setGreen(int i, int j, T g); 
   void setBlue(int i, int j, T b);

   int getWidth();
   int getHeight();

   void setWidth(int w);
   void setHeight(int h);

   // Question.  What should the function thats reallocate the
   // buffer be called?  Is that function even necessary?

   T *getBuffer(); 
private:
  int getIndex(int i, int j);
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

template <typename T>
ImageBuffer<T>::~ImageBuffer()
{
  delete _bufferPtr;
}

template <typename T>
T ImageBuffer<T>::getRed(int i, int j)
{
   return _bufferPtr[getIndex(i,j)];
}

template <typename T>
T ImageBuffer<T>::getGreen(int i, int j)
{
   return _bufferPtr[getIndex(i,j) + 1];
}

template <typename T>
T ImageBuffer<T>::getBlue(int i, int j)
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
int ImageBuffer<T>::getWidth()
{
  return _width;
}

template <typename T>
int ImageBuffer<T>::getHeight()
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
T *ImageBuffer<T>::getBuffer() 
{
   return _bufferPtr;
}

template <typename T>
int ImageBuffer<T>::getIndex(int i, int j) 
{
    return 3*(i*_width + j);
}



#endif // IMAGE_BUFFER_H_