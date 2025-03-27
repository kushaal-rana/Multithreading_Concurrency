/*
Ptoblem: A Bathroom is being designed for the use of both males and females in a office, but requires the following constraint to maintain:
1. They cannot be men and woman in the bathroom at the same time.
2. There should never be more than three employees in the bathroom simultaneously.
*/

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

public:
    void maleUseBathroom(string name)
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]
                { return counter < 3 && gender == 'M' || gender == 'N'; });
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
        cv.wait(lock, [&]
                { return counter < 3 && gender == 'F' || gender == 'N'; });
        counter++;
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

// Starvation is Still Possible

// If multiple male threads arrive consecutively, they will keep entering the bathroom because the condition allows them to go in as long as gender == 'M' or the bathroom is empty (gender == 'N').

// This means that if a female arrives while males keep coming in, she will never get a chance.

// The same issue happens if females keep arriving first—males may be locked out indefinitely.

// No Consideration for Waiting Males/Females

// There is still no mechanism to track waiting males or females and prioritize the gender that has been waiting longer.

// If there is a mix of males and females waiting, the bathroom will always favor the first one that arrives, leading to starvation for the other gender.

// Bathroom Switch Only When Empty

// The bathroom only resets (gender = 'N') when the last person leaves.

// But if one gender continuously occupies the bathroom without giving the other gender a chance, the waiting gender may never get access.