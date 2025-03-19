// lock_guard is a class template that provides a convenient way to lock and unlock a mutex in a RAII (Resource Acquisition Is Initialization) pattern.

//  0. It is very light weight wrapper for owning mutex on scoped basis.
//  1. It acquires mutex lock the moment you create the object of lock_guard.
//  2. It automatically removes the lock while goes out of scope.
//  3. You can not explicitly unlock the lock_guard.
//  4. You can not copy lock_guard.
// RVO and NRVO.

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex m1;
int buffer = 0;

void task(const char *threadNumber, int loopFor)
{
    // m1.lock();
    lock_guard<mutex> lock(m1); // When the scope of the lock is over, the restrictor is called and in the destructor unlock method is called
    for (int i = 0; i < loopFor; i++)
    {
        cout << "Thread " << threadNumber << ": " << buffer++ << endl;
    }
    // m1.unlock();
    cout << "Unlock by thread " << threadNumber << endl;
}

int main()
{
    thread t1(task, "T1", 5);
    thread t2(task, "T2", 5);
    t1.join();
    t2.join();
    return 0;
}