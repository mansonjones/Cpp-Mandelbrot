#ifndef SAVE_JOB_H_
#define SAVE_JOB_H_

#include "ImageBuffer.h"
#include "ImageIO.h"

#include <string>


class SaveJob 
{
public:
    SaveJob();
    void setFileType(FileType fileType);
    void setImageBuffer(ImageBuffer<unsigned char> imageBuffer);
    void setFileName(std::string fileName);

    std::string getFileName();
    FileType getFileType();
    ImageBuffer<unsigned char> getImageBuffer();
    void write();
private:
    // Todo: Add image 
    ImageBuffer<unsigned char> _imageBuffer;
    std::string _fileName;
    FileType _fileType;
};

#endif // SAVE_JOB_H_