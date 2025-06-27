#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
  public:
    explicit ThreadPool(size_t threads_count);
    ~ThreadPool();

    void enqueue_task(std::function<void()> task);
    void stop();

  private:
    std::vector<std::thread>          workers;
    std::queue<std::function<void()>> task_queue;
    std::mutex                        queue_mutex;
    std::condition_variable           cv;
    std::atomic<bool>                 stop_flag;
};

#endif
