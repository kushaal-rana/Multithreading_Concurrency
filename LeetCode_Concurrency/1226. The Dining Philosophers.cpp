// https://leetcode.com/problems/dining-philosophers/
// Approach: Using Mutex
class DiningPhilosophers
{
private:
    mutex forks[5];
    mutex eating;

public:
    DiningPhilosophers()
    {
    }

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork)
    {
        int left_fork = philosopher;
        int right_fork = (philosopher + 1) % 5;
        // to avoid deadlock, we'll pick up forks in specific order
        unique_lock<mutex> lock(eating);
        if (left_fork < right_fork)
        {
            forks[left_fork].lock();
            pickLeftFork();
            forks[right_fork].lock();
            pickRightFork();
        }
        else
        {
            forks[right_fork].lock();
            pickRightFork();
            forks[left_fork].lock();
            pickLeftFork();
        }
        // Now philosopher has both forks and can eat
        eat();

        putLeftFork();
        forks[left_fork].unlock();
        putRightFork();
        forks[right_fork].unlock();
    }
};

// Approach 2: Using Semaphore
class Semaphore
{
private:
    mutex mtx;
    condition_variable cv;
    int count;

public:
    Semaphore() {}
    Semaphore(int c) : count(c) {};
    void setCount(int a)
    {
        count = a;
    }
    void signal()
    {
        unique_lock<mutex> lock(mtx);
        count++;
        cv.notify_one();
    }
    void wait()
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]
                { return count > 0; });
        count--;
    }
};
class DiningPhilosophers
{
    Semaphore sem;
    mutex mtx[5];

public:
    DiningPhilosophers()
    {
        sem.setCount(4);
    }

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork)
    {
        int left = philosopher;
        int right = (philosopher + 1) % 5;

        sem.wait();

        unique_lock<mutex> lckLeft(mtx[left]);
        unique_lock<mutex> lckRight(mtx[right]);
        pickLeftFork();
        pickRightFork();

        eat();

        putLeftFork();
        putRightFork();
        // lckRight.unlock();
        // lckLeft.unlock();
        sem.signal(); // finish eating, release 1 spot for other philosophers
    }
};

// Working: If Philosopher 0 is eating, they hold forks 0 and 1
// This means Philosophers 1 and 4 cannot eat(as they need forks 1 and 0 respectively) But Philosophers 2 and 3 could potentially eat simultaneously with Philosopher 0, as they use different forks