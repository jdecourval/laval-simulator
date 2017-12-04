#include "thread_pool.h"


Thread::Thread()
{
    running = true;
    done = false;
    ready = false;
    exception = nullptr;

    thread = std::thread{[this]()
    {
        while (running)
        {
            std::unique_lock lock(mutex);
            cv.wait(lock, [this] { return ready; });

            if (!running)
            {
                break;
            }

            try
            {
                work();
            }
            catch (...)
            {
                exception = std::current_exception();
            }

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

    auto thread_exception = std::exception_ptr();
    std::swap(exception, thread_exception);
    if (thread_exception)
    {
        std::rethrow_exception(thread_exception);
    }
}

void Thread::stop()
{
    running = false;

    {
        std::lock_guard lock(mutex);
        ready = true;
    }

    cv.notify_one();
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
