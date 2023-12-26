/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-21 20:15:11
 * @LastEditTime: 2023-12-21 20:15:13
 * @FilePath: /adsf/include/utils/util.hpp
 * @Description: 工具类定义
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef ADSF_UTIL_H_
#define ADSF_UTIL_H_

#include <byteswap.h> // 字节序转换相关的函数
#include <unistd.h>   // 提供与进程和线程相关的系统调用
#include <syscall.h>  // 提供系统调用接口
#include <sys/time.h> // 提供获取时间的函数

#include <bit>         // C++20 中的位操作库
#include <concepts>    // C++20 中的概念库
#include <cstdint>     // 提供标准整数类型
#include <string_view> // 提供字符串视图类型
#include <algorithm>   // 提供算法函数
#include <iostream>    // 提供标准输入输出流

namespace adsf
{
    /**
     * @brief 获取当前线程id
     */
    pid_t GetThreadId();
    /**
     * @brief 获取当前协程id
     */
    int64_t GetFiberId();

    /**
     * @brief 栈信息和 string 相互转换
     */
    void Backtrace(std::vector<std::string> &bt, int size, int skip = 1);
    std::string BacktraceToString(int size, int skip = 2, const std::string &prefix = "");

    /**
     * @brief 获取当前时间戳
     */
    uint64_t GetCurrentMS();
    uint64_t GetCurrentUS();
}

#endif // ADSF_UTIL_H_