/**
 * CpuInfo
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
 * @file compiler.h
 * @authors LLVM Team
 *          Linux Kernel (https://github.com/torvalds/linux/blob/master/include/linux/compiler.h)
 *          Aidan Khoury (ajkhoury)
 * @date 11/15/2018 
 */

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "stdarg.h"

#if !defined(__ASSEMBLY__)

#if (defined(__GNUC__) && defined(__GNUC_MINOR__)) || defined(__clang__)
#define GNUC_PREREQ(major, minor)   \
    (__GNUC__ > (major) || (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))
#else
#define GNUC_PREREQ(major, minor) 0
#endif

#if defined(_MSC_VER)
#define MSC_PREREQ(major, minor) (_MSC_VER >= ((major) * 100 + (minor) * 10))
#else
#define MSC_PREREQ(major, minor) 0
#endif

#if defined(_MSC_VER) && !(defined(__GNUC__) || defined(__clang__))

#define __has_builtin(x)    1
#define __visible           // not supported

#define __safe              // not supported
#define __force             // not supported
#define __nocast            // not supported

/* Include annotations */
//#include <sal.h>
//#include <specstrings_strict.h>

#endif

/* Indirect macros required for expanded argument pasting, eg. __LINE__. */
#define ___PASTE(a,b) a##b
#define __PASTE(a,b) ___PASTE(a,b)
#define __CONCAT(a,b) ___PASTE(a,b)
#define _ASSERT_CONCAT(a,b) ___PASTE(a,b)

//
// C_ASSERT() can be used to perform many compile-time assertions:
//            type sizes, field offsets, etc.
//
// An assertion failure results in error C2118: negative subscript.
//
#ifndef C_ASSERT
#if defined(_MSC_VER) && !(defined(__GNUC__) || defined(__clang__))
# define C_ASSERT(e) typedef char _ASSERT_CONCAT(__C_ASSERT__,__LINE__)[(e)?1:-1]
#else
# define C_ASSERT(e) ;enum { _ASSERT_CONCAT(__C_ASSERT__,_ASSERT_CONCAT(__COUNTER__,__LINE__)) = 1/(int)(!!(e)) } __attribute__((unused))
#endif
#endif // C_ASSERT

/* __CHECKER__ macros */
#if defined(__GNUC__) || defined(__clang__)
#define __safe              __attribute__((safe))
#define __force             __attribute__((force))
#define __nocast            __attribute__((nocast))
#endif

/* C99 defines the restrict type qualifier keyword, which was made available in GCC 2.92. */
#if (__STDC_VERSION__ >= 199901L)
# define __restrict    restrict
#else
# if defined(_MSC_VER) || !GNUC_PREREQ(2, 92)
#  define __restrict  /* delete __restrict when not supported */
# endif
#endif

/* Optimization barrier */
#define barrier()       \
    _ReadWriteBarrier()

/**
 * GCC specific compiler macros
 */
#if (defined(__GNUC__) || defined(__clang__))

/* Common definitions for all gcc versions go here. */
#if defined(__clang__)
# define CLANG_VERSION  (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#else
# define GCC_VERSION    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

/*
 * This version is i.e. to prevent dead stores elimination on @ptr
 * where GCC and llvm may behave differently when otherwise using
 * normal barrier(): while GCC behavior gets along with a normal
 * barrier(), llvm needs an explicit input variable to be assumed
 * clobbered. The issue is as follows: while the inline asm might
 * access any memory it wants, the compiler could have fit all of
 * @ptr into memory registers instead, and since @ptr never escaped
 * from that, it proved that the inline asm wasn't touching any of
 * it. This version works well with both compilers, i.e. we're telling
 * the compiler that the inline asm absolutely may see the contents
 * of @ptr. See also: https://llvm.org/bugs/show_bug.cgi?id=15495
 */
#define barrier_data(ptr)   \
    __asm__ __volatile__("": :"r"(ptr) :"memory")

/*
 * This macro obfuscates arithmetic on a variable address so that GCC
 * shouldn't recognize the original var, and make assumptions about it.
 *
 * This is needed because the C standard makes it undefined to do
 * pointer arithmetic on "objects" outside their boundaries and the
 * GCC optimizers assume this is the case. In particular they
 * assume such arithmetic does not wrap.
 *
 * A miscompilation has been observed because of this on PPC.
 * To work around it we hide the relationship of the pointer and the object
 * using this macro.
 *
 * Versions of the ppc64 compiler before 4.1 had a bug where use of
 * RELOC_HIDE could trash r30. The bug can be worked around by changing
 * the inline assembly constraint from =g to =r, in this particular
 * case either is valid.
 */
