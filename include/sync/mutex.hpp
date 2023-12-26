/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-19 20:08:11
 * @LastEditTime: 2023-12-19 20:08:13
 * @FilePath: /adsf/include/sync/mutex.hpp
 * @Description: 自定义锁相关类头文件
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef INCLUDE_SYNC_MUTEX_HPP_
#define INCLUDE_SYNC_MUTEX_HPP_
#include <semaphore.h>
#include <pthread.h>
#include <atomic>
#include <memory>
#include <queue>

#include "../utils/noncopyable.hpp"

namespace adsf
{

    /**
     * @brief RAII 锁包装
     *
     */
    template <class T>
    class ScopedLock
    {
    public:
        ScopedLock(T &mutex) : m_mutex(mutex)
        {
            m_mutex.lock();
            m_locked = true;
        }
        ~ScopedLock()
        {
            unlock();
        }
        void lock()
        {
            if (!m_locked)
            {
                m_mutex.lock();
                m_locked = true;
            }
        }
        bool tryLock()
        {
            if (!m_locked)
            {
                m_locked = m_mutex.tryLock();
            }
            return m_locked;
        }
        void unlock()
        {
            if (m_locked)
            {
                m_mutex.unlock();
                m_locked = false;
            }
        }

    private:
        T &m_mutex;
        bool m_locked = false;
    };

    /**
     * @brief 自旋锁
     *
     */
    class SpinLock : Noncopyable
    {
    public:
        using Lock = ScopedLock<SpinLock>;
        SpinLock()
        {
            pthread_spin_init(&m_spinlock, 0);
        }
        ~SpinLock()
        {
            pthread_spin_destroy(&m_spinlock);
        }
        void lock()
        {
            pthread_spin_lock(&m_spinlock);
        }
        bool tryLock()
        {
            return pthread_spin_trylock(&m_spinlock) == 0;
        }
        void unlock()
        {
            pthread_spin_unlock(&m_spinlock);
        }

    private:
        pthread_spinlock_t m_spinlock;
    };

    /**
     * @brief 互斥量
     *
     */
    class Mutex : Noncopyable
    {
    public:
        using Lock = ScopedLock<Mutex>;
        Mutex()
        {
            pthread_mutex_init(&m_mutex, nullptr);
        }
        ~Mutex()
        {
            pthread_mutex_destroy(&m_mutex);
        }
        void lock()
        {
            pthread_mutex_lock(&m_mutex);
        }
        bool tryLock()
        {
            return pthread_mutex_trylock(&m_mutex) == 0;
        }
        void unlock()
        {
            pthread_mutex_unlock(&m_mutex);
        }

    private:
        pthread_mutex_t m_mutex;
    };

    /**
     * @brief 协程锁类
     *
     * 这个类提供了一种用于协程之间的同步机制。它使用了 SpinLock 作为底层锁，并维护了一个协程等待队列。
     * 当一个协程尝试获取已被其他协程持有的锁时，该协程会被添加到等待队列中，直到锁被释放。
     */

    class Fiber; // 假设 Fiber 是协程类，这里只是声明，实际定义在其他地方

    class CoMutex : Noncopyable
    {
    public:
        using Lock = ScopedLock<CoMutex>;

        bool lock();
        bool tryLock();
        bool unlock();

    private:
        SpinLock m_mutex;                                // 协程所持有的锁
        SpinLock m_gaurd;                                // 保护等待队列的锁
        uint64_t m_fiber_id;                             // 协程 ID
        std::queue<std::shared_ptr<Fiber>> m_wait_queue; // 协程等待队列
    };

    /**
     * @brief 读锁 RAII 包装
     */
    template <class T>
    class ReadScopedLock
    {
    public:
        ReadScopedLock(T &mutex) : m_mutex(mutex)
        {
            m_mutex.rlock();
            m_locked = true;
        }
        ~ReadScopedLock()
        {
            unlock();
        }
        void lock()
        {
            if (!m_locked)
            {
                m_mutex.rlock();
                m_locked = true;
            }
        }
        void unlock()
        {
            if (m_locked)
            {
                m_mutex.unlock();
                m_locked = false;
            }
        }

    private:
        T &m_mutex;
        bool m_locked = false;
    };

    /**
     * @brief 写锁 RAII 包装
     */
    template <class T>
    class WriteScopedLock
    {
    public:
        WriteScopedLock(T &mutex) : m_mutex(mutex)
        {
            m_mutex.wlock();
            m_locked = true;
        }
        ~WriteScopedLock()
        {
            unlock();
        }
        void lock()
        {
            if (!m_locked)
            {
                m_mutex.wlock();
                m_locked = true;
            }
        }
        void unlock()
        {
            if (m_locked)
            {
                m_mutex.unlock();
                m_locked = false;
            }
        }

    private:
        T &m_mutex;
        bool m_locked = false;
    };

    /**
     * @brief 读写锁
     */
    class RWMutex : Noncopyable
    {
    public:
        using ReadLock = ReadScopedLock<RWMutex>;
        using WriteLock = WriteScopedLock<RWMutex>;
        RWMutex()
        {
            pthread_rwlock_init(&m_rwmutex, nullptr);
        }
        ~RWMutex()
        {
            pthread_rwlock_destroy(&m_rwmutex);
        }
        void rlock()
        {
            pthread_rwlock_rdlock(&m_rwmutex);
        }
        void wlock()
        {
            pthread_rwlock_wrlock(&m_rwmutex);
        }
        void unlock()
        {
            pthread_rwlock_unlock(&m_rwmutex);
        }

    private:
        pthread_rwlock_t m_rwmutex;
    };
}

#endif // INCLUDE_SYNC_MUTEX_HPP_