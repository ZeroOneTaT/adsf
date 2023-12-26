/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-20 20:48:25
 * @LastEditTime: 2023-12-20 20:48:26
 * @FilePath: /adsf/include/sync/co_semaphore.hpp
 * @Description: 自定义信号量类定义
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef INCLUDE_SYNC_CO_SEMAPHORE_HPP_
#define INCLUDE_SYNC_CO_SEMAPHORE_HPP_

#include <atomic>
#include <condition_variable>
#include <mutex>

#include "co_condvar.hpp"

namespace adsf
{
    /**
     * @brief 协程信号量
     */
    class CoSemaphore : Noncopyable
    {
    public:
        CoSemaphore(uint32_t num)
        {
            m_num = num;
            m_used = 0;
        }

        void wait()
        {
            CoMutex::Lock lock(m_mutex);
            // 如果已经获取的信号量大于等于信号量数量则让出协程等待
            while (m_used >= m_num)
            {
                m_condvar.wait(lock);
            }
            ++m_used;
        }

        void notify()
        {
            CoMutex::Lock lock(m_mutex);
            if (m_used > 0)
            {
                --m_used;
            }
            // 通知一个等待的协程
            m_condvar.notify();
        }

    private:
        uint32_t m_num;      // 信号量数目
        uint32_t m_used;     // 信号量已使用数目
        CoCondVar m_condvar; // 协程条件变量
        CoMutex m_mutex;     // 协程互斥锁
    };

    class Semaphore : Noncopyable
    {
    public:
        Semaphore(uint32_t count)
        {
            if (sem_init(&m_sem, 0, count))
            {
                throw std::logic_error("sem_init error");
            }
        }
        ~Semaphore()
        {
            sem_destroy(&m_sem);
        }
        void wait()
        {
            if (sem_wait(&m_sem))
            {
                throw std::logic_error("sem_wait error");
            }
        }
        void notify()
        {
            if (sem_post(&m_sem))
            {
                throw std::logic_error("sem_notify error");
            }
        }

    private:
        sem_t m_sem;
    };
}

#endif // INCLUDE_SYNC_CO_SEMAPHORE_HPP_