#define RELOC_HIDE(ptr, off)                    \
({                                              \
    unsigned long __ptr;                        \
    __asm__ ("" : "=r"(__ptr) : "0"(ptr));      \
    (typeof(ptr)) (__ptr + (off));              \
})

/* Make the optimizer believe the variable can be manipulated arbitrarily. */
#define OPTIMIZER_HIDE_VAR(var)                 \
    __asm__ ("" : "=r" (var) : "0" (var))

#ifdef __CHECKER__
#define __must_be_array(a)  0
#else
/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a)  BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#endif

/*
 * Simple shorthand for a section definition
 */
#define __section(S)    __attribute__((__section__(#S)))

/*
 * Force always-inline if the user requests it so via the .config,
 * or if GCC is too old.
 * GCC does not warn about unused static inline functions for
 * -Wunused-function.  This turns out to avoid the need for complex #ifdef
 * directives.  Suppress the warning in clang as well by using "unused"
 * function attribute, which is redundant but not harmful for gcc.
 */
#define __always_inline     inline __attribute__((always_inline))
#define __noinline          __attribute__((noinline))

/*
 * Allow us to mark functions as 'deprecated' and have gcc emit a nice
 * warning for each use, in hopes of speeding the functions removal.
 * Usage is:
 *      int __deprecated foo(void)
 */
#define __deprecated        __attribute__((deprecated))

/*
 * Specify structure as packed (no alignment)
 */
#if (defined(__clang__) || defined(__GNUC__))
#define __packed            __attribute__((packed))
#endif

#define __weak              __attribute__((weak))
#define __alias(symbol)     __attribute__((alias(#symbol)))

/*
 * Are two types/vars the same type (ignoring qualifiers)?
 */
#define __same_type(a, b)   __builtin_types_compatible_p(typeof(a), typeof(b))

/*
 * Is this type a native word size (useful for atomic operations)
 */
#define __native_word(t)            \
    (sizeof(t) == sizeof(char)  ||  \
    sizeof(t) == sizeof(short)  ||  \
    sizeof(t) == sizeof(int)    ||  \
    sizeof(t) == sizeof(long))

/*
 * Calling Conventions
 */
#if (defined(__GNUC__) || defined(__clang__)) //&& !defined(_MSC_VER)
#define __cdecl             __attribute__((__cdecl__))
#define __stdcall           __attribute__((__stdcall__))
#define __fastcall          __attribute__((__fastcall__))
#endif


/*
 * Pretty function formatting
 */
#ifndef __func__
#define __func__ __FUNCTION__
#endif


/*
 * it doesn't make sense on ARM (currently the only user of __naked)
 * to trace naked functions because then mcount is called without
 * stack and frame pointer being set up and there is no chance to
 * restore the lr register to the value before mcount was called.
 *
 * The asm() bodies of naked functions often depend on standard calling
 * conventions, therefore they must be noinline and noclone.
 *
 * GCC 4.[56] currently fail to enforce this, so we must do so ourselves.
 * See GCC PR44290.
 */
#define __naked             __attribute__((naked)) __noinline __noclone notrace


/*
 * Allow us to avoid 'defined but not used' warnings on functions and data,
 * as well as force them to be emitted to the assembly file.
 *
 * As of gcc 3.4, static functions that are not marked with attribute((used))
 * may be elided from the assembly file.  As of gcc 3.4, static data not so
 * marked will not be elided, but this may change in a future gcc version.
 *
 * NOTE: Because distributions shipped with a backported unit-at-a-clock
 * compiler in gcc 3.3, we must define __used to be __attribute__((used))
 * for gcc >=3.3 instead of 3.4.
 *
 * In prior versions of gcc, such functions and data would be emitted, but
 * would be warned about except with attribute((unused)).
 *
 * Mark functions that are referenced only in inline assembly as __used so
 * the code is emitted even though it appears to be unreferenced.
 */
#define __used              __attribute__((used))
#define __maybe_unused      __attribute__((unused))
#define __always_unused     __attribute__((unused))
#define __unused            __attribute__((unused))

#define __mode(x)           __attribute__((mode(x)))


