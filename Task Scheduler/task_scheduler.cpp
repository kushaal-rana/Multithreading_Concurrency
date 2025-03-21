#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;
class TaskScheduler
{
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;     // Flag to stop threads
    int activeWorkers = 0; // Track running workers

public:
    TaskScheduler(size_t numThreads)
    {
        for (size_t i = 0; i < numThreads; ++i)
        {
            workers.emplace_back([this]()
                                 {
                        while (true) 
                        {
                            unique_lock<mutex> lock(mtx);
                            cv.wait(lock, [this] { return !tasks.empty() || stop; });
    
                            if (stop && tasks.empty()) return;

                            function<void()> task = std::move(tasks.front());
                            tasks.pop();
                            activeWorkers++;
                            lock.unlock();

                            task(); //Execute the task
                            
                            unique_lock<mutex> lock2(mtx);
                            activeWorkers--;
                            lock2.unlock();
                            cv.notify_all(); //Notify when a worker finishes
                        } });
        }
    }
    void submit(function<void()> task)
    {
        {
            unique_lock<mutex> lock(mtx);
            tasks.push(std::move(task));
        }
        cv.notify_one(); // Wake up one worker thread
    }
    void block_until_finish()
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]
                { return tasks.empty() && activeWorkers == 0; });
    }

    ~TaskScheduler()
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true; // Inform the worker threads to stop
        }
        cv.notify_all(); // Wake up all threads to let them exit
        for (std::thread &worker : workers)
        {
            worker.join(); // Ensure all threads finish execution
        }
    }
};

int main()
{
    TaskScheduler scheduler(4);
    scheduler.submit([]
                     { std::cout << "Task 1\n"; });
    scheduler.submit([]
                     { std::cout << "Task 2\n"; });
    scheduler.block_until_finish();
    return 0;
}
