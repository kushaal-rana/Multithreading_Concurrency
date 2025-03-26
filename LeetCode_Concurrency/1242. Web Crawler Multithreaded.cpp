/**
 * // This is the HtmlParser's API interface.
 * // You should not implement it, or speculate about its implementation
 * class HtmlParser {
 *   public:
 *     vector<string> getUrls(string url);
 * };
 */
class Solution
{
private:
    string getHostName(string url)
    {
        stringstream ss(url);
        string protocol, hostname;

        getline(ss, protocol, '/');
        getline(ss, protocol, '/');
        getline(ss, hostname, '/');

        return hostname;
    }
    int thread_num;
    queue<string> q;
    unordered_set<string> visited;
    int working = 0;
    bool done;
    mutex mtx;
    condition_variable cv;
    vector<thread> workers;
    string hostName;

    // this a worker thread that will be doing tasks.
    void startWorker(HtmlParser htmlParser)
    {
        while (true)
        {
            unique_lock<mutex> lock(mtx);
            // wait untill there are some tasks or we are done executing
            cv.wait(lock, [&]
                    { return !q.empty() || done; }); // or insted of this working==0 also works

            // if Done return
            if (done || q.empty())
            {
                return;
            }
            // indicate that this thread is in progress: working
            working++;
            string url = q.front();
            q.pop();
            // here result array likh baad mai
            lock.unlock();

            // since getUrls can take a lot of time, release the lock.
            vector<string> urls = htmlParser.getUrls(url);

            // acquire the lock and add tasks.
            lock.lock();
            for (auto &nextUrl : urls)
            {
                // if it has been seen already or the host name doesn't match, ignore it.
                if (visited.count(nextUrl) == 0 && getHostName(nextUrl) == hostName)
                {
                    q.push(nextUrl);
                    visited.insert(nextUrl);
                }
            }
            working--;

            // IF
            //   1) no thread is processing
            //   2) no tasks are available even after executing this task
            // THEN we are done.
            if (working == 0 && q.empty())
            {
                done = true;
            }

            // notify all the threads either about finishing or about availability of tasks.
            cv.notify_all();
        }
    }

public:
    vector<string> crawl(string startUrl, HtmlParser htmlParser)
    {
        thread_num = thread::hardware_concurrency();

        hostName = getHostName(startUrl);
        q.push(startUrl);
        visited.insert(startUrl);
        done = false;

        // start bunch of worker threads.
        for (int i = 0; i < thread_num; i++)
        {
            workers.emplace_back(&Solution::startWorker, this, htmlParser);
        }

        for (auto &worker : workers)
        {
            worker.join();
        }

        return vector<string>(visited.begin(), visited.end());
    }
};
