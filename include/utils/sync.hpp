/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-20 21:36:34
 * @LastEditTime: 2023-12-20 21:38:39
 * @FilePath: /adsf/include/utils/sync.hpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef ADFS_SYNC_HPP_
#define ADFS_SYNC_HPP_

/**
 * @brief 包含了协程同步原语
 * CoMutex      协程锁
 * CoCondvar    协程条件变量
 * CoCountDownLatch 协程计数器
 * CoSemaphore  协程信号量
 * Channel      消息通道
 */

#include "../sync/mutex.hpp"
#include "../sync/co_condvar.hpp"
#include "../sync/co_semaphore.hpp"
#include "../sync/mutex.hpp"

#endif // ADFS_SYNC_HPP_