#define __noreturn          __attribute__((noreturn))

#define likely(x)           __builtin_expect(!!(x), 1)
#define unlikely(x)         __builtin_expect(!!(x), 0)


/*
 * From the GCC manual:
 *
 * Many functions have no effects except the return value and their
 * return value depends only on the parameters and/or global
 * variables.  Such a function can be subject to common subexpression
 * elimination and loop optimization just as an arithmetic operator
 * would be.
 * [...]
 */
#define __pure              __attribute__((pure))
#define __aligned(x)        __attribute__((__aligned__(x)))
#define __aligned_largest   __attribute__((__aligned__))
#define __printf(a, b)      __attribute__((format(printf, a, b)))
#define __scanf(a, b)       __attribute__((format(scanf, a, b)))

/*
 * From the GCC manual:
 *
 * Many functions do not examine any values except their arguments,
 * and have no effects except the return value.  Basically this is
 * just slightly more strict class than the `pure' attribute above,
 * since function is not allowed to read global memory.
 *
 * Note that a function that has pointer arguments and examines the
 * data pointed to must _not_ be declared `const'.  Likewise, a
 * function that calls a non-`const' function usually must not be
 * `const'.  It does not make sense for a `const' function to return
 * `void'.
 */
#define __attribute_const__ __attribute__((__const__))

/*
 * Mark functions as cold.
 * This tells the compiler that a function is "cold", meaning infrequently
 * executed. This may lead it to optimize for size more aggressively than speed,
 * or to allocate the body of the function in a distant part of the text segment
 * to help keep it from taking up unnecessary icache when it isn't in use.
 */
#define __cold              __attribute__((__cold__))



/**
 * GCC or Clang version specific checks
 */
#if defined(__clang__)

/**
 * Clang Specific Compiler Macros
 */

/* Clang 3.8 or newer required. */
#if CLANG_VERSION < 30800
#error Clang version is too old - please upgrade to at least Clang version 4.0.
#endif

/* warn_unused_result attribute was added in Clang 3.9 */
#if CLANG_VERSION >= 30900
#define __must_check        __attribute__((warn_unused_result))
#else
#define __must_check        // nothing
#endif

#define __malloc            __attribute__((__malloc__))

#define __compiler_offsetof(a, b)           \
    __builtin_offsetof(a, b)

#define __compiletime_object_size(obj)      \
    __builtin_object_size(obj, 0)

#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)

#define __compiletime_warning(message)  __attribute__((warning(message)))
#define __compiletime_error(message)    __attribute__((error(message)))


#define annotate_unreachable() // nothing

/* Mark a position in code as unreachable.
 * This can be used to suppress control flow warnings after asm blocks that
 * transfer control elsewhere.
 */
#define unreachable()               \
    do { annotate_unreachable();    \
        __builtin_unreachable();    \
    } while (0)

/* The noclone attribute is not supported by Clang yet... */
#define __noclone           // nothing

/* The externally_visible attribute is not supported by Clang yet... */
#define __visible           // nothing

/*
 * __assume_aligned(n, k): Tell the optimizer that the returned
 * pointer can be assumed to be k modulo n. The second argument is
 * optional (default 0), so we use a variadic macro to make the
 * shorthand.
 *
 * Beware: Do not apply this to functions which may return
 * ERR_PTRs. Also, it is probably unwise to apply it to functions
 * returning extra information in the low bits (but in that case the
 * compiler should see some alignment anyway, when the return value is
 * massaged by 'flags = ptr & 3; ptr &= ~3;').
 */
#define __assume_aligned(a, ...) __attribute__((__assume_aligned__(a, ##__VA_ARGS__)))

/*
 * GCC 'asm goto' miscompiles certain code sequences:
 *
 *   http://gcc.gnu.org/bugzilla/show_bug.cgi?id=58670
 *
 * Work it around via a compiler barrier quirk suggested by Jakub Jelinek.
 *
 * (asm goto is automatically volatile - the naming reflects this.)
 */
#define asm_volatile_goto(x...) do { asm goto(x); asm (""); } while (0)

/* These have been supported since Clang 3.8 (?) */
#define __HAVE_BUILTIN_BSWAP16__
#define __HAVE_BUILTIN_BSWAP32__
#define __HAVE_BUILTIN_BSWAP64__

/*
 * Clang needs to reference var.
 */
