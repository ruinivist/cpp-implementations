#include <bits/stdc++.h>
using namespace std;

class ThreadPool {
   private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    bool stop = false;

   public:
    ThreadPool(int num_threads) {
        while (num_threads-- > 0) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(mtx);
                        cv.wait(lock,
                                [this] { return stop || !tasks.empty(); });

                        if (stop && tasks.empty()) return;

                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template <typename F, typename... Args>
    future<typename result_of<F(Args...)>::type> enqueue(F&& f,
                                                         Args&&... args) {
        if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");

        using return_type = typename std::result_of<F(Args...)>::type;
        // this is not calling the return type its a func with no args and
        // returning return type value as in int(int) this is int()
        auto task = make_shared<packaged_task<return_type()>>(
            bind(forward<F>(f), forward<Args>(args)...));
        future<return_type> fut = task->get_future();
        // since its a share pointer so the task lives as long as future lives
        // you could not make it shared but then pool might die with future
        // being invalid now the desctructor isn't called now unless the future
        // also goes out of scope

        {
            unique_lock<mutex> lock(mtx);
            tasks.emplace([task] { (*task)(); });
        }

        cv.notify_one();
        return fut;
    }

    ~ThreadPool() {
        {
            unique_lock<mutex> lock(mtx);
            stop = true;
        }

        cv.notify_all();
        for (auto& worker : workers) worker.join();
    }
};

int main() {
    ThreadPool pool(4);

    auto taskFunction = [](int a, int& b, std::string&& c) {
        std::cout << "Task executed with: " << a << ", " << b << ", " << c
                  << std::endl;
        b *= 2;  // Modify the lvalue reference
        return a + b;
    };

    int lval = 10;
    auto future = pool.enqueue(taskFunction, 5, ref(lval), "hello");

    cout << "result: " << future.get() << endl;
}