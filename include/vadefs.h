/**
 * CpuInfo
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Copyright (c) 2007-2018 University of Illinois at Urbana-Champaign. All rights reserved.
 * Copyright (c) 2017-2018, Aidan Khoury. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file vadefs.h
 * @authors Microsoft (Windows Kits\10\Include\10.0.17134.0\km\crt\vadefs.h)
 *          LLVM Team
 *          Aidan Khoury (ajkhoury)
 * @date 11/15/2018 
 */

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef _INC_VADEFS
#define _INC_VADEFS

#ifndef __WIDL__
#pragma pack(push,8)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined _W64
#define _W64
#endif

#ifndef _VA_LIST_DEFINED
    #define _VA_LIST_DEFINED
    #if defined(__clang__) || defined(__GNUC__)
        typedef __builtin_va_list va_list;
    #elif defined(_MSC_VER)
        #ifdef _M_CEE_PURE
            typedef System::ArgIterator va_list;
        #else
            typedef char* va_list;
        #endif
    #else
        typedef char* va_list;
    #endif
#endif /* _VA_LIST_DEFINED */

#if defined(__clang__) || defined(__GNUC__) /* Use Clang/GCC builtins */

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_copy(d,s)    __builtin_va_copy(d,s)

#elif defined(_MSC_VER) /* Use MSVC specific */

#ifdef __cplusplus
    #define __ADDRESSOF(v) (&const_cast<char&>(reinterpret_cast<const volatile char&>(v)))
#else
    #define __ADDRESSOF(v) (&(v))
#endif

#if (defined _M_ARM || defined _M_HYBRID_X86_ARM64) && !defined(_M_CEE_PURE)
    #define _VA_ALIGN       4
    #define _SLOTSIZEOF(t)  ((sizeof(t) + _VA_ALIGN - 1) & ~(_VA_ALIGN - 1))
    #define _APALIGN(t,ap)  (((va_list)0 - (ap)) & (__alignof(t) - 1))
#elif defined(_M_ARM64) && !defined(_M_CEE_PURE)
    #define _VA_ALIGN       8
    #define _SLOTSIZEOF(t)  ((sizeof(t) + _VA_ALIGN - 1) & ~(_VA_ALIGN - 1))
    #define _APALIGN(t,ap)  (((va_list)0 - (ap)) & (__alignof(t) - 1))
#else
    #define _SLOTSIZEOF(t)  (sizeof(t))
    #define _APALIGN(t,ap)  (__alignof(t))
#endif

#if defined _M_CEE_PURE || (defined _M_CEE && !defined _M_ARM && !defined _M_ARM64)

    //#pragma intrinsic(__va_start)
    //#pragma intrinsic(__va_arg)
    //#pragma intrinsic(__va_end)
    void  __cdecl __va_start( va_list*, ... );
    void* __cdecl __va_arg( va_list*, ... );
    void  __cdecl __va_end( va_list* );

    #define __base_va_start_a(ap, v) ((void)(__va_start(&ap, __ADDRESSOF(v), _SLOTSIZEOF(v), __alignof(v), __ADDRESSOF(v))))
    #define __base_va_arg(ap, t)   (*(t *)__va_arg(&ap, _SLOTSIZEOF(t), _APALIGN(t,ap), (t*)0))
    #define __base_va_end(ap)      ((void)(__va_end(&ap)))

#elif (defined(_M_IX86) || defined(__i386__)) && !defined(_M_HYBRID_X86_ARM64)

    #define _INTSIZEOF(n)          ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

    #define __base_va_start_a(ap, v) ((void)(ap = (va_list)__ADDRESSOF(v) + _INTSIZEOF(v)))
    #define __base_va_arg(ap, t)   (*(t*)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
    #define __base_va_end(ap)      ((void)(ap = (va_list)0))

#elif defined(_M_ARM)

    #ifdef __cplusplus
        #pragma intrinsic(__va_start)
        void __cdecl __va_start( va_list*, ... );
        #define __base_va_start_a(ap, v) ((void)(__va_start(&ap, __ADDRESSOF(v), _SLOTSIZEOF(v), __ADDRESSOF(v))))
    #else
        #define __base_va_start_a(ap, v) ((void)(ap = (va_list)__ADDRESSOF(v) + _SLOTSIZEOF(v)))
    #endif

    #define __base_va_arg(ap, t) (*(t*)((ap += _SLOTSIZEOF(t) + _APALIGN(t,ap)) - _SLOTSIZEOF(t)))
    #define __base_va_end(ap)    ((void)(ap = (va_list)0))

