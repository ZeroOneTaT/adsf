/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-25 17:53:14
 * @LastEditTime: 2023-12-25 17:53:16
 * @FilePath: /adsf/include/utils/io_manager.hpp
 * @Description: io管理类
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef ADFS_IO_MANAGER_HPP_
#define ADFS_IO_MANAGER_HPP_

#include <atomic>
#include <memory>
#include "scheduler.hpp"
#include "sync.hpp"
#include "timer.hpp"

namespace adsf
{
    class IOManager : public Scheduler, public TimeManager
    {
    public:
        using ptr = std::shared_ptr<IOManager>;
        using RWMutexType = RWMutex;
        enum Event
        {
            NONE = 0x0,
            READ = 0x1, // EPOLLIN
            WRITE = 0x4 // EPOLLOUT
        };

    public:
        IOManager(size_t threads = 4, const std::string &name = "");
        ~IOManager();

        /**
         * @brief 事件操作相关函数
         */
        bool addEvent(int fd, Event event, std::function<void()> cb = nullptr);
        bool delEvent(int fd, Event event);
        bool cancelEvent(int fd, Event event);
        bool cancelAllEvent(int fd);

        static IOManager *GetThis();

    protected:
        void notify() override;
        void wait() override;
        bool stopping() override;
        void contextResize(size_t size);
        void onInsertAtFront() override;

    private:
        /**
         * @brief 事件上下文
         */
        struct FdContext
        {
            using MutexType = Mutex;
            struct EventContext
            {
                Scheduler *scheduler = nullptr; // 事件执行的Scheduler
                Fiber::ptr fiber;               // 事件的协程
                std::function<void()> cb;       // 事件的回调函数
                bool empty()
                {
                    return !scheduler && !fiber && !cb;
                }
            };
            EventContext &getContext(Event event);
            void resetContext(EventContext &event);
            void triggerEvent(Event event);
            int fd = 0;          // 事件关联的句柄
            EventContext read;   // 读事件
            EventContext write;  // 写事件
            Event events = NONE; // 注册的事件类型
            MutexType mutex;
        };

    private:
        int m_epfd = 0;                                // epoll 文件句柄
        int m_tickleFds[2];                            // pipe 文件句柄，fd[0]读端，fd[1]写端
        std::atomic<size_t> m_pendingEventCount = {0}; // 当前等待执行的IO事件数量
        RWMutexType m_mutex;                           // IOManager的Mutex
        std::vector<FdContext *> m_fdContexts;         // socket事件上下文的容器

        bool stopping(uint64_t &timeout);
    };

/**
 * @brief 协程事件操作宏
 */
#define go (*adfs::IOManager::GetThis()) +
#define GO (*adsf::IOManager::GetThis()) + [=]() mutable
}

#endif // ADFS_IO_MANAGER_HPP_