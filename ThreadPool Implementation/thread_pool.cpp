#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;
class Threadpool
{
private:
    int m_threads;
    vector<thread> threads;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    bool stop;

public:
    // explicit checks if the constructor is called with same datatype no type casted datatype should be passed
    explicit Threadpool(int num_threads) : m_threads(num_threads), stop(false)
    {
        for (int i = 0; i < m_threads; i++)
        {
            threads.emplace_back([this]
                                 {
                    while (true) {
                        unique_lock<mutex> lock(mtx);
                        //in short for condtion we want to perform task we write its opposite in wait lambda funciton(predicate)
                        //The thread will move forward (i.e., exit the wait state) when the predicate returns true
                        cv.wait(lock, [this] {return !tasks.empty() || stop;}); 
                        //basically when condition is true the thread is on wait (and to make it wait we have to write a false scenario so that we dont want the thread to execute the main task)


                        if(stop){ // this means the destructor is called and all the threads are to be terminated no more execution of tasks is to be done
                            return; // stop unecessary execution;
                        }

                        function<void()> task = std::move(tasks.front());
                        tasks.pop();
                        cout<<"Size of Queue : "<<tasks.size()<<endl;
                        lock.unlock(); //unlock before executing the task as another thread can push the task to the queue

                        task(); // execute the task as this can take long time to execute so outside of unlock
                    } });
        }
    }
    ~Threadpool()
    {
        unique_lock<mutex> lock(mtx);
        stop = true;
        lock.unlock();
        cv.notify_all(); // notify all the threads to wake up and finish the tasks

        for (auto &thread : threads)
        {
            thread.join(); // wait for all the finish the tasks
        }
    }

    void ExecuteTask(function<void()> task)
    {
        unique_lock<mutex> lock(mtx);
        tasks.push(task);
        lock.unlock();
        cv.notify_one(); // since we have added the task we can notify one thread
    }
};
int main()
{

    Threadpool pool(8);
    while (1)
    {
        pool.ExecuteTask([]
                         {cout << "Task 1" <<this_thread::get_id()<<" started\n";
                         this_thread::sleep_for(chrono::seconds(1)); // Simulate work
                         cout << "Task 1" <<  " completed\n"; });
        pool.ExecuteTask([]
                         {cout << "Task 2" <<  " started\n";
                         this_thread::sleep_for(chrono::seconds(1)); // Simulate work
                         cout << "Task 2" <<   " completed\n"; });
    }
}
