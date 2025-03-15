// 0. Once a thread is started, we will wait for this thread to finish by calling join() function on the thread object.
// 1. Double joint will result into program termination.
// 2. If needed, we should check thread is joinable before joining. (using joinable() function)

// Note:
//  0. This is used to detach newly Created thread from parent thread.
//  1. Always check before detaching a thread that it is joinable otherwise we may end up double detaching, and double detach() will result in program termination
//  2. If we have detached thread and main function is returning then the detached thread execution is suspended.

// Notes:
//  Either join() or detach() should be called on thread object, otherwise during thread object's destructor it will terminate the program.
//  Because inside destructor it checks if thread is still joinable? If yes then it terminates the program.

#include <iostream>
#include <thread>
using namespace std;

void run(int count)
{
    while (count-- > 0)
    {
        cout << count << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main()
{
    thread t1(run, 10);
    cout << "Thread starting " << endl;

    // t1.join();
    t1.detach();

    cout << "Thread Done " << endl;
    this_thread::sleep_for(chrono::seconds(5));
    return 0;
}