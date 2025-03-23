#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

class ProducerConsumer
{
private:
    mutex mtx;
    condition_variable cv;
    deque<int> buffer;
    int maxBufferSize;
    bool done;

public:
    ProducerConsumer(int maxBufferSize) : maxBufferSize(maxBufferSize), done(false)
    {
        // this->maxBufferSize = maxBufferSize;
    }
    void produce(int val)
    {
        while (val--)
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]
                    { return buffer.size() < maxBufferSize; });
            buffer.push_front(val);
            cout << "Producer: " << val << endl;
            lock.unlock();
            cv.notify_one();
            {
                if (buffer.size() == maxBufferSize)
                {
                    unique_lock<mutex> lock(mtx);
                    done = true;
                    cv.notify_all();
                }
            }
        }
    }
    void consumer()
    {
        while (true)
        {

            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]
                    { return buffer.size() > 0 || done; });

            if (done && buffer.empty())
                return; // Exit the loop if production is done and buffer is empty

            int val = buffer.front();
            buffer.pop_front();
            cout << "Consumer: " << val << endl;
            lock.unlock();
            cv.notify_one();
        }
    }
};

int main()
{
    ProducerConsumer pc(10);
    thread t1(&ProducerConsumer::produce, &pc, 10);
    thread t2(&ProducerConsumer::consumer, &pc);
    t1.join();
    t2.join();
    return 0;
}