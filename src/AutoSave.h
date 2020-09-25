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


#include "MessageQueue.h"
#include "WaitingSaveJobs.h"

class ImageWriter
{
public:
   static void write1();
   static void write(std::promise<void> &&promise, std::string fileName);
};

class AutoSave
{
public:
   AutoSave();
   ~AutoSave();

   void runTimerOnThread();
   void runMonitorOnThread();
   void waitForAutoSaveMessage();
   void launchSaveJobOnThread();
   void saveFile(std::promise<std::string> && promise, std::string fileName);
private:
   void sendMessageAtInterval();
// Add a MessageQueue of AutoSaveJobs here
   std::vector<std::thread> _threads;  // holds all threads that have been launched within this object
   std::shared_ptr<MessageQueue<bool>> _messageQueue;
   WaitingSaveJobs _waitingSaveJobs;
   static std::mutex _mutex;
   
//
};


#endif // AUTOSAVE_H_