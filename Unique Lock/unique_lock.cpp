/* Few important points to remember about the unique_lock is as follows:
1. The class unique_lock is a mutex ownership wrapper.
2. It Allows:
    a. Can Have Different Locking Strategies
    b. time-constrained attempts at locking (try_lock_for, try_lock_until)
    c. recursive locking
    d. transfer of lock ownership (move not copy)
    e. condition variables. (See this in coming videos)

Locking Strategies
   TYPE           EFFECTS(S)
1. defer_lock :  do not acquire ownership of the mutex.
2. try_to_lock:  try to acquire ownership of the mutex without blocking.
3. adopt_lock :  assume the calling thread already has ownership of the mutex. */

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex m1;
int buffer = 0;

void task(const char *threadNumber, int loopFor)
{
    unique_lock<mutex> lock(m1); // When the scope of the lock is over, the destructor is called and in the destructor unlock method is called
    for (int i = 0; i < loopFor; i++)
    {
        cout << "Thread " << threadNumber << ": " << buffer++ << endl;
    }
    cout << "Unlock by thread " << threadNumber << endl;
}

void deferTask(const char *threadNumber, int loopFor)
{
    unique_lock<mutex> lock(m1, defer_lock); // Does not call lock on mutex m1, because used defer_lock
    // so there you just own the lock and defer the locking in the future
    lock.lock(); // But then we will have to explicitly call lock() when we want to acquire the lock mutex m1
    for (int i = 0; i < loopFor; i++)
    {
        buffer++;
        cout << "Thread " << threadNumber << ": " << buffer << endl;
    }
}

int main()
{
    // thread t1(task, "A", 5);
    // thread t2(task, "B", 5);
    thread t3(deferTask, "C", 5);
    thread t4(deferTask, "D", 5);
    // t1.join();
    // t2.join();
    t3.join();
    t4.join();
    return 0;
}

// Difference between unique_lock and lock_guard:
//  Difference between unique_lock and lock_guard:
//  unique_lock:
//  1. More flexible than lock_guard.
//  2. Can be used with different locking strategies (defer_lock, try_to_lock, adopt_lock).
//  3. Can be locked and unlocked multiple times.
//  4. Can be moved (transfer ownership of the lock).
//  5. Supports time-constrained attempts at locking (try_lock_for, try_lock_until).
//  6. Can be used with condition variables.

// lock_guard:
// 1. Simpler and more efficient than unique_lock.
// 2. Automatically locks the mutex when created and unlocks when destroyed.
// 3. Cannot be unlocked manually (only unlocks when it goes out of scope).
// 4. Cannot be moved or copied.
// 5. Does not support time-constrained attempts at locking.
// 6. Cannot be used with condition variables.