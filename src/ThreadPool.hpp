#pragma once
#include <condition_variable> 
#include <functional> 
#include <iostream> 
#include <mutex> 
#include <queue> 
#include <thread>
#include <future>
#include <unordered_set>
#include <atomic>
#include <vector>

class ThreadPool
{
private:

    std::queue<std::pair<std::future<void>, int64_t>> q; // очередь задач - хранит функцию(задачу), которую нужно исполнить и номер данной задачи
    std::mutex q_mtx;
    std::condition_variable q_cv;

    std::unordered_set<int64_t> completed_task_ids;      // помещаем в данный контейнер исполненные задачи
    std::condition_variable completed_task_ids_cv;
    std::mutex completed_task_ids_mtx;

    std::vector<std::thread> threads;


    std::atomic<bool> quite{ false };                    // флаг завершения работы thread_pool
    std::atomic<int64_t> last_idx = 0;                   // переменная хранящая id который будет выдан следующей задаче

    void run() {
        while (!quite) {
            std::unique_lock<std::mutex> lock(q_mtx);
            q_cv.wait(lock, [this]()->bool { return !q.empty() || quite; });

            if (!q.empty()) {
                auto elem = std::move(q.front());
                q.pop();
                lock.unlock();

                elem.first.get();

                std::lock_guard<std::mutex> lock(completed_task_ids_mtx);
                completed_task_ids.insert(elem.second);

                completed_task_ids_cv.notify_all();
            }
        }
    }

public:

    ThreadPool(uint32_t num_threads) {
        threads.reserve(num_threads);
        for (uint32_t i = 0; i < num_threads; ++i) {
            threads.emplace_back(&ThreadPool::run, this);
        }
    }

    template <typename Func, typename ...Args>
    int64_t addTaskThreadPool(const Func& task_func, Args&&... args) {
        int64_t task_idx = last_idx++;

        std::lock_guard<std::mutex> q_lock(q_mtx);
        q.emplace(std::async(std::launch::deferred, task_func, args...), task_idx);
        q_cv.notify_one();
        return task_idx;
    }


    ~ThreadPool() {
        quite = true;
        for (uint32_t i = 0; i < threads.size(); ++i) {
            q_cv.notify_all();
            threads[i].join();
        }
    }

    void wait_all() {
        std::unique_lock<std::mutex> lock(q_mtx);
        completed_task_ids_cv.wait(lock, [this]()->bool {
            std::lock_guard<std::mutex> task_lock(completed_task_ids_mtx);
            return q.empty() && last_idx == completed_task_ids.size();
            });
    }
};
