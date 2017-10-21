#include "thread_pool.h"


Thread::Thread()
{
    running = true;
    done = false;
    ready = false;

    thread = std::thread{[this]()
    {
        while (running)
        {
            std::unique_lock lock(mutex);
            cv.wait(lock, [this] { return ready; });

            work();

            done = true;
            ready = false;
            lock.unlock();
            cv.notify_one();
        }
    }};
}

Thread::~Thread()
{
    stop();
    thread.join();
}

void Thread::stop()
{
    running = false;
}

void Thread::join()
{
    std::unique_lock lock(mutex);
    cv.wait(lock, [this] { return done; });
    done = false;
}

Thread& Thread::operator=(std::function<void()>&& function)
{
    {
        std::lock_guard lock(mutex);
        work = std::move(function);
        ready = true;
    }

    cv.notify_one();

    return *this;
}
