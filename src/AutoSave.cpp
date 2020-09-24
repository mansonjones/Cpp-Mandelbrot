#include "AutoSave.h"
#include <chrono>

void ImageWriter::write(std::promise<void> &&promise, std::string fileName)
{
   std::cout << " ImageWriter::write " << std::endl;
   std::string fileNameTemp = "output1.txt";
   std::ofstream outputFileStream(fileNameTemp);
   if (outputFileStream.is_open()) {
      outputFileStream << "This is the first line" << std::endl;
   }
   outputFileStream.close();
}

void ImageWriter::write1()
{
   std::cout << " ImageWriter::write " << std::endl;
   std::string fileName = "output1.txt";
   std::ofstream outputFileStream(fileName);
   if (outputFileStream.is_open()) {
      outputFileStream << "This is the first line" << std::endl;
   }
   outputFileStream.close();
}
AutoSave::AutoSave()
{
  std::cout << " AutoSave Constructor " << std::endl;
  _messageQueue = std::make_shared<MessageQueue<bool>>();
}

AutoSave::~AutoSave()
{
  // set up thread barrier before this object is destroyed
  std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
     t.join();
  });
}

void AutoSave::runScheduler()
{
   threads.emplace_back(std::thread(&AutoSave::schedule, this));
}

void AutoSave::waitForAutoSave()
{
   while (true)
   {
       // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      bool saveFile = _messageQueue->receive();
      if (saveFile == true) {
          // return;
         std::cout << " wait for Save " << std::endl;
         // ImageWriter::write1();
         std::promise<void> promise;
         std::future<void> future = promise.get_future();

         // future.wait();

        std::string fileName2 = "output2.txt";
        std::thread t(ImageWriter::write, std::move(promise), fileName2);
        future.get();
        // thread barrier
        t.join();
         // ImageWriter::write();
         //
         // Use promise - future to save file
      }
   }
}

void AutoSave::schedule()
{
   unsigned int interval = 5000; // milliseconds

   while (true)
   {
       std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        std::cout << " Auto Save ... " << std::endl;
        bool autoSave = true;
        _messageQueue->send(std::move(autoSave));
        std::cout << " Message Queue Size " << _messageQueue->size() << std::endl;
         // I could write the output here
         // Question: should mutex be used for console messages?
   }
}

