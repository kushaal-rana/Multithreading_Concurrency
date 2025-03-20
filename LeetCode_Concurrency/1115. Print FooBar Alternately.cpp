// Approach 1 : Using Mutex and Condition Variable
class FooBar
{
private:
    int n;
    mutex mtx;
    condition_variable cv;
    bool isFooPrinted;

public:
    FooBar(int n)
    {
        this->n = n;
        isFooPrinted = false;
    }

    void foo(function<void()> printFoo)
    {
        for (int i = 0; i < n; i++)
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&]
                    { return isFooPrinted == false; });
            printFoo();
            isFooPrinted = true;
            cv.notify_all();
            // printFoo() outputs "foo". Do not change or remove this line.
        }
    }

    void bar(function<void()> printBar)
    {
        for (int i = 0; i < n; i++)
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&]
                    { return isFooPrinted == true; });
            printBar();
            isFooPrinted = false;
            cv.notify_all();
            // printBar() outputs "bar". Do not change or remove this line.
        }
    }
};