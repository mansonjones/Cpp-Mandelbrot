#include "SaveJob.h"


SaveJob::SaveJob()
{

}
void SaveJob::setImageBuffer(ImageBuffer<unsigned char> imageBuffer)
{
   _imageBuffer = imageBuffer;
}

void SaveJob::setFileName(std::string fileName)
{
    _fileName = fileName;
}

std::string SaveJob::getFileName()
{
    return _fileName;
}