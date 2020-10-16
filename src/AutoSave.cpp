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
  _messageQueue2 = std::shared_ptr<MessageQueue<SaveJob>>();
}

AutoSave::AutoSave(MandelbrotPanel *mandelbrotPanel) : _mandelbrotPanel(mandelbrotPanel)
{
  // std::cout << " AutoSave Constructor " << std::endl;
  _messageQueue = std::make_shared<MessageQueue<int>>();
    _messageQueue2 = std::shared_ptr<MessageQueue<SaveJob>>();
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

void AutoSave::runSaveMessagesThread() 
{
    _threads.emplace_back(std::thread(&AutoSave::sendSaveMessages, this));
}

void AutoSave::sendSaveMessages()
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
        FileType fileType = PPM;
        std::string fileName = "autosave_" + std::to_string(counter) + ".ppm";
        ImageBuffer<unsigned char> imageBuffer(10,10);
        // BufferEffects::setColor(yellow, imageBuffer);

        SaveJob saveJob(fileType, fileName, imageBuffer);
        _messageQueue2->send(std::move(saveJob));
        // _messageQueue->send(std::move(counter));
        counter++;
        // std::cout << " Message Queue Size " << _messageQueue->size() << std::endl;
         // I could write the output here
         // Question: should mutex be used for console messages?
    }

}

void AutoSave::runReceiveMessagesThread()
{
     _threads.emplace_back(std::thread(&AutoSave::receiveSaveMessages, this));
}
void AutoSave::receiveSaveMessages() 
{
    while (true) 
    {
        std::cout << " *** Receive Save Message " << std::endl;
        auto saveJob = _messageQueue2->receive();
    }
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
    _saveState = false;
   while (true)
   {
       std::this_thread::sleep_for(std::chrono::milliseconds(1));
       int autoSave = _messageQueue->receive();
       
       if (autoSave >= 0) {
           std::unique_lock<std::mutex> myLock(_mutex);
           _counter++;
           _saveState = true;
           std::cout << " message received " << autoSave << std::endl;
           std::string fileName = std::string("autosave_" + std::to_string(autoSave) + std::string(".ppm"));
           std::cout << " file name " << fileName << std::endl;
           myLock.unlock();
           // Call addSaveJobToQueue
           // Create a local version of SaveJob
           // TODO: Create a red buffer and test
           // TODO: Get the image buffer from the MandelbrotPanel
           std::shared_ptr<SaveJob> saveJob(new SaveJob);
           saveJob->setFileName(fileName);
           ImageBuffer<unsigned char> imageBuffer(400,400);
           saveJob->setImageBuffer(imageBuffer);
           BufferEffects::setColor(red, imageBuffer);
           saveJob->setFileType(PPM);
           std::this_thread::sleep_for(std::chrono::milliseconds(1));
           auto future = std::async(std::launch::async, &AutoSave::addSaveJobToQueue, this, saveJob);
           future.get();
           // return _counter;
           // std::unique_lock<std::mutex> myLock(_mutex);
           // std::lock_guard<std::mutex> lock(_mutex);
           // std::string fileName = std::string("autosave_" + std::to_string(autoSave) + std::string(".ppm"));
           
           // std::cout << " Save Message Received " << " " << _counter << " " << fileName << std::endl;
           // std::cout << "auto save number " << autoSave << std::endl;
           // myLock.unlock();
           // _mandelbrotPointer->sayHello();
           // lock.unlock();
           // std::lock_guard<std::mutex> lock(_mutex);
           // std::promise<std::string> promise;
           // std::future<std::string> future = promise.get_future();

           // start thread and pass promise as argument
           // Need to get the latest buffer from
           // std::shared_ptr<SaveJob> saveJob(new SaveJob);
           
           // saveJob->setFileName(fileName);
           // saveJob->setFileType(PPM);
           // ImageBuffer<unsigned char> imageBuffer = _mandelbrotPanel->getImageBuffer();
           // ImageBuffer<unsigned char> imageBuffer(400,400);
           // BufferEffects::setColor(red, imageBuffer);
           // ImageBuffer<unsigned char> imageBuffer(100,100);
           // BufferEffects::setColor(blue, imageBuffer);
           // saveJob->setImageBuffer(imageBuffer);
           // auto future = std::async(std::launch::async, &AutoSave::addSaveJobToQueue, this, saveJob);
           // future.get();
           // ImageBuffer<unsigned char> temp = _mandelbrotPointer->getImageBuffer2();
            // ImageBuffer<unsigned char> imageBuffer(temp.getWidth(), temp.getHeight());
            // std::cout << " (width, hei
            // std::lock_guard<std::mutex> lock(_mutex);
            // _threads.emplace_back(std::thread(&SaveJob::write, saveJob));
            // _saveJobs.push_back(saveJob);
           // _jobThreads.emplace_back(std::thread(&SaveJob::write, this, std::move(promise), saveJob));
           // _threads.back().join();
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
    // _threads.back().join();
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
    // waitForAutoSaveMessage();
    
    std::unique_lock<std::mutex> lock(_mutex);
    // std::cout << " save id = " << temp << std::endl;
    std::cout << "::addSaveJobToQueue: thread id = " << std::this_thread::get_id() << std::endl;
    lock.unlock();

    
    // Add new save job to the end of the waiting line
    
    std::promise<void> promiseSaveJob;
    std::future<void> futureSaveJob = promiseSaveJob.get_future();

    _waitingSaveJobs.pushBack(saveJob, std::move(promiseSaveJob));

    // wait until the saveJob is ready to run
    // Do not wait here.  It causes problems.
    // futureSaveJob.wait();
    lock.lock();
    std::cout << " Save Job file name " << saveJob->getFileName() << "is granted entry " << std::endl;
    lock.unlock();
    
}

void AutoSave::sendMessageAtInterval()
{
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

void AutoSave::saveJobPollingLoop()
{
    _threads.emplace_back(std::thread(&AutoSave::processSaveJobQueue, this));
}

void AutoSave::processSaveJobQueue()
{
    // continuously process the queue of save jobs
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (_waitingSaveJobs.getSize() > 0) {

            _waitingSaveJobs.permitEntryToFirstInQueue();
        }
    }
}

void AutoSave::monitorAutoSaveMessages()
{
    /*
    while (true) {
        if (!getCurrentSaveState()) 
        {
            waitForAutoSaveMessage();
            // Call addSaveJob here
        }
    }
    */
}