#define uninitialized_var(x) x = *(&(x))

#else /* __clang__ */

/**
 * GCC Specific Compiler Macros
 */

#if (GCC_VERSION < 30200)
#error Sorry, your compiler is too old - please upgrade it.
#endif

#if GCC_VERSION < 30300
#define __used              __attribute__((__unused__))
#else
#define __used              __attribute__((__used__))
#endif

#if GCC_VERSION >= 30400
#define __must_check        __attribute__((warn_unused_result))
#define __malloc            __attribute__((__malloc__))
#endif

#if GCC_VERSION >= 40000

#undef __used
#define __used              __attribute__((__used__))

#define __compiler_offsetof(a, b)           \
    __builtin_offsetof(a, b)

#if GCC_VERSION >= 40100
#define __compiletime_object_size(obj)      \
    __builtin_object_size(obj, 0)
#endif

#if GCC_VERSION >= 40300
/* Mark functions as cold. gcc will assume any path leading to a call
 * to them will be unlikely.  This means a lot of manual unlikely()s
 * are unnecessary now for any paths leading to the usual suspects
 * like BUG(), printk(), panic() etc. [but let's keep them for now for
 * older compilers]
 *
 * Early snapshots of gcc 4.3 don't support this and we can't detect this
 * in the preprocessor, but we can live with this because they're unreleased.
 * Maketime probing would be overkill here.
 *
 * gcc also has a __attribute__((__hot__)) to move hot functions into
 * a special section, but I don't see any sense in this right now in
 * the kernel context
 */
#define __cold                  __attribute__((__cold__))

/*
 * Same as GCC, this was present in clang-2.6 so we can assume it works
 * with any version that can compile the kernel
 */
#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)

#define __compiletime_warning(message)  __attribute__((warning(message)))
#define __compiletime_error(message)    __attribute__((error(message)))
#endif /* GCC_VERSION >= 40300 */

#if GCC_VERSION >= 40500
#define annotate_unreachable()
/*
 * Mark a position in code as unreachable.  This can be used to
 * suppress control flow warnings after asm blocks that transfer
 * control elsewhere.
 *
 * Early snapshots of gcc 4.5 don't support this and we can't detect
 * this in the preprocessor, but we can live with this because they're
 * unreleased.  Really, we need to have autoconf for the kernel.
 */
#define unreachable()               \
    do { annotate_unreachable();    \
        __builtin_unreachable();    \
    } while (0)

/* Mark a function definition as prohibited from being cloned. */
#define __noclone               __attribute__((__noclone__, __optimize__("no-tracer")))
#endif /* GCC_VERSION >= 40500 */

#if GCC_VERSION >= 40600
/*
 * When used with Link Time Optimization, gcc can optimize away C functions or
 * variables which are referenced only from assembly code.  __visible tells the
 * optimizer that something else uses this function or variable, thus preventing
 * this.
 */
#define __visible               __attribute__((externally_visible))
#endif /* GCC_VERSION >= 40600 */

 #if GCC_VERSION >= 40900
/*
 * __assume_aligned(n, k): Tell the optimizer that the returned
 * pointer can be assumed to be k modulo n. The second argument is
 * optional (default 0), so we use a variadic macro to make the
 * shorthand.
 *
 * Beware: Do not apply this to functions which may return
 * ERR_PTRs. Also, it is probably unwise to apply it to functions
 * returning extra information in the low bits (but in that case the
 * compiler should see some alignment anyway, when the return value is
 * massaged by 'flags = ptr & 3; ptr &= ~3;').
 */
#define __assume_aligned(a, ...) __attribute__((__assume_aligned__(a, ##__VA_ARGS__)))
#endif

/*
 * GCC 'asm goto' miscompiles certain code sequences:
 *
 *   http://gcc.gnu.org/bugzilla/show_bug.cgi?id=58670
 *
 * Work it around via a compiler barrier quirk suggested by Jakub Jelinek.
 *
 * (asm goto is automatically volatile - the naming reflects this.)
 */
#define asm_volatile_goto(x...) do { asm goto(x); asm (""); } while (0)

/*
 * sparse (__CHECKER__) pretends to be gcc, but can't do constant
 * folding in __builtin_bswap*() (yet), so don't set these for it.
 */
#if GCC_VERSION >= 40400
#define __HAVE_BUILTIN_BSWAP32__
#define __HAVE_BUILTIN_BSWAP64__
#endif
#if GCC_VERSION >= 40800
#define __HAVE_BUILTIN_BSWAP16__
#endif

