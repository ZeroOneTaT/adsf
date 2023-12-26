/*
 * @Author: ZeroOneTaT
 * @Date: 2023-12-20 20:15:03
 * @LastEditTime: 2023-12-26 20:18:56
 * @FilePath: /adsf/source/sync/mutex.cc
 * @Description: 自定义锁相关类实现代码
 *
 * Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#include "include/utils/fiber.hpp"
#include "include/utils/io_manager.hpp"
#include "include/sync/mutex.hpp"

namespace adsf
{
    bool CoMutex::tryLock()
    {
        return m_mutex.tryLock();
    }

    void CoMutex::lock()
    {
        // 本协程已经执有锁,退出
        if (Fiber::GetFiberId() == m_fiber_id)
            return;

        // 尝试获取锁
        while (!tryLock())
        {
            // 加锁保护队列
            m_gaurd.lock();
            // 由于进入等待队列和出队的代价比较大，所以再次尝试获取锁，成功获取锁将m_fiberId改成自己的id
            if (tryLock())
            {
                m_fiber_id = Fiber::GetFiberId();
                m_gaurd.unlock();
                return;
            }

            // 尝试获取锁失败，将自己加入等待队列
            // 获取所在的协程
            Fiber::ptr self = Fiber::GetThis();
            // 将自己加入协程等待队列
            m_wait_queue.push(self);
            m_gaurd.unlock();
            // 让出协程
            Fiber::YieldToHold();
        }
        // 成功获取锁，将m_fiberId改成自己的id
        m_fiber_id = GetFiberId();
    }

    void CoMutex::unlock()
    {
        m_gaurd.lock();
        m_fiber_id = 0;

        Fiber::ptr fiber;
        if (!m_wait_queue.empty())
        {
            // 获取等待线程
            fiber = m_wait_queue.front();
            m_wait_queue.pop();
        }

        m_mutex.unlock();
        m_gaurd.unlock();
        if (fiber)
        {
            // 将等待的协程重新加入调度
            go fiber;
        }
    }
}