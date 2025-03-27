// Approach 1: Using Mutex and Condition Variable
/*
Using a single condition variable (cv) instead of separate not_full and not_empty works but is less efficient because it notifies all threads instead of only the necessary ones. Here’s why:

Issue with notify_all()
In enqueue(), after adding an element, cv.notify_all() wakes up all waiting threads, including producers and consumers.

In dequeue(), after removing an element, cv.notify_all() again wakes up all waiting threads.

This leads to unnecessary context switching and overhead.

Why Two Condition Variables (not_full, not_empty) Are Better
not_full.wait() ensures that only producers wait when the queue is full.

not_empty.wait() ensures that only consumers wait when the queue is empty.

notify_one() ensures only the relevant thread (either a producer or consumer) is woken up, reducing CPU overhead.
*/
class BoundedBlockingQueue
{
public:
    queue<int> q;
    int sizee;
    mutex mtx;
    condition_variable cv;
    BoundedBlockingQueue(int capacity) : sizee(capacity)
    {
    }

    void enqueue(int element)
    {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [this]
                { return q.size() < sizee; });

        q.push(element);

        cv.notify_all();
    }

    int dequeue()
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]
                { return !q.empty(); });
        int ele = q.front();
        q.pop();
        lock.unlock();

        cv.notify_all();
        return ele;
    }

    int size()
    {
        unique_lock<mutex> lock(mtx);
        return q.size();
    }
};
/*
Approach : Single cv with notify_all()
Efficiency : ❌ Inefficient (wakes up all waiting threads)
Correctness: Correct
Issue: Wakes up unnecessary threads, causing unnecessary context switches

Approach : Single cv with notify_one()
Efficiency : ✅ More efficient than notify_all()
Correctness: ✅ Correct but can lead to minor inefficiencies
Issue: May wake up the wrong thread (producer when queue is full or consumer when queue is empty), requiring it to go back to waiting

Approach : Two condition variables (not_full & not_empty)
Efficiency : ✅✅ Most efficient
Correctness: ✅ Most correct
Issue: Ensures that only the relevant thread is woken up

Why notify_one() is Still Slightly Less Efficient
Even though notify_one() wakes up only one thread instead of all, it does not differentiate between a producer and a consumer.

Suppose the queue is empty, and a consumer is waiting on cv.wait(lock, [&] { return !q.empty(); }).

When a producer enqueues an element and calls cv.notify_one(), it might wake up another producer instead of the waiting consumer.

That producer will check the condition q.size() < sizee, find that it's true, and go back to waiting.

Meanwhile, the consumer is still waiting, causing a small delay.

With two condition variables, this does not happen because:

not_empty.notify_one() only wakes up a consumer when an item is available.
not_full.notify_one() only wakes up a producer when space is available.
*/

#include <queue>
#include <mutex>
#include <condition_variable>

class BoundedBlockingQueue {
private:
    std::queue<int> q;
    int capacity;
    std::mutex mtx;
    std::condition_variable not_full, not_empty;

public:
    BoundedBlockingQueue(int capacity) : capacity(capacity) {}

    void enqueue(int element) {
        std::unique_lock<std::mutex> lock(mtx);
        not_full.wait(lock, [this] { return q.size() < capacity; }); // Wait if full
        q.push(element);
        not_empty.notify_one(); // Notify one waiting consumer
    }

    int dequeue() {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [this] { return !q.empty(); }); // Wait if empty
        int val = q.front();
        q.pop();
        not_full.notify_one(); // Notify one waiting producer
        return val;
    }

    int size() {
        std::lock_guard<std::mutex> lock(mtx);
        return q.size();
    }
};