#endif  /* gcc version >= 40000 specific checks */

#if !defined(__noclone)
#define __noclone   /* not needed */
#endif

#if !defined(__no_sanitize_address)
#define __no_sanitize_address // nothing
#endif

/* A trick to suppress uninitialized variable warning without generating any code */
#define uninitialized_var(x) x = x

#endif /* __clang__ */

#else

/*
 * Most likely MSVC Compiler
 */
#define __used              // not supported
#define __maybe_unused      // not supported
#define __always_unused     // not supported
#define __unused            // not supported

#if defined(_MSC_VER)
#define __section(S)        __declspec(code_seg(#S))
#endif

#endif /* __GNUC__ || __clang__ */

#endif /* __ASSEMBLY__ */



/*
 * Prevent the compiler from merging or refetching accesses. The compiler
 * is also forbidden from reordering successive instances of ACCESS_ONCE(),
 * but only when the compiler is aware of some particular ordering.  One way
 * to make the compiler aware of ordering is to put the two invocations of
 * ACCESS_ONCE() in different C statements.
 *
 * ACCESS_ONCE will only work on scalar types. For union types, ACCESS_ONCE
 * on a union member will work as long as the size of the member matches the
 * size of the union and the size is smaller than word size.
 *
 * The major use cases of ACCESS_ONCE used to be (1) Mediating communication
 * between process-level code and irq/NMI handlers, all running on the same CPU,
 * and (2) Ensuring that the compiler does not fold, spindle, or otherwise
 * mutilate accesses that either do not require ordering or that interact
 * with an explicit memory barrier or atomic instruction that provides the
 * required ordering.
 *
 * If possible use READ_ONCE()/WRITE_ONCE() instead.
 */
#ifndef ACCESS_ONCE
#define __ACCESS_ONCE(x) ({ \
     __maybe_unused typeof(x) __var = (__force typeof(x)) 0; (volatile typeof(x) *)&(x); })
#define ACCESS_ONCE(x) (*__ACCESS_ONCE(x))
#endif // !ACCESS_ONCE

/*
 * Calling conventions
 */
#ifndef _MANAGED
#if defined(_M_IX86) || defined(__i386__)
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
#else
#define FASTCALL NTAPI
#endif
#define STDCALL __stdcall
#define CDECL   __cdecl


/* Unused compiler dependent type specifier. */
#if (defined(__GNUC__) || defined(__clang__))
#  define UNUSED       __unused
#  define UNUSED_PARAM(x)   ((void)(x))
#else
#  define UNUSED            // unimplemented
#  define UNUSED_PARAM(x)   ((void)(x))
#endif // (__clang__ || __GNUC__)

/* Restrict specifier */
#ifndef RESTRICT_DEFINED
#define RESTRICT_DEFINED
#if (defined(__GNUC__) || defined(__clang__)) && !defined(_MSC_VER)
#  define RESTRICT          __restrict__
#  define RESTRICT_FUNC     __restrict__
#elif defined(_MSC_VER) || defined(__INTEL_COMPILER)
#  define RESTRICT          __restrict
#  define RESTRICT_FUNC     __declspec(restrict)
#endif // (__clang__ || __GNUC__) && !_MSC_VER
#endif // !RESTRICT_DEFINED

/* Restricted pointer. Only used for MIPS instruction set. */
#if (defined(__GNUC__) || defined(__clang__)) && !defined(_MSC_VER)
#  define RESTRICTED_POINTER    __restrict__
#else
#if defined(_M_MRX000) && !(defined(MIDL_PASS) || defined(RC_INVOKED)) && defined(ENABLE_RESTRICTED)
#  define RESTRICTED_POINTER    __restrict
#else
#  define RESTRICTED_POINTER
#endif // _M_MRX000 && !(MIDL_PASS || RC_INVOKED) && ENABLE_RESTRICTED
#endif // (__clang__ || __GNUC__) && !_MSC_VER

/* Force Inline  */
#ifndef FORCEINLINE
#if defined(__GNUC__) || defined(__clang__)
# ifndef __forceinline
#  define __forceinline __attribute__((always_inline)) inline
# endif
# define FORCEINLINE    static __attribute__((__always_inline__,__gnu_inline__)) inline
#elif defined(_MSC_VER)
# define FORCEINLINE    __forceinline
#else
# define FORCEINLINE    __attribute__((__always_inline__)) __inline__
#endif // _MSC_VER
#endif // !FORCEINLINE

