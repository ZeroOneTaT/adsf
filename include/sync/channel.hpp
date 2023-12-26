/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-20 21:16:42
 * @LastEditTime: 2023-12-20 21:16:44
 * @FilePath: /adsf/include/sync/channel.hpp
 * @Description: 自定义通道
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef INCLUDE_SYNC_CHANNEL_H_
#define INCLUDE_SYNC_CHANNEL_H_

#include "co_condvar.hpp"
namespace adsf
{
    /**
     * @brief Channel主要是用于协程之间的通信，属于更高级层次的抽象
     *        在类的实现上采用了 PIMPL 设计模式，将具体操作转发给实现类
     *        Channel 对象可随意复制，通过智能指针指向同一个 ChannelImpl
     */
    template <typename T>
    class Channel
    {
    public:
        Channel(size_t capacity)
        {
            m_channel = std::make_shared<ChannelImpl<T>>(capacity);
        }
        Channel(const Channel &chan)
        {
            m_channel = chan.m_channel;
        }
        void close()
        {
            m_channel->close();
        }
        operator bool() const
        {
            return *m_channel;
        }

        bool push(const T &t)
        {
            return m_channel->push(t);
        }

        bool pop(T &t)
        {
            return m_channel->pop(t);
        }

        Channel &operator>>(T &t)
        {
            (*m_channel) >> t;
            return *this;
        }

        Channel &operator<<(const T &t)
        {
            (*m_channel) << t;
            return *this;
        }

        size_t capacity() const
        {
            return m_channel->capacity();
        }

        size_t size()
        {
            return m_channel->size();
        }

        bool empty()
        {
            return m_channel->empty();
        }

        bool unique() const
        {
            return m_channel.unique();
        }

    private:
        std::shared_ptr<ChannelImpl<T>> m_channel;
    };

    /**
     * @brief ChannelImpl 实现类，负责具体的操作
     */
    template <typename T>
    class ChannelImpl : Noncopyable
    {
    public:
        ChannelImpl(size_t capacity)
            : m_isClose(false), m_capacity(capacity) {}
        ~ChannelImpl()
        {
            close();
        }

        /**
         * @brief 发送数据到 Channel
         * @param[in] t 发送的数据
         * @return 返回调用结果
         */
        bool push(const T &t)
        {
            CoMutex::Lock lock(m_mutex);
            if (m_isClose)
                return false;
            // 如果缓冲区已满,等待m_condvar_push唤醒
            while (m_queue.size() >= m_capacity)
            {
                m_condvar_push.wait(lock);
                if (m_isClose)
                    return false;
            }
            // 缓冲区存在空间,数据放入缓冲区，并唤醒m_condvar_pop
            m_queue.push(t);
            m_condvar_pop.notify();
            return true;
        }

        /**
         * @brief 从 Channel 读取数据
         * @param[in] t 读取到 t
         * @return 返回调用结果
         */
        bool pop(T &t)
        {
            CoMutex::Lock lock(m_mutex);
            if (m_isClose)
            {
                return false;
            }
            // 如果缓冲区为空，等待 m_condvar_pop 唤醒
            while (m_queue.empty())
            {
                m_condvar_pop.wait(lock);
                if (m_isClose)
                {
                    return false;
                }
            }
            t = m_queue.front();
            m_queue.pop();
            // 唤醒 m_condvar_push
            m_condvar_push.notify();
            return true;
        }

        /**
         * @brief 重载 >>
         */
        ChannelImpl &operator>>(T &t)
        {
            pop(t);
            return *this;
        }

        /**
         * @brief 重载 <<
         */
        ChannelImpl &operator<<(const T &t)
        {
            push(t);
            return *this;
        }

        /**
         * @brief 关闭 Channel
         */
        void close()
        {
            CoMutex::Lock lock(m_mutex);
            if (m_isClose)
                return;
            m_isClose = true;
            // 唤醒等待协程
            m_condvar_push.notify();
            m_condvar_pop.notify();
            // 清空消息队列
            std::queue<T>().swap(m_queue);
        }

        operator bool()
        {
            return !m_isClose;
        }

        size_t capacity() const
        {
            return m_capacity;
        }

        size_t size()
        {
            CoMutex::Lock lock(m_mutex);
            return m_queue.size();
        }

        bool empty()
        {
            return !size();
        }

    private:
        bool m_isClose;           // 关闭标识
        size_t m_capacity;        // Channel 缓冲区大小
        CoMutex m_mutex;          // 协程锁和协程条件变量配合使用保护消息队列
        CoCondVar m_condvar_push; // 入队条件变量
        CoCondVar m_condvar_pop;  // 出队条件变量
        std::queue<T> m_queue;    // 消息队列
    };
}
#endif // INCLUDE_SYNC_CHANNEL_H_