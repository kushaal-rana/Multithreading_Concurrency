// There are 5 types we can create threads in C++.
// 1. Using Function.
// 2. Using Lambda.
// 3. Using Member Function.
// 4. Using Static Member Function.
// Note: If we create multiple threads at the same time it doesn't gurantee which one will start first.

#include <iostream>
#include <thread>
using namespace std;

// This is Function Pointer: A very basic form of thread creation
// void func(int x)
// {
//     while (x--)
//     {
//         cout << x << endl;
//     }
// }

// int main()
// {
//     thread t1(func, 11);
//     t1.join();
//     return 0;
// }

// 2. Lambda Function
int main()
{
    int count = 10;
    thread t1([&]()
              {
        for (int i = 0; i < count; i++)
        {
            cout << i << endl;
        } });
    t1.join();
    return 0;
}

// 3. Using Member Function
class Demo
{
public:
    void display()
    {
        for (int i = 0; i < 10; i++)
        {
            cout << i << endl;
        }
    }
};
int main()
{
    Demo d;
    thread t1(&Demo::display, &d);
    t1.join();
    return 0;
}

// 4. Static Member Function
class Demo
{
public:
    static void display()
    {
        for (int i = 0; i < 10; i++)
        {
            cout << i << endl;
        }
    }
};
int main()
{
    thread t1(&Demo::display);
    t1.join();
    return 0;
}