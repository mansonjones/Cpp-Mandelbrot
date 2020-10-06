#include "AutoSave.h"
#include "BufferEffects.h"
#include "ImageIO_PPM.h"
#include "mandelbrot_gui.h"

#include <chrono>

std::mutex AutoSave::_mutex
;
unsigned long AutoSave::_counter = 0;

AutoSave::AutoSave()
{
  // std::cout << " AutoSave Constructor " << std::endl;
  _messageQueue = std::make_shared<MessageQueue<int>>();
}

AutoSave::AutoSave(MandelbrotPanel *mandelbrotPanel) : _mandelbrotPanel(mandelbrotPanel)
{
  // std::cout << " AutoSave Constructor " << std::endl;
  _messageQueue = std::make_shared<MessageQueue<int>>();
}

AutoSave::~AutoSave()
{
  // set up thread barrier before this object is destroyed
  std::for_each(_threads.begin(), _threads.end(), [](std::thread &t) {
     t.join();
  });
  std::for_each(_jobThreads.begin(), _jobThreads.end(), [](std::thread &t) {
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
       int autoSave = _messageQueue->receive();
       
       if (autoSave >= 0) {
           // std::unique_lock<std::mutex> mylock(_mutex);
           _counter++;
           // std::unique_lock<std::mutex> myLock(_mutex);
           std::lock_guard<std::mutex> lock(_mutex);
           std::string fileName = std::string("autosave_" + std::to_string(autoSave) + std::string(".ppm"));
           
           std::cout << " Save Message Received " << " " << _counter << " " << fileName << std::endl;
           std::cout << "auto save number " << autoSave << std::endl;
           // myLock.unlock();
           // _mandelbrotPointer->sayHello();
           // lock.unlock();
           // std::lock_guard<std::mutex> lock(_mutex);
           // std::promise<std::string> promise;
           // std::future<std::string> future = promise.get_future();

           // start thread and pass promise as argument
           // Need to get the latest buffer from
           std::shared_ptr<SaveJob> saveJob(new SaveJob);
           
           saveJob->setFileName(fileName);
           saveJob->setFileType(PPM);
           ImageBuffer<unsigned char> imageBuffer = _mandelbrotPanel->getImageBuffer();
           // ImageBuffer<unsigned char> imageBuffer(400,400);
           // BufferEffects::setColor(red, imageBuffer);
           // ImageBuffer<unsigned char> imageBuffer(100,100);
           // BufferEffects::setColor(blue, imageBuffer);
           saveJob->setImageBuffer(imageBuffer);
           // ImageBuffer<unsigned char> temp = _mandelbrotPointer->getImageBuffer2();
            // ImageBuffer<unsigned char> imageBuffer(temp.getWidth(), temp.getHeight());
            // std::cout << " (width, hei
            // std::lock_guard<std::mutex> lock(_mutex);
            _threads.emplace_back(std::thread(&SaveJob::write, saveJob));
            _saveJobs.push_back(saveJob);
           // _jobThreads.emplace_back(std::thread(&SaveJob::write, this, std::move(promise), saveJob));
           _threads.back().join();
           // set the wait
           /*
            std::for_each(_threads.begin(), _threads.end(), [](std::thread &t) {
                t.join();
            });
            */


           // lock.unlock();
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
    _threads.back().join();
    // _threads.emplace_back(std::thread(&SaveJob::saveFile, this, std::move(promise)));
    std::string returnMessage = future.get();
}

void AutoSave::saveFile(std::promise<std::string> && promise, SaveJob saveJob)
{
    // TODO: use the ImageIO library to write this out.
    // ImageIO_PPM::writeTest(saveJob.getFileName(), saveJob.getImageBuffer());
    saveJob.write();

}

void AutoSave::addSaveJobToQueue(std::shared_ptr<SaveJob> saveJob)
{
    _threads.emplace_back(std::thread(&SaveJob::write, saveJob));
    
    // Create a thread barrier
    _threads.back().join();
    /*
    std::unique_lock<std::mutex> lock(_mutex);
    std::cout << " AutoSave::addSaveJobToQueue : thread id = " << std::this_thread::get_id() << std::endl;
    lock.unlock();

    // add new Save Job to the end of the thread queueh
    // 
    std::promise<void> promiseSaveJob;
    std::future<void> futureSaveJob = promiseSaveJob.get_future();
    _waitingSaveJobs.pushBack(saveJob, std::move(promiseSaveJob));
    */
    // TODO: Not exactly sure how to call the Function in Save Job to write save the file to disk
    // One possibility is to use the functor, which is called automatically.
}

void AutoSave::sendMessageAtInterval()
{
   unsigned int interval = 10000; // milliseconds

    unsigned int counter = 0;
   while (true)
   {
       std::this_thread::sleep_for(std::chrono::milliseconds(interval));
       // std::unique_lock<std::mutex> lock(_mutex);
        std::cout << " Auto Save ... " << std::endl;
        // lock.unlock();
        // bool autoSave = true;
        _messageQueue->send(std::move(counter));
        counter++;
        // std::cout << " Message Queue Size " << _messageQueue->size() << std::endl;
         // I could write the output here
         // Question: should mutex be used for console messages?
   }
}

int AutoSave::getCounter()
{
    return _counter;
}
