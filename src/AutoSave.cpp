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

AutoSave::AutoSave(Mandelbrot *mandelbrotPointer) : _mandelbrotPointer(mandelbrotPointer)
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
           _mandelbrotPointer->sayHello();
           // lock.unlock();
           std::promise<std::string> promise;
           std::future<std::string> future = promise.get_future();

           // start thread and pass promise as argument
           SaveJob saveJob;
           saveJob.setFileName(fileName);
           ImageBuffer<unsigned char> temp = _mandelbrotPointer->getImageBuffer2();
            ImageBuffer<unsigned char> imageBuffer(temp.getWidth(), temp.getHeight());
            std::cout << " (width, height) = " << temp.getWidth() << " " << temp.getHeight() << std::endl;
           for (int i = 0; i < temp.getWidth(); i++) {
               for (int j = 0; j < temp.getHeight(); j++) {
                   imageBuffer.setRed(i,j,255);
                   imageBuffer.setGreen(i,j,temp.getGreen(i,j));
                   imageBuffer.setBlue(i,j,temp.getBlue(i,j));
               }
           }
           saveJob.setImageBuffer(/* temp  */ imageBuffer);
           _threads.emplace_back(std::thread(&AutoSave::saveFile, this, std::move(promise), saveJob));
           // set the wait
            std::for_each(_threads.begin(), _threads.end(), [](std::thread &t) {
                t.join();
            });


           lock.unlock();
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
    // TODO: use the ImageIO library to write this out.
    // ImageIO_PPM::writeTest(saveJob.getFileName(), saveJob.getImageBuffer());
    saveJob.write();

}

void AutoSave::sendMessageAtInterval()
{
   unsigned int interval = 10000; // milliseconds

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

