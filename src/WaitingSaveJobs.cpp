
#include "WaitingSaveJobs.h"


WaitingSaveJobs::WaitingSaveJobs()
{}

int WaitingSaveJobs::getSize()
{
    std::lock_guard<std::mutex> lock(_mutex);

    return _saveJobs.size(); 
}

void WaitingSaveJobs::pushBack(std::shared_ptr<SaveJob> saveJob, std::promise<void> &&promise)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _saveJobs.push_back(saveJob);
    _promises.push_back(std::move(promise));
}

void WaitingSaveJobs::permitEntryToFirstInQueue()
{
    std::lock_guard<std::mutex> lock(_mutex);

    // get entries from the front of both queues
    auto firstPromise = _promises.begin();
    auto firstSaveJob = _saveJobs.begin();

    // fulfill pomise and send signal back that permission to enter has been granted
    firstPromise->set_value();
    (*firstSaveJob)->write();

    // remove fron elements from both queues
    _promises.erase(firstPromise);
    _saveJobs.erase(firstSaveJob);
}
