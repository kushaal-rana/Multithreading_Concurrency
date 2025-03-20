class FizzBuzz
{
private:
    int n;
    int i = 0;
    mutex mtx;
    condition_variable cv;

public:
    FizzBuzz(int n)
    {
        this->n = n;
        this->i = 1;
    }

    // printFizz() outputs "fizz".
    void fizz(function<void()> printFizz)
    {
        while (true)
        {
            unique_lock<mutex> lock(mtx);
            while (i <= n && (i % 3 != 0 || i % 5 == 0))
            { // opposite condition for wait
                cv.wait(lock);
            }
            if (i > n)
                return; // stop unecessary execution;
            // if correct condition
            printFizz();
            i++;
            cv.notify_all();
        }
    }

    // printBuzz() outputs "buzz".
    void buzz(function<void()> printBuzz)
    {
        while (true)
        {
            unique_lock<mutex> lock(mtx);
            while (i <= n && (i % 3 == 0 || i % 5 != 0))
            { // opposite condition for wait
                cv.wait(lock);
            }
            if (i > n)
                return; // stop unecessary execution;
            // if correct condition
            printBuzz();
            i++;
            cv.notify_all();
        }
    }

    // printFizzBuzz() outputs "fizzbuzz".
    void fizzbuzz(function<void()> printFizzBuzz)
    {
        while (true)
        {
            unique_lock<mutex> lock(mtx);
            while (i <= n && (i % 3 != 0 || i % 5 != 0))
            { // opposite condition for wait
                cv.wait(lock);
            }
            if (i > n)
                return; // stop unecessary execution;
            // if correct condition
            printFizzBuzz();
            i++;
            cv.notify_all();
        }
    }

    // printNumber(x) outputs "x", where x is an integer.
    void number(function<void(int)> printNumber)
    {
        while (true)
        {
            unique_lock<mutex> lock(mtx);
            while (i <= n && (i % 3 == 0 || i % 5 == 0))
            { // opposite condition for wait
                cv.wait(lock);
            }
            if (i > n)
                return; // stop unecessary execution;
            // if correct condition
            printNumber(i);
            i++;
            cv.notify_all();
        }
    }
};