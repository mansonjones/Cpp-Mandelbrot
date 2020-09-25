#include "AutoSave.h"
#include <chrono>

std::mutex AutoSave::_mutex;

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
       std::this_thread::sleep_for(std::chrono::milliseconds(1000));
       bool autoSave = _messageQueue->receive();
       
       if (autoSave == true) {
           std::unique_lock<std::mutex> lock(_mutex);
           std::cout << " Save Message Received " << std::endl;
           lock.unlock();
           // This job needs to be launched asychronouly
           std::async(&AutoSave::launchSaveJobOnThread, this);
          // launchSaveJobOnThread();
        }
    }
}


void AutoSave::launchSaveJobOnThread()
{
    // create promise and future
    std::promise<std::string> promise;
    std::future<std::string> future = promise.get_future();
    
    std::string fileName = "autosave.ppm";
    // start thread and pass promise as an argument
    _threads.emplace_back(std::thread(&AutoSave::saveFile, this, std::move(promise), fileName));
    std::string returnMessage = future.get();
}

void AutoSave::saveFile(std::promise<std::string> && promise, std::string fileName)
{
    std::ofstream outputFileStream(fileName);
    if (outputFileStream.is_open()) 
    {
        outputFileStream << " Hello There!" << std::endl;
    }
    promise.set_value("RETURN MESSAGE");
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

