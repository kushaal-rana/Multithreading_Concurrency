// Example 2: With loop
#include <iostream>

#include <thread>
#include <mutex>
using namespace std;

std::recursive_mutex m1;

int main()
{
    int loopFor = 5;

    for (int i = 0; i <= loopFor; i++)
    {
        m1.lock();
        cout << "Lock N°" << i << endl;
    }

    for (int i = 0; i <= loopFor; i++)
    {
        m1.unlock();
        cout << "Unlock N°" << i << endl;
    }
    return 0;
}