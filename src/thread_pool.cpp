#include "thread_pool.h"

ThreadPool::ThreadPool(size_t threads_count) : stop_flag(false) {
    for (size_t i = 0; i < threads_count; i++) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    cv.wait(lock, [this]() {
                        return stop_flag || !task_queue.empty();
                    });

                    if (stop_flag && task_queue.empty()) {
                        return;
                    }

                    task = std::move(task_queue.front());
                    task_queue.pop();
                }

                task();
            }
        });
    }
};

ThreadPool::~ThreadPool() {
    if (stop_flag) {
        stop();
    }
};

void ThreadPool::enqueue_task(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        task_queue.push(std::move(task));
    }
    
    cv.notify_one();
};

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop_flag = true;
    }

    cv.notify_all();

    for (std::thread& worker : workers) {
        worker.join();
    }
};
