/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-21 20:08:41
 * @LastEditTime: 2023-12-21 20:08:42
 * @FilePath: /adsf/include/utils/singleton.hpp
 * @Description: 单例类定义
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef ADSF_SINGLETON_H_
#define ADSF_SINGLETON_H_

#include <memory>
#include "thread.h"
namespace adsf
{
    /**
     * @brief 单例类，返回一个指向 T 类型对象的指针
     */
    template <typename T>
    class Singleton
    {
    public:
        static T *GetInstance()
        {
            static T instance;
            return &instance;
        }
    };

    /**
     * @brief 单例指针类，返回一个指向 T 类型对象的 std::shared_ptr
     * @tparam T
     */
    template <typename T>
    class SingletonPtr
    {
    public:
        static std::shared_ptr<T> GetInstance()
        {
            static std::shared_ptr<T> instance(std::make_shared<T>());
            return instance;
        }
    };

}
#endif // ADSF_SINGLETON_H_