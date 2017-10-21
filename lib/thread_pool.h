#ifndef PROJECT_THREADPOOL_H
#define PROJECT_THREADPOOL_H

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>


class Thread
{
public:
    Thread();

    ~Thread();

    void stop();

    void join();

    Thread& operator=(std::function<void()>&& function);


private:
    std::thread thread;
    std::mutex mutex;
    std::condition_variable cv;
    std::function<void()> work;
    bool ready;
    bool done;
    std::atomic<bool> running;
};


template <size_t Cores>
class ThreadPool
{
public:
    template<class InputIt, class UnaryFunction>
    void apply(InputIt first, InputIt last, UnaryFunction f);

private:
    std::array<Thread, Cores> cores;
};


#include "impl/thread_pool.hpp"
#endif //PROJECT_THREADPOOL_H