#elif defined(_M_HYBRID_X86_ARM64)
    void __cdecl __va_start(va_list*, ...);
    #define __crt_va_start_a(ap,v) ((void)(__va_start(&ap, _ADDRESSOF(v), _SLOTSIZEOF(v), __alignof(v), _ADDRESSOF(v))))
    #define __crt_va_arg(ap, t)    (*(t*)((ap += _SLOTSIZEOF(t)) - _SLOTSIZEOF(t)))
    #define __crt_va_end(ap)       ((void)(ap = (va_list)0))

#elif defined(_M_ARM64)

    void __cdecl __va_start( va_list*, ... );

    #define __base_va_start_a(ap,v) ((void)(__va_start(&ap, __ADDRESSOF(v), _SLOTSIZEOF(v), __alignof(v), __ADDRESSOF(v))))
    #define __base_va_arg(ap, t)                                                 \
          ((sizeof(t) > (2 * sizeof(__int64)))                                   \
              ? **(t**)((ap += sizeof(__int64)) - sizeof(__int64))               \
              : *(t*)((ap += _SLOTSIZEOF(t) + _APALIGN(t,ap)) - _SLOTSIZEOF(t)))
    #define __base_va_end(ap)       ((void)(ap = (va_list)0))

#elif defined(_M_AMD64) || defined(__x86_64__)

    void __cdecl __va_start( va_list* , ... );

    #define __base_va_start_a(ap, x) ((void)(__va_start(&ap, x)))
    #define __base_va_arg(ap, t)                                               \
          ((sizeof(t) > sizeof(__int64) || (sizeof(t) & (sizeof(t) - 1)) != 0) \
              ? **(t**)((ap += sizeof(__int64)) - sizeof(__int64))             \
              :  *(t* )((ap += sizeof(__int64)) - sizeof(__int64)))
    #define __base_va_end(ap)        ((void)(ap = (va_list)0))

#elif defined(_M_IA64)

#error VARARGS not implemented for IA64

#else

#error VARARGS not implemented for this TARGET

#endif

#endif //

#ifdef __cplusplus
} // extern "C"
#endif


#if defined(__cplusplus) && !defined _CRT_NO_VA_START_VALIDATION
    extern "C++"
    {
        template <typename _Ty>
        struct __base_va_list_is_reference
        {
            enum : bool { __the_value = false };
        };

        template <typename _Ty>
        struct __base_va_list_is_reference<_Ty&>
        {
            enum : bool { __the_value = true };
        };

        template <typename _Ty>
        struct __base_va_list_is_reference<_Ty&&>
        {
            enum : bool { __the_value = true };
        };

        template <typename _Ty>
        struct __base_assert_va_start_is_not_reference
        {
            static_assert(!__base_va_list_is_reference<_Ty>::__the_value,
                "va_start argument must not have reference type and must not be parenthesized");
        };
    } // extern "C++"

    #define __base_va_start(ap, x)  ((void)(__base_assert_va_start_is_not_reference<decltype(x)>(), __base_va_start_a(ap, x)))

#else // ^^^ __cplusplus ^^^ // vvv !__cplusplus vvv //

    #define __base_va_start(ap, x) __base_va_start_a(ap, x)

#endif // __cplusplus

/* Override macros from vadefs.h with definitions that work with Clang. */
#ifndef _crt_va_start
#define _crt_va_start(ap, param) __base_va_start(ap, param)
#endif
#ifndef _crt_va_end
#define _crt_va_end(ap)          __base_va_end(ap)
#endif
#ifndef _crt_va_arg
#define _crt_va_arg(ap, type)    __base_va_arg(ap, type)
#endif

/* VS 2015 switched to double underscore names, which is an improvement, but now
 * we have to intercept those names too.
 */
#ifndef __crt_va_start
#define __crt_va_start(ap, param) __base_va_start(ap, param)
#endif
#ifndef __crt_va_end
#define __crt_va_end(ap)          __base_va_end(ap)
#endif
#ifndef __crt_va_arg
#define __crt_va_arg(ap, type)    __base_va_arg(ap, type)
#endif

#ifndef __WIDL__
#pragma pack(pop)
#endif

#endif // _INC_VADEFS
