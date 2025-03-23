class ZeroEvenOdd
{
private:
    int n;
    condition_variable cv;
    mutex mtx;
    int count;
    int i;

public:
    ZeroEvenOdd(int n)
    {
        this->n = n;
        count = 1;
        i = 1;
    }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber)
    {
        for (int j = 0; j < n; j++)
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&]
                    { return count == 1; });
            printNumber(0);
            count = (i % 2 == 0) ? 2 : 3;
            cv.notify_all();
        }
    }

    void even(function<void(int)> printNumber)
    {
        for (int j = 0; j < n / 2; ++j)
        {

            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&]
                    { return count == 2; });
            printNumber(i);
            i++;
            count = 1;
            cv.notify_all();
        }
    }

    void odd(function<void(int)> printNumber)
    {
        for (int j = 0; j < (n + 1) / 2; ++j)
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [&]
                    { return count == 3; });
            printNumber(i);
            i++;
            count = 1;
            cv.notify_all();
        }
    }
};