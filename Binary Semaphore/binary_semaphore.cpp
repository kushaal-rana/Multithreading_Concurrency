/*
    Binary Semaphore is used for singnalling and waiting. One thread should signal other thread to start after it has completed its work.
    Mutex is for ownership of the resource.
*/

#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
using namespace std;

binary_semaphore signalMainToThread{0}, signalThreadToMain{0}; // 0 means blocked

void ThreadProc()
{

    signalMainToThread.acquire();
    cout << "[Thread] Thread Got the Signal\n";

    this_thread::sleep_for(chrono::seconds(2));
    cout << "Thread Sending the Signal" << endl;
    signalThreadToMain.release();
}

int main()
{
    thread worker(ThreadProc);
    cout << "[Main] Send the Signal" << endl;
    signalMainToThread.release();

    signalThreadToMain.acquire();
    cout << "[Main] Got the Signalf" << endl;
    worker.join();
    return 0;
}