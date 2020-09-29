#ifndef WAITING_SAVE_JOBS_H_
#define WAITING_SAVE_JOBS_H_

#include <future>
#include <memory>
#include <mutex>
#include <vector>

#include "SaveJob.h"

class WaitingSaveJobs
{
public:
    WaitingSaveJobs();
    int getSize();
    SaveJob popBack();
    void pushBack(SaveJob saveJob, std::promise<void> &&promise);
    void runFirstJobInQueue();
private:
    std::vector<SaveJob> _saveJobs;
    std::vector<std::promise<void>> _promises;
    std::mutex _mutex;
    int _numberOfSaveJobs;
};

#endif // WAITING_SAVE_JOBS_H_