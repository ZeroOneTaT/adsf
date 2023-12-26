/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-20 21:40:51
 * @LastEditTime: 2023-12-20 21:40:53
 * @FilePath: /adsf/include/utils/thread.hpp
 * @Description: 线程类定义
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef ADFS_THREAD_H_
#define ADFS_THREAD_H_
#include <atomic>
#include <string>
#include <memory>
#include <functional>
#include <pthread.h>
#include <unistd.h>
#include "sync.hpp"

namespace adsf
{
    class Thread
    {
    public:
        using ptr = std::shared_ptr<Thread>;
        using callback = std::function<void()>;

        Thread(const std::string &name, callback cb)
            : m_id(0), m_pthread(0) {}
        ~Thread();

        void join();
        const ats::string &getName() const { return m_name; }
        pid_t getId() const { return m_id; }

        static void *run(void *arg);
        static const std::string &GetName();
        static Thread *GetThis();
        static void SetName(const std::string &name);

    private:
        Thread(const Thread &thread) = delete;
        Thread(const Thread &&thread) = delete;
        Thread &operator=(const Thread &thread) = delete;

    private:
        pid_t m_id;
        pthread_t m_pthread;
        callback m_callback;
        std::string m_name;
        Semaphore m_sem{1};
    };
}

#endif // ADFS_THREAD_H_