/* Aligned */
#ifndef ALIGNED
#ifndef __WIDL__
# if (defined(__clang__) || defined(__GNUC__)) && !defined(_MSC_VER)
#  define ALIGNED(x)  __aligned(x)
# elif defined(_MSC_VER) && (_MSC_VER >= 1300) && !defined(MIDL_PASS)
# if !(defined(__clang__) || defined(__GNUC__))
#  define __aligned(x) __declspec(align(x))
# endif
#  define ALIGNED(x) __declspec(align(x))
# else
#  define ALIGNED(x)      /*__declspec(align(x))*/
# endif
#else
# define ALIGNED(x)
#endif // !__WIDL__
#endif // !ALIGNED

/* Unaligned value specifier */
#ifndef _UNALIGNED
#if (defined(_M_AMD64) || defined(__x86_64__)) && defined(_MSC_VER)
#define _UNALIGNED  __unaligned
#define UNALIGNED   __unaligned
#else
#define _UNALIGNED
#define UNALIGNED
#endif // __X64__ && _MSC_VER
#endif // !UNALIGNED

#ifndef DLLIMPORT
#if ((defined(_M_IX86) || defined(__i386__)) || \
     (defined(_M_AMD64) || defined(__x86_64__))) && !defined(MIDL_PASS)
# if defined(_MSC_VER)
#  define DLLIMPORT __declspec(dllimport)
# elif (defined(__clang__) || defined(__GNUC__))
#  define DLLIMPORT __attribute__((dllimport))
# else
#  define DLLIMPORT extern
# endif
#else
# define DLLIMPORT
#endif // ((_M_IX86 || __i386__) || (_M_AMD64 || __x86_64__)) && !MIDL_PASS
#endif // !DLLIMPORT

#ifndef DLLEXPORT
#if ((defined(_M_IX86) || defined(__i386__)) || \
     (defined(_M_AMD64) || defined(__x86_64__))) && !defined(MIDL_PASS)
# if defined(_MSC_VER)
#  define DLLEXPORT __declspec(dllexport)
# elif (defined(__clang__) || defined(__GNUC__))
#  define DLLEXPORT __attribute__((dllexport))
# else
#  define DLLEXPORT __attribute__((visibility("default")))
# endif
#else
# define DLLEXPORT
#endif // ((_M_IX86 || __i386__) || (_M_AMD64 || __x86_64__)) && !MIDL_PASS
#endif // !DLLEXPORT

#ifndef NORETURN
#ifndef __WIDL__
# if (defined(__clang__) || defined(__GNUC__)) && !defined(_MSC_VER)
#  define NORETURN __attribute__((noreturn))
# elif defined(_MSC_VER) && (_MSC_VER >= 1200)
#  define NORETURN __declspec(noreturn)
# else
#  define NORETURN
# endif
#else
# define NORETURN
#endif // !__WIDL__
#endif // !NORETURN

#ifndef NOINLINE
#ifndef __WIDL__
# if (defined(__clang__) || defined(__GNUC__)) && !defined(_MSC_VER)
#  define NOINLINE __noinline
# elif defined(_MSC_VER) && (_MSC_VER >= 1300)
#  define NOINLINE __declspec(noinline)
# else
#  define NOINLINE
# endif
#else
# define NOINLINE
#endif // !__WIDL__
#endif // !NORETURN

#ifndef NOTHROW
# if (GNUC_PREREQ(3, 3) || defined(__clang__))
#  define NOTHROW     __attribute__((__nothrow__))
# elif (MSC_PREREQ(12, 0) && defined(__cplusplus))
#  define NOTHROW     __declspec(nothrow)
# else
#  define NOTHROW
# endif
#endif // !NOTHROW

/* __EXTENSION__ type specifier */
#ifdef __WIDL__
#define __EXTENSION__
#else
#if (defined(__clang__) || defined(__GNUC__)) && !defined(_MSC_VER)
# define EXTENSION __extension__
#else
# if !(defined(__clang__) || defined(__GNUC__))
#  define __extension__
# endif
# define EXTENSION
#endif
#endif // __WIDL__

