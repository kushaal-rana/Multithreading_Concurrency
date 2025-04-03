// Implement two methods in a class schedule(), wait until complete()
// schedule() should enqueue work to be performed and should be non-blocking
// WaitUnitll Complete() should block all the calls until all scheduled work is completed.
// Builtin  thread safe constructs like dequeue, blocking, Queue etc. can't be used implemented using lock etc ensuring threat safety

#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <mutex>
#include <exception>

class TaskScheduler
{
private:
    std::mutex mtx;
    std::vector<std::thread> threads;
    std::vector<std::exception_ptr> exceptions; // To capture exceptions from threads

public:
    void schedule(const std::function<void()> &task)
    {
        std::lock_guard<std::mutex> lock(mtx);
        threads.emplace_back([this, task]()
                             {
            try {
                task();
            } catch (...) {
                // Capture exception and store it safely
                std::lock_guard<std::mutex> exLock(mtx);
                exceptions.push_back(std::current_exception());
            } });
    }

    void waitUntilComplete()
    {
        std::vector<std::thread> localThreads;
        {
            std::lock_guard<std::mutex> lock(mtx);
            localThreads.swap(threads);
        }
        for (std::thread &t : localThreads)
        {
            if (t.joinable())
            {
                t.join();
            }
        }

        // If any exception was captured, rethrow the first one.
        if (!exceptions.empty())
        {
            std::rethrow_exception(exceptions[0]);
        }
    }
};

//////////////////////
/// Example Usage:
//////////////////////
void exampleTask(int id)
{
    std::cout << "Task " << id << " is starting." << std::endl;
    // Simulate some work with sleep.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Task " << id << " is complete." << std::endl;
}

int main()
{
    TaskScheduler scheduler;

    // Schedule several tasks.
    for (int i = 0; i < 5; i++)
    {
        scheduler.schedule([i]()
                           { exampleTask(i); });
    }

    // Wait until all tasks are complete.
    scheduler.waitUntilComplete();

    std::cout << "All tasks completed." << std::endl;
    return 0;
}
