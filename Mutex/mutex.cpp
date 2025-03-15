// Mutex: Mutual Exclusion
// Mutual Exclusion means that only one thread can access a resource at a time.
// mutexes are used to ensure that only one thread can access a shared resource at a time, such as a file or a database.

// Race Conditon:

// The race condition occurs when two or more threads access a shared resource concurrently, and the behavior of the program depends on the order in which the threads access the resource.

// To avoid race conditions, we can use mutexes to ensure that only one thread can access a shared resource at a time.
// If there is a race condition then we have to protect it and the protected section is called critical section/region

// MUTEX:
// Mutex is used to avoid race condtion
// We use lock(), unlock() on mutex to avoid race condition

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int myAmount = 0;
mutex m;

void addMoney()
{
    m.lock();
    ++myAmount;
    cout << "Amount: " << myAmount << endl;
    m.unlock();
}
int main()
{
    thread t1(addMoney);
    thread t2(addMoney);
    thread t3(addMoney);
    t1.join();
    t2.join();
    t3.join();
    cout << "Amount: " << myAmount << endl;
    return 0;
}