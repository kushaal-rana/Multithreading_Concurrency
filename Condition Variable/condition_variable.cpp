/*
"Condition Variable In C++ Threading".
Few important points to remember while using "condition variables" are as follows:
1. Condition variables allow us to synchronise threads via notifications. They allow running threads to wait on some condition and once those conditions are met the waiting thread is notified using:
    a. notify_one();
    b. notify_all();
2. You need mutex to use condition variable
3. Condition variable is used to synchronise two or more threads.
4. Best use case of condition variable is Producer/Consumer problem.
5. Condition variables can be used for two purposes:
    a. Notify other threads
    b. Wait for some condition
6. If some thread want to wait on some condtion then it has to do:
    a. Acquire the mutex lock using std::unique_lock<std::mutex> lock(m);
    b. Execute wait, wait_for, or wait_until. The wait operations atomatically release the mutex and suspend the execution of the thread.
    c. When the condition variable is notifed, the thread is AWAKENED and the mutex is reacquired.
        The thread should then check the condition and resume waiting if the wake up was spurious.
7. Best use of Condition Varialbe is Producer/Consumer problem
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

condition_variable cv;
mutex m;
int balance = 0;

void deposite(int amount)
{
    lock_guard<mutex> lock(m);
    balance += amount;
    cout << "Deposited Amount Current Balance is: " << balance << endl;
    cv.notify_one();
}

void withdraw(int amount)
{ // if this method is called first it locks the mutex first then it unlocks waits and checks the condition if condition is true it will proceed otherwise it will wait
    unique_lock<mutex> ul(m);
    cv.wait(ul, []
            { return balance != 0 ? true : false; }); // unlock yaha automatically karega
    if (balance >= amount)
    {
        balance -= amount;
        cout << "Amount Deducted: " << amount << endl;
    }
    else
    {
        cout << "Amount Cannot be Deducted" << endl;
    }
    cout << "Current Balance: " << balance << endl;
}

int main()
{
    thread t2(withdraw, 500);
    thread t1(deposite, 600);
    t1.join();
    t2.join();
    return 0;
}