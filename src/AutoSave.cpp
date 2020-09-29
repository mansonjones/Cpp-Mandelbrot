#include "AutoSave.h"
#include "ImageIO_PPM.h"

#include <chrono>

std::mutex AutoSave::_mutex;
unsigned long AutoSave::_counter = 0;

void ImageWriter::write(std::promise<void> &&promise, std::string fileName)
{
   std::string fileNameTemp = "output1.txt";
   std::ofstream outputFileStream(fileNameTemp);
   if (outputFileStream.is_open()) {
      outputFileStream << "This is the first line" << std::endl;
   }
   outputFileStream.close();
}

void ImageWriter::write1()
{
   std::string fileName = "output1.txt";
   std::ofstream outputFileStream(fileName);
   if (outputFileStream.is_open()) {
      outputFileStream << "This is the first line" << std::endl;
   }
   outputFileStream.close();
}
AutoSave::AutoSave()
{
  // std::cout << " AutoSave Constructor " << std::endl;
  _messageQueue = std::make_shared<MessageQueue<bool>>();
}

AutoSave::~AutoSave()
{
  // set up thread barrier before this object is destroyed
  std::for_each(_threads.begin(), _threads.end(), [](std::thread &t) {
     t.join();
  });
}

void AutoSave::runTimerOnThread()
{
   _threads.emplace_back(std::thread(&AutoSave::sendMessageAtInterval, this));
}

void AutoSave::runMonitorOnThread()
{
    _threads.emplace_back(std::thread(&AutoSave::waitForAutoSaveMessage, this));
}

void AutoSave::waitForAutoSaveMessage()
{
   while (true)
   {
       std::this_thread::sleep_for(std::chrono::milliseconds(1));
       bool autoSave = _messageQueue->receive();
       
       if (autoSave == true) {
           std::unique_lock<std::mutex> lock(_mutex);
           _counter++;
           std::string fileName = std::string("autosave_" + std::to_string(_counter) + std::string(".ppm"));
           std::cout << " Save Message Received " << " " << _counter << " " << fileName << std::endl;
           lock.unlock();
           std::promise<std::string> promise;
           std::future<std::string> future = promise.get_future();

           // start thread and pass promise as argument
           SaveJob saveJob;
           saveJob.setFileName(fileName);
           ImageBuffer<unsigned char> imageBuffer;
           saveJob.setImageBuffer(imageBuffer);
           std::string messageToThread = "My Message";
           // std::thread t(&AutoSave::saveFile, this, std::move(promise), saveJob);
           _threads.emplace_back(std::thread(&AutoSave::saveFile, this, std::move(promise), saveJob));
           // Thread barrier
           // t.join();
           // This job needs to be launched asychronouly
           // std::async(&AutoSave::launchSaveJobOnThread, this, fileName);
          // launchSaveJobOnThread();
        }
    }
}

void AutoSave::launchSaveJobOnThread(std::string fileName)
{
    // create promise and future
    std::promise<std::string> promise;
    std::future<std::string> future = promise.get_future();
    
    // start thread and pass promise as an argument
    SaveJob saveJob;
    saveJob.setFileName(fileName);
    ImageBuffer<unsigned char> imageBuffer;
    saveJob.setImageBuffer(imageBuffer);
    // saveJob.write();
    _threads.emplace_back(std::thread(&AutoSave::saveFile, this, std::move(promise), saveJob));
    // _threads.emplace_back(std::thread(&SaveJob::saveFile, this, std::move(promise)));
    std::string returnMessage = future.get();
}

void AutoSave::saveFile(std::promise<std::string> && promise, SaveJob saveJob)
{
    // ImageIO_PPM::writeTest(saveJob.getFileName(), saveJob.getImageBuffer());
    
    std::ofstream outputFileStream(saveJob.getFileName());
    if (outputFileStream.is_open()) 
    { 
        // ImageIO_PPM::writeTest(saveJob.getFileName(), saveJob.getImageBuffer());

       // outputFileStream << " Hello There ABC!" << std::endl;

        // TODO : Need to add ImageBuffer
        
        int _width = saveJob.getImageBuffer().getWidth();
        int _height = saveJob.getImageBuffer().getHeight();

        if (outputFileStream.is_open()) {
            outputFileStream << "P3\n" << _width << " " << _height << " 255\n";
                for (int i = 0; i < _width; i++) {
                    for (int j = 0; j < _height; j++) {
                        // int val = value(i, j);
                        int valRed = static_cast<int>(saveJob.getImageBuffer().getRed(i,j));
                        int valGreen = static_cast<int>(saveJob.getImageBuffer().getGreen(i,j));
                        int valBlue = static_cast<int>(saveJob.getImageBuffer().getBlue(i,j));
                        outputFileStream << valRed << ' ' << valGreen << ' ' << valBlue << "\n";
                    }
                }
        }
        
        promise.set_value("Succeeded in writing for auto save");
    } else {
        promise.set_value("Error Opening file for auto save");
    }
    
}

void AutoSave::sendMessageAtInterval()
{
   unsigned int interval = 5000; // milliseconds

   while (true)
   {
       std::this_thread::sleep_for(std::chrono::milliseconds(interval));
       // std::unique_lock<std::mutex> lock(_mutex);
        std::cout << " Auto Save ... " << std::endl;
        // lock.unlock();
        bool autoSave = true;
        _messageQueue->send(std::move(autoSave));
        // std::cout << " Message Queue Size " << _messageQueue->size() << std::endl;
         // I could write the output here
         // Question: should mutex be used for console messages?
   }
}

