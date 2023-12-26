/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-20 20:20:10
 * @LastEditTime: 2023-12-20 20:47:19
 * @FilePath: /adsf/include/sync/co_condvar.hpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */
/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-20 20:20:10
 * @LastEditTime: 2023-12-20 20:20:12
 * @FilePath: /adsf/include/sync/co_condvar.hpp
 * @Description: 协程条件变量类定义
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef INCLUDE_SYNC_CO_CONDVAR_HPP_
#define INCLUDE_SYNC_CO_CONDVAR_HPP_

#include "mutex.hpp"

namespace adsf
{
    /**
     * @brief 协程条件变量
     */
    class Fiber;
    class Timer;
    class CoCondVar : Noncopyable
    {
    public:
        using MutexType = SpinLock;
        /**
         * @brief 唤醒一个等待协程
         */
        void notify();
        /*
         * @brief 唤醒所有等待协程
         */
        void broadcast();
        /**
         * @brief 不加锁地等待唤醒
         */
        void wait();
        /**
         * @brief 等待唤醒
         */
        void wait(CoMutex::Lock &lock);

    private:
        std::queue<std::shared_ptr<Fiber>>
            m_wait_queue;               // 协程等待队列
        MutexType m_mutex;              // 保护协程等待队列
        std::shared_ptr<Timer> m_timer; // 空任务定时器，让调度期保持调度
    };
}

#endif // INCLUDE_SYNC_CO_CONDVAR_HPP_