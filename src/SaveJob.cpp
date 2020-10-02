#include "SaveJob.h"

#include <fstream>
#include <sstream>
#include <string>

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

void SaveJob::setFileType(FileType fileType) 
{
    _fileType = fileType;
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
    // TODO: Use the ImageIO code to write this out.
    std::ofstream outputFileStream(_fileName);;
    if (outputFileStream.is_open()) 
    {
        int _width = getImageBuffer().getWidth();
        int _height = getImageBuffer().getHeight();

        if (outputFileStream.is_open()) {
            outputFileStream << "P3\n" << _width << " " << _height << " 255\n";
                for (int i = 0; i < _width; ++i) {
                    for (int j = 0; j < _height; ++j) {
                        // int val = value(i, j);
                        // int valRed = static_cast<int>(getImageBuffer().getRed(i,j));
                        // int valGreen = static_cast<int>(getImageBuffer().getGreen(i,j));
                        // int valBlue = static_cast<int>(getImageBuffer().getBlue(i,j));
                        int valRed = getImageBuffer().getRed(i,j);
                        int valGreen = getImageBuffer().getGreen(i,j);
                        int valBlue = getImageBuffer().getBlue(i,j);
                        std::stringstream stringStream;
                        stringStream << std::to_string(valRed) << " " << std::to_string(valGreen) << " " << std::to_string(valBlue) << "\n";
                        outputFileStream << stringStream.str();
                        // outputFileStream << valRed << " " << valGreen << " " << valBlue << "\n";
                        outputFileStream.flush();
                    }
                }
        }
        outputFileStream.close();
        // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    } else {
        std::cout << " unable to open file " << std::endl;
    }
}