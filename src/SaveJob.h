#ifndef SAVE_JOB_H_
#define SAVE_JOB_H_

#include "ImageBuffer.h"
#include "ImageIO.h"

#include <string>
#include <mutex>

class SaveJob 
{
public:
    SaveJob();
    void setFileType(FileType fileType);
    void setImageBuffer(ImageBuffer<unsigned char> imageBuffer);
    void setFileName(std::string fileName);

    std::string getFileName() const;
    FileType getFileType() const;
    ImageBuffer<unsigned char> getImageBuffer() const;
    void write();
private:
    // Todo: Add image 
    ImageBuffer<unsigned char> _imageBuffer;
    std::string _fileName;
    FileType _fileType;
    static std::mutex _ioMutex; // mutex shared by all save jobs to protect outputstream
};

#endif // SAVE_JOB_H_