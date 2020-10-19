#ifndef AUTOSAVE_H_
#define AUTOSAVE_H_

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <queue>
#include <future>
#include <mutex>
#include <random>
#include <memory>

#include "mandelbrot.h"
#include "MessageQueue.h"
#include "WaitingSaveJobs.h"
#include "SaveJob.h"
// #include "mandelbrot_gui.h"
class MandelbrotPanel;

class AutoSave
{
public:
   AutoSave();
   AutoSave(MandelbrotPanel *pointer);
   ~AutoSave();

   void emitAutoSaveMessagesOnThread();
   // void emitAutoSaveMessages();

   void waitForAutoSaveMessagesOnThread();
   // void waitForAutoSaveMessages();

   int messageQueueSize() { return _messageQueue->size(); }
private:
   void emitAutoSaveMessages();
   void waitForAutoSaveMessages();
   static int getCounter();
   std::vector<std::thread> _threads;  // holds all threads that have been launched within this object
   std::shared_ptr<MessageQueue<int>> _messageQueue;
   static std::mutex _mutex;
   static unsigned long _counter;
   MandelbrotPanel *_mandelbrotPanel;

   void writeFile(std::promise<std::string> &&promise, std::shared_ptr<SaveJob> saveJob); 

//
};


#endif // AUTOSAVE_H_