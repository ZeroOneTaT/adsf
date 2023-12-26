/***
 * @Author: ZeroOneTaT
 * @Date: 2023-12-25 16:48:43
 * @LastEditTime: 2023-12-25 16:49:20
 * @FilePath: /adsf/include/utils/macro.hpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ZeroOneTaT, All Rights Reserved.
 */

#ifndef ADFS_MACRO_HPP_
#define ADFS_MACRO_HPP_

#include <assert.h>
#include <stdlib.h>
#include <string>
#include "util.hpp"

#if defined(__GNUC__) || defined(__llvm__)
#define ADSF_LIKELY(x) __builtin_expect(!!(x), 1)
#define ADSF_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define ADSF_LIKELY(x) (x)
#define ADSF_UNLIKELY(x) (x)
#endif

#define ADSF_ASSERT(x)                                                              \
    if (ADSF_UNLIKELY(!(x)))                                                        \
    {                                                                               \
        ADSF_LOG_ERROR(ADSF_LOG_ROOT()) << "ASSERTION: " << #x                      \
                                        << "\nbacktrace:\n"                         \
                                        << adsf::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                  \
        exit(1);                                                                    \
    }

#define _ASSERT2(x, m)                                                              \
    if (ADSF_UNLIKELY(!(x)))                                                        \
    {                                                                               \
        ADSF_LOG_ERROR(ADSF_LOG_ROOT()) << "ASSERTION: " << #x                      \
                                        << "\n"                                     \
                                        << m << "\n"                                \
                                        << "\nbacktrace:\n"                         \
                                        << adsf::BacktraceToString(100, 2, "    "); \
        assert(x);                                                                  \
        exit(1);                                                                    \
    }

#define ADSF_STATIC_ASSERT(x)                                                       \
    if constexpr (!(x))                                                             \
    {                                                                               \
        ADSF_LOG_ERROR(ADSF_LOG_ROOT()) << "ASSERTION: " << #x                      \
                                        << "\nbacktrace:\n"                         \
                                        << adsf::BacktraceToString(100, 2, "    "); \
        exit(1);                                                                    \
    }

#endif // ADFS_MACRO_HPP_