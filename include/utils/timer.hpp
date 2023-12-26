/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-25 16:59:03
 * @LastEditTime: 2023-12-25 16:59:06
 * @FilePath: /adsf/include/utils/timer.hpp
 * @Description: 计时器类
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef ADFS_TIMER_HPP_
#define ADFS_TIMER_HPP_

#include <functional>
#include <memory>
#include <set>
#include "log.hpp"
#include "sync.hpp"

namespace adsf
{
    /**
     * @brief 计时器类
     */
    class TimeManager;
    class Timer : public std::enable_shared_from_this<Timer>
    {
    public:
        using ptr = std::shared_ptr<Timer>;
        bool cancel();
        bool refresh();
        bool reset(uint64_t ms, bool fromnow);

    private:
        Timer(uint64_t ms, std::function<void()> cb, bool recurring, TimeManager *timeManager);
        Timer(uint64_t next);

    private:
        uint64_t m_ms = 0;          // 执行周期
        uint64_t m_next = 0;        // 精确的执行时间
        std::function<void()> m_cb; // 定时器回调
        bool m_recurring = false;   // 是否循环
        TimeManager *m_manager = nullptr;

    private:
        struct Compare
        {
            bool operator()(const Timer::ptr &lhs, const Timer::ptr &rhs) const;
        };
    };

    /**
     * @brief 计时器管理类
     */
    class TimeManager
    {
        friend class Timer;

    public:
        using MutexType = Mutex;
        TimeManager();
        virtual ~TimeManager();
        Timer::ptr addTimer(uint64_t ms, std::function<void()> cb, bool recurring = false);
        Timer::ptr addConditionTimer(uint64_t ms, std::function<void()> cb,
                                     std::weak_ptr<void> cond,
                                     bool recurring = false);
        uint64_t getNextTimer();
        void getExpiredCallbacks(std::vector<std::function<void()>> &cbs);
        bool hasTimer();

    protected:
        virtual void onInsertAtFront() = 0;
        void addTimer(Timer::ptr timer, MutexType::Lock &lock);

    private:
        MutexType m_mutex;
        std::set<Timer::ptr, Timer::Compare> m_timers;
        bool m_tickled = false;
    };
}

#endif // ADFS_TIMER_HPP_
