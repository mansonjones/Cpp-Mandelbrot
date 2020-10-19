#include "AutoSave.h"
#include "BufferEffects.h"
#include "ImageIO_PPM.h"
#include "mandelbrot_gui.h"

#include <chrono>

std::mutex AutoSave::_mutex;

unsigned long AutoSave::_counter = 0;

AutoSave::AutoSave()
{
  _messageQueue = std::make_shared<MessageQueue<int>>();
}

AutoSave::AutoSave(MandelbrotPanel *mandelbrotPanel) : _mandelbrotPanel(mandelbrotPanel)
{
  _messageQueue = std::make_shared<MessageQueue<int>>();
}

AutoSave::~AutoSave()
{
  // set up thread barrier before this object is destroyed
  std::for_each(_threads.begin(), _threads.end(), [](std::thread &t) {
     t.join();
  });
}

void AutoSave::emitAutoSaveMessagesOnThread() 
{
    _threads.emplace_back(std::thread(&AutoSave::emitAutoSaveMessages, this));
}

void AutoSave::emitAutoSaveMessages()
{
    // Send save messages at a regular interval
    unsigned int interval = 10000; // milliseconds

    unsigned int counter = 0;
    // TODO : Cyle the file number 
    while (true)
    {
       std::this_thread::sleep_for(std::chrono::milliseconds(interval));
       // std::unique_lock<std::mutex> lock(_mutex);
        std::cout << " Auto Save ... " << std::endl;
        // lock.unlock();
        // bool autoSave = true;
        // FileType fileType = PPM;
        // std::string fileName = "autosave_" + std::to_string(counter) + ".ppm";
        // ImageBuffer<unsigned char> imageBuffer(400,400);
        // BufferEffects::setColor(blue, imageBuffer);

        // SaveJob saveJob(fileType, fileName, imageBuffer);
        // saveJob.write();
        _messageQueue->send(std::move(counter));
        counter++;
        // std::cout << " Message Queue Size " << _messageQueue->size() << std::endl;
         // I could write the output here
         // Question: should mutex be used for console messages?
    }

}

void AutoSave::waitForAutoSaveMessagesOnThread()
{
    _threads.emplace_back(std::thread(&AutoSave::waitForAutoSaveMessages, this));
}

void AutoSave::waitForAutoSaveMessages()
{
   while (true)
   {
       std::this_thread::sleep_for(std::chrono::milliseconds(1));
       int autoSave = _messageQueue->receive();
       
       if (autoSave >= 0) {
           std::unique_lock<std::mutex> myLock(_mutex);
           _counter++;
           // _saveState = true;
           std::cout << "  ************ message received " << autoSave << std::endl;
           std::string fileName = std::string("autosave_" + std::to_string(autoSave) + std::string(".ppm"));
           std::cout << " file name " << fileName << std::endl;
           std::shared_ptr<SaveJob> saveJob(new SaveJob);
           saveJob->setFileType(PPM);
           saveJob->setFileName(fileName);
           // ImageBuffer<unsigned char> imageBuffer(400,400);
           // BufferEffects::setColor(blue, imageBuffer);
           ImageBuffer imageBuffer = _mandelbrotPanel->getImageBuffer();
           saveJob->setImageBuffer(imageBuffer);

           std::promise<std::string> promise;
           std::future<std::string> future = promise.get_future();
           std::thread thread1(&AutoSave::writeFile, this, std::move(promise), saveJob);
           std::string result = future.get();
        // Thread barrier
            thread1.join();
           
        }
    }
}

int AutoSave::getCounter()
{
    return _counter;
}


void AutoSave::writeFile(std::promise<std::string> &&promise, std::shared_ptr<SaveJob> saveJob)
{
    saveJob->write();
    promise.set_value(saveJob->getFileName());
}