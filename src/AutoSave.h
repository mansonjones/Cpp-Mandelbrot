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

/*
template <class T>
class MessageQueue
{
public:
    T receive()
    {
        // perform queue modification under the lock
        std::unique_lock<std::mutex> uLock(_mutex);
        _cond.wait(uLock, [this] { return !_messages.empty(); }); // pass unique lock to condition variable

        // remove last vector element from queue
        T msg = std::move(_messages.back());
        _messages.pop_back();

        return msg; // will not be copied due to return value optimization (RVO) in C++
    }

    void send(T &&msg)
    {
        // simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // perform vector modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);

        // add vector to queue
        std::cout << "   Message " << msg << " has been sent to the queue" << std::endl;
        _messages.push_back(std::move(msg));
        _cond.notify_one(); // notify client after pushing new Vehicle into vector
    }

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    std::deque<T> _messages;
};
*/

class ImageWriter
{
public:

   static void write(std::promise<void> &&promise);
};

class AutoSave
{
public:
   AutoSave();
   ~AutoSave();
// private:
   void runScheduler();
   void waitForAutoSave();
private:
   void schedule();
// Add a MessageQueue of AutoSaveJobs here
   std::vector<std::thread> threads;  // holds all threads that have been launched within this object
   std::shared_ptr<MessageQueue<bool>> _messageQueue;
//
};


#endif // AUTOSAVE_H_