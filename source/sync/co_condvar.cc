/*
 * @Author: ZeroOneTaT
 * @Date: 2023-12-26 19:39:34
 * @LastEditTime: 2023-12-26 20:31:48
 * @FilePath: /adsf/source/sync/co_condvar.cc
 * @Description: 协程条件变量实现
 *
 * Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#include "include/utils/fiber.hpp"
#include "include/utils/io_manager.hpp"
#include "include/sync/co_condvar.hpp"

namespace adsf
{
    void CoCondVar::notify()
    {
        Fiber::ptr fiber;
        // 减小锁的粒度
        {
            // 获取等待协程
            MutexType::Lock lock(m_mutex);
            if (m_wait_queue.empty())
                return;
            fiber = m_wait_queue.front();
            m_wait_queue.pop();
            // 若存在定时器，将其删除
            if (m_timer)
            {
                m_timer->cancel();
                m_timer = nullptr;
            }
        }
        // 将协程重新加入调度
        if (fiber)
            go fiber;
    }

    void CoCondVar::broadcast()
    {
        MutexType::Lock lock(m_mutex);
        // 将等待队列中的所有协程重新加入调度
        while (m_wait_queue.size())
        {
            Fiber::ptr fiber = m_wait_queue.front();
            m_wait_queue.pop();
            if (fiber)
                go fiber;

            // 若存在定时器，将其删除
            if (m_timer)
            {
                m_timer->cancel();
                m_timer = nullptr;
            }
        }
    }

    void CoCondVar::wait()
    {
        Fiber::ptr self = Fiber::GetThis();
        // 将当前协程加入等待队列
        m_wait_queue.push(self);
        if (!m_timer)
        {
            // 加入一个空任务定时器，不让调度器退出
            m_timer = IOManager::GetThis()->addTimer(
                -1, [] {}, true);
        }
        // 让出协程
        Fiber::YieldToHold();
    }

    void CoCondVar::wait(CoMutex::Lock &lock)
    {
        Fiber::ptr self = Fiber::GetThis();
        {
            MutexType::Lock lock1(m_mutex);
            // 将当前协程加入等待队列
            m_wait_queue.push(self);
            if (!m_timer)
            {
                // 加入一个空任务定时器，不让调度器退出
                m_timer = IOManager::GetThis()->addTimer(
                    -1, [] {}, true);
            }
        }
        // 先解锁
        lock.unlock();
        // 让出协程
        Fiber::YieldToHold();
        // 重新获取锁
        lock.lock();
    }
}