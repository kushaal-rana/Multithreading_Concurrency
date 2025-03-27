// Approach 1: Using Mutex and Condition Variable
/*Some Questions -
How does this solution ensure proper synchronization?
"We use a mutex to prevent race conditions and a condition_variable to ensure that hydrogen and oxygen atoms wait for each other before forming water molecules."

Why use unique_lock instead of lock_guard?
"unique_lock allows us to unlock the mutex while waiting ( cv.wait() ), whereas lock_guard does not support unlocking."

What happens when an oxygen thread arrives?
"It waits until exactly 2 hydrogen atoms are ready before proceeding. This ensures that molecules form in correct proportions."

If the thread is locked then how hydrogen_count is incremented
"When a thread calls cv.wait(lock, condition), two things happens the mutex (lock) is released, allowing other threads to acquire it and the thread enters a waiting state (paused) until another thread calls cv.notify_one() or cv.notify_all()."*/
class H2O
{
public:
    mutex mtx;
    condition_variable cv;
    int hydrogen_count;
    H2O()
    {
        hydrogen_count = 0;
    }

    void hydrogen(function<void()> releaseHydrogen)
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]
                { return hydrogen_count < 2; });
        // releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen();
        hydrogen_count++;
        cv.notify_all();
    }

    void oxygen(function<void()> releaseOxygen)
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]
                { return hydrogen_count == 2; });
        // releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen();
        hydrogen_count = 0;
        cv.notify_all();
    }
};
