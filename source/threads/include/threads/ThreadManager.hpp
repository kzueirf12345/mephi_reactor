#ifndef MEPHI_REACTOR_SOURCE_TREADS_INCLUDE_THREADS_THREAD_MANAGER_HPP
#define MEPHI_REACTOR_SOURCE_TREADS_INCLUDE_THREADS_THREAD_MANAGER_HPP

#include <mutex>
#include <condition_variable>

namespace Mephi
{

template<typename T>
class ThreadManager{
private:
    std::mutex mtx_;
    std::condition_variable condVar_;
    T data_;
    bool isDataRdy_;
    std::atomic<bool> isRunning_;
public:
    ThreadManager() 
        : mtx_{}, condVar_{}, data_{}, isDataRdy_{false}, isRunning_{true}
    {}

    void setData(T newVal) {
        std::lock_guard<std::mutex> lock(mtx_);
        data_ = newVal;
        isDataRdy_ = true;
        condVar_.notify_one();
    }
    
    bool getData(T& result) {
        std::unique_lock<std::mutex> lock(mtx_);
        condVar_.wait(lock, [this]() { return isDataRdy_ || !isRunning_; });
        
        if (!isRunning_) return false;
        
        result = data_;
        isDataRdy_ = false;
        return true;
    }
    
    void stop() {
        isRunning_ = false;
        condVar_.notify_all();
    }
    
    bool isRunning() const {
        return isRunning_;
    }
};
    
}
#endif /*MEPHI_REACTOR_SOURCE_TREADS_INCLUDE_THREADS_THREAD_MANAGER_HPP*/
