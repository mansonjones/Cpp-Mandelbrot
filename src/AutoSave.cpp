#include "AutoSave.h"


void ImageWriter::write(std::promise<void> &&promise)
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
      bool saveFile = _messageQueue->receive();
      if (saveFile == true) {
         std::cout << " wait for Save " << std::endl;
         std::promise<void> promise;
         std::future<void> future = promise.get_future();

         std::thread t(ImageWriter::write, std::move(promise));
         t.join();
         // ImageWriter::write();
         //
         // Use promise - future to save file
      }
   }
}

void AutoSave::schedule()
{
   double interval = 5000; // milliseconds
   std::chrono::time_point<std::chrono::system_clock> startTime;

   startTime = std::chrono::system_clock::now();

   while (true)
   {
      long timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count();

      if (timeElapsed > interval)
      {
         std::cout << " Auto Save ... " << std::endl;
         bool autoSave = true;
         _messageQueue->send(std::move(autoSave));
         // I could write the output here
         // All I need is
         startTime = std::chrono::system_clock::now();
         // TODO: send an update to the message queue
         //
      }
   }
}

