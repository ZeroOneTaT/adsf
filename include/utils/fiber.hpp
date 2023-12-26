/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-21 16:55:50
 * @LastEditTime: 2023-12-21 16:55:52
 * @FilePath: /adsf/include/utils/fiber.hpp
 * @Description: 协程类定义
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef ADSF_FIBER_H_
#define ADSF_FIBER_H_
#include <functional>
#include <memory>
#include <ucontext.h>

#include "thread.hpp"

namespace adsf
{
    /**
     * @brief 协程类，自动保存调用者协程的指针，可在协程里面调用另一个协程
     */
    class Fiber : public std::enable_shared_from_this<Fiber>
    {
    public:
        using ptr = std::shared_ptr<Fiber>;
        enum State
        {
            INIT,
            HOLD,
            EXEC,
            TERM,
            READY,
            EXCEPT
        };

    public:
        Fiber(std::function<void()> callback, size_t stacksize = 0);
        ~Fiber();

        /**
         * @brief 重置协程执行函数，重置状态
         */
        void reset(std::function<void()> callback);
        /**
         * @brief 切换到当前协程
         */
        void resume();
        /**
         * @brief 释放当前协程
         */
        void yield();

        uint64_t getId() { return m_id; }
        State getState() const { return m_state; }
        bool isTerminate() const
        {
            return m_state == TERM || m_state == EXCEPT;
        }

        /**
         * @brief 在当前线程启用协程
         */
        static void EnableFiber();
        /**
         * @brief 获取当前协程
         */
        static Fiber::ptr GetThis();
        /**
         * @brief 获取当前协程id
         */
        static uint64_t GetFiberId();
        /**
         * @brief 设置当前协程
         */
        static void SetThis(Fiber *fiber);
        /**
         * @brief 让出协程，并设置协程状态为 READY
         */
        static void YieldToReady();
        /**
         * @brief 让出协程，并设置协程状态为 HOLD
         */
        static void YieldToHold();
        /**
         * @brief 协程总数
         */
        static uint64_t TotalFibers();

        static void MainFunc();

    private:
        Fiber();

    private:
        uint64_t m_id = 0;                // 协程id
        uint32_t m_stack_size = 0;        //  协程栈大小
        State m_state = INIT;             // 协程状态
        ucontext_t m_ctx;                 // 协程上下文信息
        void *m_stack = nullptr;          // 协程栈指针
        std::function<void()> m_callback; // 协程回调函数
    };
}

#endif // ADSF_FIBER_H_