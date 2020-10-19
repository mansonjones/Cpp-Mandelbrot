#include "SaveJob.h"

#include <fstream>
#include <sstream>
#include <string>

std::mutex SaveJob::_ioMutex;

SaveJob::SaveJob()
{}

SaveJob::SaveJob(FileType fileType, std::string fileName, ImageBuffer<unsigned char> imageBuffer) :
_fileType(fileType), _fileName(fileName), _imageBuffer(imageBuffer)
{}

SaveJob::~SaveJob()
{}

// Copy Constructor
SaveJob::SaveJob(const SaveJob& source) :
    _fileType(source._fileType),
    _fileName(source._fileName),
    _imageBuffer(source._imageBuffer)
{}

// Copy Assignment Operator
SaveJob &SaveJob::operator = (const SaveJob &source)
{
    if (this == &source) {
        return *this;
    }
    _fileType = source._fileType;
    _fileName = source._fileName;
    _imageBuffer = source._imageBuffer;

    return *this;
}

// Move Constructor
SaveJob::SaveJob(SaveJob &&source) :
    _fileType(source._fileType),
    _fileName(source._fileName)
{
    std::cout << " SaveJob::Move Constructor " << std::endl;
    _imageBuffer = std::move(source._imageBuffer);
}

// Move Assignment Operator
SaveJob &SaveJob::operator = (SaveJob &&source)
{
    std::cout << " SaveJob::Move Assignment Operator " << std::endl;
    if (this == &source) {
        return *this;
    }
    _fileType = source._fileType;
    _fileName = source._fileName;
    _imageBuffer = std::move(source._imageBuffer);

    return *this;
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
std::string SaveJob::getFileName() const
{
    return _fileName;
}

FileType SaveJob::getFileType() const
{
    return _fileType;
}

ImageBuffer<unsigned char> SaveJob::getImageBuffer() const
{
    return _imageBuffer;
}

void SaveJob::write()
{
    // TODO: Use the ImageIO code to write this out.
    std::unique_lock<std::mutex> ioLock(_ioMutex);
    std::ofstream outputFileStream(_fileName);
    if (outputFileStream.is_open()) 
    {
    
        int _width = _imageBuffer.getWidth();
        int _height = _imageBuffer.getHeight();

        if (outputFileStream.is_open()) {
            outputFileStream << "P3\n" << _width << " " << _height << " 255\n";
                for (int i = 0; i < _width; ++i) {
                    for (int j = 0; j < _height; ++j) {
                        int valRed = _imageBuffer.getRed(i,j);
                        int valGreen = _imageBuffer.getGreen(i,j);
                        int valBlue = _imageBuffer.getBlue(i,j);
                        // TODO: See if there is any performance difference between these, 
                        std::stringstream stringStream;
                        stringStream << valRed << " " << valGreen << " " << valBlue << "\n";
                        outputFileStream << stringStream.str();
                        // outputFileStream << 0 << " " << 255 << " " << 0 << "\n";
                        // outputFileStream.flush();
                    }
                }
            outputFileStream.close();
        }
    // ioLock.unlock();
        // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    } else {
        std::cout << " unable to open file " << std::endl;
    }
}

// Function object
void SaveJob::operator()()
{
    write();
}