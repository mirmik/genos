#ifndef IGRIS_SYNC_EVENT_H
#define IGRIS_SYNC_EVENT_H

// https://elweitzel.de/drupal/?q=node/6

#include <chrono>
#include <condition_variable>
#include <mutex>

namespace igris
{
    class event
    {
    private:
        bool m_bFlag = false;
        mutable std::mutex m_mutex = {};
        mutable std::condition_variable m_condition = {};

    public:
        inline void wait() const
        {
            std::unique_lock<std::mutex> _lock(m_mutex);
            m_condition.wait(_lock, [&]() -> bool { return m_bFlag; });
        }

        template <typename R, typename P>
        bool wait(const std::chrono::duration<R, P> &crRelTime) const
        {
            std::unique_lock<std::mutex> _lock(m_mutex);
            if (!m_condition.wait_for(_lock, crRelTime,
                                      [&]() -> bool { return m_bFlag; }))
                return false;
            return true;
        }

        inline bool signal()
        {
            bool bWasSignalled;
            m_mutex.lock();
            bWasSignalled = m_bFlag;
            m_bFlag = true;
            m_mutex.unlock();
            m_condition.notify_all();
            return bWasSignalled == false;
        }

        inline bool reset()
        {
            bool bWasSignalled;
            m_mutex.lock();
            bWasSignalled = m_bFlag;
            m_bFlag = false;
            m_mutex.unlock();
            return bWasSignalled;
        }

        inline bool isset() const { return m_bFlag; }
    };
}

#endif