/* Packed compiler dependent definition */
#if (defined(__GNUC__) || defined(__clang__)) && !defined(_MSC_VER)
# define PACKED __packed
#else
# if !(defined(__clang__) || defined(__GNUC__))
#  define __packed
# endif
# define PACKED
#endif

/* Section definition */
#ifndef SECTION
#define SECTION __section
#endif

/* Packing push and pop macros */
#if !(defined(lint) || defined(RC_INVOKED))
#if (defined(__clang__) || defined(__GNUC__))
#define PACK_PUSH_1 _Pragma("pack(push,1)")
#else
#define PACK_PUSH_1 __pragma(pack(push,1))
#endif
#else
#error PACK_PUSH_1 cant be used!
#endif // !(lint || RC_INVOKED)

#if !(defined(lint) || defined(RC_INVOKED))
#if (defined(__clang__) || defined(__GNUC__))
#define PACK_PUSH_2 _Pragma("pack(push,2)")
#else
#define PACK_PUSH_2 __pragma(pack(push,2))
#endif
#else
#error PACK_PUSH_2 cant be used!
#endif // !(lint || RC_INVOKED)

#if !(defined(lint) || defined(RC_INVOKED))
#if (defined(__clang__) || defined(__GNUC__))
#define PACK_PUSH_4 _Pragma("pack(push,4)")
#else
#define PACK_PUSH_4 __pragma(pack(push,4))
#endif
#else
#error PACK_PUSH_4 cant be used!
#endif // !(lint || RC_INVOKED)

#if !(defined(lint) || defined(RC_INVOKED))
#if (defined(__clang__) || defined(__GNUC__))
#define PACK_PUSH_8 _Pragma("pack(push,8)")
#else
#define PACK_PUSH_8 __pragma(pack(push,8))
#endif
#else
#error PACK_PUSH_8 cant be used!
#endif // !(lint || RC_INVOKED)

#if !(defined(lint) || defined(RC_INVOKED))
#if (defined(__clang__) || defined(__GNUC__))
#define PACK_PUSH_16 _Pragma("pack(push,16)")
#else
#define PACK_PUSH_16 __pragma(pack(push,16))
#endif
#else
#error PACK_PUSH_16 cant be used!
#endif // !(lint || RC_INVOKED)

#if !(defined(lint) || defined(RC_INVOKED))
#if (defined(__clang__) || defined(__GNUC__))
#define PACK_POP _Pragma("pack(pop)")
#else
#define PACK_POP __pragma(pack(pop))
#endif
#else
#error PACK_POP cant be used!
#endif // !(lint || RC_INVOKED)

/* Force undefined symbols */
#if defined(_MSC_VER)
#if defined(__X64__)
#define FORCE_UNDEFINED_SYMBOL(x) __pragma(comment(linker, "/INCLUDE:" #x))
#else
#define FORCE_UNDEFINED_SYMBOL(x) __pragma(comment(linker, "/INCLUDE:_" #x))
#endif
#else
#define FORCE_UNDEFINED_SYMBOL(x) void* __ ## x ## _fp = (void*)&x
#endif

#ifndef SYSTEM_CACHE_ALIGNMENT_SIZE
#if (defined(_M_AMD64) || defined(__x86_64__)) || (defined(_M_X86) || defined(__i386__))
#define SYSTEM_CACHE_ALIGNMENT_SIZE 64
#else
#define SYSTEM_CACHE_ALIGNMENT_SIZE 128
#endif
#endif



#ifndef __linux__
/* Sync with Linux. */

#define BUG_ON(cond)    do {    \
    if (!!(cond))               \
        __ud2();                \
} while (0)

// Sets a break point that works only when a debugger is present
#if !defined(DBG_BREAK)
#if defined(_DEBUG)
#define DBG_BREAK() __debugbreak()
#else
#define DBG_BREAK() (void*)(0)
#endif
#endif

#define DBG_BREAK_ON(cond) do {         \
    if (!!(cond)) {                     \
        DBG_BREAK();                    \
    }                                   \
} while(0)
#endif

#if !defined(lint)

#if defined(__CLION_IDE__)

//
// Trick the parser into not erroring on MSVC SEH keywords
//

#ifndef __try
#define __try {
#endif
#ifndef __except
#define __except(x) } /* Idk why this works, but it does */
#endif
#ifndef __finally
#define __finally __finally
#endif

#endif // __CLION_IDE__

#endif // !lint

#endif // _COMPILER_H_
