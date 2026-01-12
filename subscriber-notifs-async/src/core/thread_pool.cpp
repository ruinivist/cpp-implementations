#include "core/thread_pool.hpp"

#include <iostream>

ThreadPool::ThreadPool(size_t num_threads) : stop_(false), active_tasks_(0) {
    workers_.reserve(num_threads);
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this] { worker(); });
    }
}

ThreadPool::~ThreadPool() noexcept {
    // cv gotcha 1: cv predicate to be touched under lock
    {
        std::scoped_lock lock(queue_mutex_);
        stop_.store(true, std::memory_order_release);
    }

    condition_.notify_all();
    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::scoped_lock lock(queue_mutex_);

        // Don't accept new tasks if shutting down
        if (stop_.load(std::memory_order_acquire)) [[unlikely]] {
            return;
        }

        tasks_.push(std::move(task));
    }

    // Notify one worker thread that a task is available
    condition_.notify_one();
}

void ThreadPool::wait_for_completion() noexcept {
    std::unique_lock lock(queue_mutex_);
    completion_condition_.wait(lock, [this] {
        return tasks_.empty() &&
               active_tasks_.load(std::memory_order_acquire) == 0;
    });
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock lock(queue_mutex_);

            // Wait for a task or stop signal
            condition_.wait(lock, [this] {
                return stop_.load(std::memory_order_acquire) || !tasks_.empty();
            });

            // Exit if stopping and no tasks remain
            if (stop_.load(std::memory_order_acquire) && tasks_.empty())
                [[unlikely]] {
                return;
            }

            // Get the next task
            if (!tasks_.empty()) {
                task = std::move(tasks_.front());
                tasks_.pop();
                active_tasks_.fetch_add(1, std::memory_order_release);
            }
        }

        // when can you do if(task)
        // -> when operator bool() is deined
        // defined for std::function, shared/unique ptr, optionals etc
        if (task) {
            try {
                task();
            } catch (const std::exception& e) {
                std::cerr << "Thread pool task threw exception: " << e.what()
                          << '\n';
            } catch (...) {
                // anything not derived from std::exception
                std::cerr << "Thread pool task threw unknown exception\n";
            }

            const auto remaining =
                active_tasks_.fetch_sub(1, std::memory_order_acq_rel) - 1;

            // Notify anyone waiting for completion if this is the last task
            if (remaining == 0) [[unlikely]] {
                completion_condition_.notify_all();
            }
        }
    }
}
