/*
Ptoblem: A Bathroom is being designed for the use of both males and females in a office, but requires the following constraint to maintain:
1. They cannot be men and woman in the bathroom at the same time.
2. There should never be more than three employees in the bathroom simultaneously.

Here we avoid starvation and introduce fairness
Starvation can occur if threads of one gender continuously occupy the bathroom, preventing threads of the other gender from ever getting a chance to use it.

To solve the problem of starvation, you can introduce a fairness mechanism that ensures that threads of both genders get a fair chance to use the bathroom. One way to achieve this is by using additional flags or counters to track the waiting threads of each gender and give priority to the gender that has been waiting longer.
This is achieved by using additional counters to track the number of waiting males and females and modifying the condition in cv.wait to give priority to the gender that has been waiting longer.
*/
// The mutex ensures that only one thread modifies the shared state at a time.
// The cv.wait condition allows multiple males to proceed one by one as long as the bathroom is not full and the gender matches.
// After a male thread enters the bathroom, it releases the lock (lock.unlock();), allowing the next male thread to acquire the lock and proceed.

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;
class UnisexBathroom
{
    int counter = 0;
    char gender = 'N'; // 'N' for none, 'M' for male, 'F' for female
    mutex mtx;
    condition_variable cv;
    int waitingMales = 0;
    int waitingFemales = 0;

public:
    void maleUseBathroom(string name)
    {
        unique_lock<mutex> lock(mtx);
        waitingMales++;
        cv.wait(lock, [&]
                { return (counter < 3 && (gender == 'M' || gender == 'N')) && (waitingFemales == 0 || gender == 'M'); });
        waitingMales--;
        counter++;
        cout << name << " Entered the bathroom." << endl;
        gender = 'M';
        lock.unlock();

        cout << "Made use of the bathroom by " << name << endl;

        lock.lock();
        counter--;
        if (counter == 0)
            gender = 'N';
        cout << name << " Exited the bathroom." << endl;
        lock.unlock();

        cv.notify_all();
    }

    void femaleUseBathroom(string name)
    {
        unique_lock<mutex> lock(mtx);
        waitingFemales++;
        cv.wait(lock, [&]
                { return (counter < 3 && (gender == 'F' || gender == 'N')) && (waitingMales == 0 || gender == 'F'); });
        counter++;
        waitingFemales--;
        cout << name << " Entered the bathroom." << endl;
        gender = 'F';
        lock.unlock();

        cout << "Made use of the bathroom by " << name << endl;

        lock.lock();
        counter--;
        if (counter == 0)
        {
            gender = 'N'; // Reset the gender when no one is in the bathroom
        }
        cout << name << " Exited the bathroom." << endl;
        lock.unlock();

        cv.notify_all();
    }
};

void maleUseBathroom(UnisexBathroom *bathroom, string name)
{
    bathroom->maleUseBathroom(name);
}

void femaleUseBathroom(UnisexBathroom *bathroom, string name)
{
    bathroom->femaleUseBathroom(name);
}

int main()
{
    UnisexBathroom *bathroom = new UnisexBathroom();
    thread t1(maleUseBathroom, bathroom, "Man1");
    thread t2(maleUseBathroom, bathroom, "Man2");
    thread t3(femaleUseBathroom, bathroom, "Woman1");
    thread t4(femaleUseBathroom, bathroom, "Woman2");
    thread t5(femaleUseBathroom, bathroom, "Woman3");
    thread t6(maleUseBathroom, bathroom, "Man3");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    delete bathroom;
    return 0;
}