// Approach 1 : Using Mutex and Condition Variable
class Foo
{
public:
    condition_variable cv;
    mutex mtx;
    int nextPrint;
    Foo()
    {
        nextPrint = 1;
    }

    void first(function<void()> printFirst)
    {
        unique_lock<mutex> lock(mtx);
        printFirst();
        nextPrint = 2;
        cv.notify_all();
        // printFirst() outputs "first". Do not change or remove this line.
    }

    void second(function<void()> printSecond)
    {
        unique_lock<mutex> ul(mtx);
        cv.wait(ul, [&]
                { return nextPrint == 2; });
        printSecond();
        nextPrint = 3;
        cv.notify_one();
        // printSecond() outputs "second". Do not change or remove this line.
    }

    void third(function<void()> printThird)
    {

        unique_lock<mutex> ul(mtx);
        cv.wait(ul, [&]
                { return nextPrint == 3; });
        printThird();
        // printThird() outputs "third". Do not change or remove this line.
    }
};