#ifndef SAVE_JOB_H_
#define SAVE_JOB_H_

#include "ImageBuffer.h"
#include "ImageIO.h"

#include <string>
#include <mutex>
#include <fstream>

// See https://stackoverflow.com/questions/2159713/overloading-output-operator-for-a-class-template-in-a-namespace

/*
class thread_safe_ofstream
{
public:
    std::mutex mu;
    std::ofstream stream;

    
    // template<typename T>
    void put(const T& thing) {
        std::lock_guard<std::mutex> lock(mu);
        stream << thing;
    }
    
    
    friend template<typename T> thread_safe_ofstream &operator<<(thread_safe_ofstream& tsof, const T& value) {
        tsof.put(value);
        return tsof;
    }


};
*/

class SaveJob 
{
public:
    SaveJob();
    SaveJob(FileType fileType, std::string fileName, ImageBuffer<PixelType> imageBuffer);
    ~SaveJob();

    // Copy Constructor
    SaveJob(const SaveJob& source);

    // Copy Assignment Operator
    SaveJob &operator = (const SaveJob &source);

    // Move Constructor
    SaveJob(SaveJob &&source);

    // Move Assignment Operator
    SaveJob &operator = (SaveJob &&source);

    void setFileType(FileType fileType);
    void setImageBuffer(ImageBuffer<PixelType> imageBuffer);
    void setFileName(std::string fileName);

    std::string getFileName() const;
    FileType getFileType() const;
    ImageBuffer<PixelType> getImageBuffer() const;
    void write();
    // Add Call operator
    void operator()();
private:
    // Todo: Add image 
    ImageBuffer<PixelType> _imageBuffer;
    std::string _fileName;
    FileType _fileType;
    static std::mutex _ioMutex; // mutex shared by all save jobs to protect outputstream
};

#endif // SAVE_JOB_H_