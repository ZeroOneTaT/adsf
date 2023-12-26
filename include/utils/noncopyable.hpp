/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-19 20:19:25
 * @LastEditTime: 2023-12-19 20:19:26
 * @FilePath: /adsf/include/utils/noncopyable.hpp
 * @Description: 定义不可拷贝类
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef ADSF_NONCOPYABLE_HPP_
#define ADSF_NONCOPYABLE_HPP_

class Noncopyable
{
public:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable &noncopyable) = delete;
    Noncopyable &operator=(const Noncopyable &noncopyable) = delete;
};

#endif // ADSF_NONCOPYABLE_HPP_