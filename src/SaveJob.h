#ifndef SAVE_JOB_H_
#define SAVE_JOB_H_

#include "ImageBuffer.h"
#include <string>


class SaveJob 
{
    SaveJob();
    void setImageBuffer(ImageBuffer<unsigned char> imageBuffer);
    void setFileName(std::string fileName);

private:
    // Todo: Add image 
    ImageBuffer<unsigned char> _imageBuffer;
    std::string _fileName;
};

#endif // SAVE_JOB_H_