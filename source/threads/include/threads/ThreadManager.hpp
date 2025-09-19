#ifndef MEPHI_REACTOR_SOURCE_TREADS_INCLUDE_THREADS_THREAD_MANAGER_HPP
#define MEPHI_REACTOR_SOURCE_TREADS_INCLUDE_THREADS_THREAD_MANAGER_HPP

#include <mutex>
#include <condition_variable>

namespace Mephi
{

template<typename T>
class ThreadManager{
private:
    std::mutex mtx;
    std::condition_variable cv;
    T data;
    bool new_data_rdy;
    std::atomic<bool> running;
public:
    ThreadManager() 
        : mtx{}, cv{}, data{}, new_data_rdy{false}, running{true}
    {}

    void setData(T new_val) {
        std::lock_guard<std::mutex> lock(mtx);
        data = new_val;
        new_data_rdy = true;
        cv.notify_one();
    }
    
    bool getData(T& result) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return new_data_rdy || !running; });
        
        if (!running) return false;
        
        result = data;
        new_data_rdy = false;
        return true;
    }
    
    void stop() {
        running = false;
        cv.notify_all();
    }
    
    bool isRunning() const {
        return running;
    }
};
    
}
#endif /*MEPHI_REACTOR_SOURCE_TREADS_INCLUDE_THREADS_THREAD_MANAGER_HPP*/
