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

   void runTimerOnThread();
   void runMonitorOnThread();
   void waitForAutoSaveMessage();
   void launchSaveJobOnThread(std::string fileName);
   void saveFile(std::promise<std::string> && promise, SaveJob saveJob);
   void addSaveJobToQueue(std::shared_ptr<SaveJob> saveJob);
private:
   static int getCounter();
   void sendMessageAtInterval();
// Add a MessageQueue of AutoSaveJobs here
   std::vector<std::thread> _threads;  // holds all threads that have been launched within this object
   std::vector<std::thread> _jobThreads; 
   // std::shared_ptr<MessageQueue<bool>> _messageQueue;
   std::shared_ptr<MessageQueue<int>> _messageQueue;
   WaitingSaveJobs _waitingSaveJobs;
   static std::mutex _mutex;
   SaveJob _saveJob;
   static unsigned long _counter;
   MandelbrotPanel *_mandelbrotPanel;
   std::vector<std::shared_ptr<SaveJob>> _saveJobs;


//
};


#endif // AUTOSAVE_H_