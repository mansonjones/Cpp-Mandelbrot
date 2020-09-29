
#include "WaitingSaveJobs.h"


WaitingSaveJobs::WaitingSaveJobs() : _numberOfSaveJobs() 
{}

int WaitingSaveJobs::getSize()
{
    std::lock_guard<std::mutex> lock(_mutex);

    return _saveJobs.size(); 
}

void WaitingSaveJobs::pushBack(SaveJob saveJob, std::promise<void> &&promise)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _saveJobs.push_back(saveJob);
    _promises.push_back(std::move(promise));
}

SaveJob WaitingSaveJobs::popBack()
{
    // perform vector modification under the lock
    std::lock_guard<std::mutex> lock(_mutex);

    // remove last vector from the queue
    SaveJob v = std::move(_saveJobs.back());
    _saveJobs.pop_back();
    return v;
}
void WaitingSaveJobs::runFirstJobInQueue() 
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto firstPromise = _promises.begin();
    auto firstSaveJob = _saveJobs.begin();

    // remove
    _saveJobs.erase(firstSaveJob);
    _promises.erase(firstPromise);
}