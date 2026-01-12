#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
   public:
    explicit ThreadPool(
        size_t num_threads = std::thread::hardware_concurrency());

    ~ThreadPool() noexcept;

    void enqueue(std::function<void()> task);

    // Get the number of worker threads
    [[nodiscard]] size_t thread_count() const noexcept {
        return workers_.size();
    }

    // Wait for all queued tasks to complete
    void wait_for_completion() noexcept;

    // Disable copy and move
    // sync primitives are not movable or copyable
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

   private:
    // Worker thread function
    void worker();

    // Thread management
    std::vector<std::thread> workers_;

    // Task queue
    std::queue<std::function<void()>> tasks_;

    // Synchronization primitives
    std::mutex queue_mutex_;
    std::condition_variable condition_;

    // Shutdown flag
    std::atomic<bool> stop_;

    // Track active tasks for wait_for_completion
    std::atomic<size_t> active_tasks_;
    std::condition_variable completion_condition_;
};
