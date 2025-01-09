#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

class ThreadPool {
    public:
        explicit ThreadPool(size_t numThreads);
        ~ThreadPool();
        
        void enqueueTask(std::function<void()> task);
        void stop();

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> taskQueue;
        std::mutex queueMutex;
        std::condition_variable cv;
        std::atomic<bool> stopFlag;
};

#endif
