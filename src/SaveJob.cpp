#include "SaveJob.h"

#include <fstream>

SaveJob::SaveJob()
{

}
void SaveJob::setImageBuffer(ImageBuffer<unsigned char> imageBuffer)
{
    _fileType = PPM;
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

FileType SaveJob::getFileType()
{
    return _fileType;
}

ImageBuffer<unsigned char> SaveJob::getImageBuffer()
{
    return _imageBuffer;
}

void SaveJob::write()
{
       std::ofstream outputFileStream(_fileName);;
    if (outputFileStream.is_open()) 
    {
        
        outputFileStream << " Hello There from SaveJob !" << std::endl;
    }


}