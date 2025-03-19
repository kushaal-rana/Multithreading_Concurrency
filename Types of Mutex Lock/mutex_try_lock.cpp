// Mutex: try_lock() is a member function of the mutex class
// Normally in mutex directly it will lock but if another thread tries to lock it will be blocked as mutex is already locked
// try_lock() Tries to lock the mutex. It is used to check if the lock is available without blocking the current thread. RETURNS IMMEDIATELY. On successful lock acquisition, it returns true if the lock is available, false otherwise. (This is nonBlocking). If it is called again and again by the same thread, which owns the Mutex is undefined, it is a deadlock situation with undefined behavior (if you want to be able to lock the same Mutex by the same thread more than one time then go for recrusive Mutex).

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
int counter = 0;
mutex mtx;
void increaseCounterFor10000Times()
{
    for (int i = 0; i < 10; i++)
    {
        if (mtx.try_lock())
        {
            // mtx.lock();
            counter++;
            cout << this_thread::get_id() << " " << counter << endl;
            mtx.unlock();
        }
    }
}
int main()
{
    thread t1(increaseCounterFor10000Times);
    thread t2(increaseCounterFor10000Times);
    t1.join();
    t2.join();
    cout << "Count: " << counter << endl;
    // The try_lock() function attempts to lock the mutex without blocking, and if it fails to acquire the lock, it returns immediately. This means that some increments of count might be missed if the mutex is already locked by another thread.

    return 0;
}