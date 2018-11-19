/**
 * CpuInfo
 * Copyright (c) 2006-2018, Intel Corporation. All rights reserved.
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
 * @file types.h
 * @authors EDK2 Team
 *          LLVM Team
 *          Aidan Khoury (ajkhoury)
 * @date 11/15/2018 
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#include "compiler.h"
#include "error.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4668) // #if not_defined treated as #if 0
#pragma warning(disable:4201) // nonstandard extension used: nameless struct/union
#endif

//
// Calling convention for Baselib
//
#if (defined(__x86_64__) || defined(_M_AMD64)) && \
    (defined(__GNUC__) || defined(__clang__))
#define BLAPI __attribute__((ms_abi))
#else
#define BLAPI __cdecl
#endif


//
// Pointer type specifiers
//
#if !defined(_MAC) && (defined(_M_MRX000) || (defined(_M_AMD64) || defined(__x86_64__))) && (_MSC_VER >= 1100) && \
    !(defined(MIDL_PASS) || defined(RC_INVOKED)) && !(defined(__clang__) || defined(__GNUC__))
 #define POINTER_64 __ptr64
 typedef unsigned __int64 POINTER_64_INT;
 #if (defined(_M_AMD64) || defined(__x86_64__)) && !(defined(__clang__) || defined(__GNUC__))
  #define POINTER_32 __ptr32
 #else
  #define POINTER_32
 #endif
#else
 #if defined(_MAC) && defined(_MAC_INT_64) && !(defined(__clang__) || defined(__GNUC__))
  #define POINTER_64 __ptr64
  typedef unsigned __int64 POINTER_64_INT;
 #else
  #if (_MSC_VER >= 1300) && !(defined(MIDL_PASS) || defined(RC_INVOKED)) && !(defined(__clang__) || defined(__GNUC__))
   #define POINTER_64 __ptr64
  #else
   #define POINTER_64
  #endif
  typedef unsigned long POINTER_64_INT;
 #endif
 #define POINTER_32
#endif

#if (defined(_M_AMD64) || defined(__x86_64__))
#define FIRMWARE_PTR
#else
#define FIRMWARE_PTR POINTER_32
#endif

#if (_MSC_FULL_VER >= 140041204) && !defined(MIDL_PASS) && !defined(RC_INVOKED) && \
    !(defined(__clang__) || defined(__GNUC__))
#define POINTER_SIGNED __sptr
#define POINTER_UNSIGNED __uptr
#else
#define POINTER_SIGNED
#define POINTER_UNSIGNED
#endif

#define SPOINTER_32 POINTER_SIGNED POINTER_32
#define UPOINTER_32 POINTER_UNSIGNED POINTER_32

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86) || defined(_ARM_) || defined(_M_ARM)) && (_MSC_VER >= 1300) && \
    !(defined(__clang__) || defined(__GNUC__))
#define _W64 __w64
#else
#define _W64
#endif
#endif


//
// Basic NULL type
//

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif


//
// Const type specifier
//

#ifndef CONST
#define CONST   const
#endif


//
// Prototype argument decorations for parameters to indicate their direction
//
//      IN       - argument is passed into the function
//      OUT      - argument (pointer) is returned from the function
//      OPTIONAL - argument is optional
//

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef OPTIONAL
#define OPTIONAL
#endif


//
// Useless FAR definition
//

#ifndef FAR
#define FAR
#endif


//
// Basic boolean types
//

typedef unsigned char boolean;
typedef boolean BOOLEAN;
typedef BOOLEAN *PBOOLEAN;
typedef int BOOL;
#ifndef __cplusplus
typedef _Bool bool;
#define true 1
#define false 0
#elif defined(__GNUC__) && !defined(__STRICT_ANSI__)
/* Define _Bool, bool, false, true as a GNU extension. */
#define _Bool bool
#define bool  bool
#define false false
#define true  true
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


//
// Helpful macros
//

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C    extern "C"
#else
#define EXTERN_C    extern
#endif
#endif


//
// Calculate the address of the base of the structure given its type, and an
// address of a field within the structure.
//

#define CONTAINING_RECORD(address, type, field) ((type *)((UINT8*)(address) - \
                                                  (UINTN)(&((type *)0)->field)))


//
// Determine if an argument is present by testing the value of the pointer
// to the argument value.
//

#define ARGUMENT_PRESENT(ArgumentPointer)    (\
    (CHAR*)((UINTN)(ArgumentPointer)) != (CHAR*)(NULL) )

//
// Calculate the byte offset of a field in a structure of type type.
//
#if defined(__x86_64__) || defined(_M_X64) || defined(__ia64__) || defined(_M_IA64) || \
    defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64)
#define FIELD_OFFSET(type, field)   ((size_t)((long long)(&(((type *)0)->field))))
#else
#define FIELD_OFFSET(type,field)    ((size_t)(&(((type *)0)->field)))
#endif

//
// Calculate the size of a field in a structure of type type, without
// knowing or stating the type of the field.
//
#define RTL_FIELD_SIZE(type, field) (sizeof(((type *)0)->field))
#define FIELD_SIZEOF(t,f)           (sizeof(((t*)0)->f))

//
// Calculate the size of a structure of type type up through and
// including a field.
//
#define RTL_SIZEOF_THROUGH_FIELD(type, field)   (FIELD_OFFSET(type, field) + RTL_FIELD_SIZE(type, field))

//
//  RTL_CONTAINS_FIELD usage:
//
//      if (RTL_CONTAINS_FIELD(pBlock, pBlock->cbSize, dwMumble))
//          // safe to use pBlock->dwMumble
//
#define RTL_CONTAINS_FIELD(Struct, Size, Field) \
    ((((char*)(&(Struct)->Field)) + sizeof((Struct)->Field)) <= (((char*)(Struct)) + (Size)))

//
// Return the number of elements in a statically sized array.
//   ULONG Buffer[100];
//   RTL_NUMBER_OF(Buffer) == 100
// This is also popularly known as: NUMBER_OF, ARRSIZE, _countof, NELEM, etc.
//
#define RTL_NUMBER_OF_V1(A) (sizeof(A)/sizeof((A)[0]))
#if defined(__cplusplus) && defined(_MSC_VER) && (_MSC_FULL_VER >= 13009466) \
 && !defined(RC_INVOKED) && !defined(_PREFAST_)
//
// RtlpNumberOf is a function that takes a reference to an array of N Ts.
//
// typedef T array_of_T[N];
// typedef array_of_T &reference_to_array_of_T;
//
// RtlpNumberOf returns a pointer to an array of N chars.
// We could return a reference instead of a pointer but older compilers do not accept that.
//
// typedef char array_of_char[N];
// typedef array_of_char *pointer_to_array_of_char;
//
// sizeof(array_of_char) == N
// sizeof(*pointer_to_array_of_char) == N
//
// pointer_to_array_of_char RtlpNumberOf(reference_to_array_of_T);
//
// We never even call RtlpNumberOf, we just take the size of dereferencing its return type.
// We do not even implement RtlpNumberOf, we just decare it.
//
// Attempts to pass pointers instead of arrays to this macro result in compile clock errors.
// That is the point.
//
extern "C++" // templates cannot be declared to have 'C' linkage
#if defined(__x86_64__) || defined(_M_X64) || defined(_M_IA64) || defined(_M_ARM) || defined(_M_ARM64)
template <typename T, unsigned long long N>
#else
template <typename T, unsigned int N>
#endif
char( *RtlpNumberOf( UNALIGNED T( &)[N] ) )[N];
#define RTL_NUMBER_OF_V2(A) (sizeof(*RtlpNumberOf(A)))
#else
#define RTL_NUMBER_OF_V2(A) RTL_NUMBER_OF_V1(A)
#endif
#ifdef ENABLE_RTL_NUMBER_OF_V2
#define RTL_NUMBER_OF(A) RTL_NUMBER_OF_V2(A)
#else
#define RTL_NUMBER_OF(A)    RTL_NUMBER_OF_V1(A)
#endif
// ARRAYSIZE is more readable version of RTL_NUMBER_OF_V2, and uses
// it regardless of ENABLE_RTL_NUMBER_OF_V2
// _ARRAYSIZE is a version useful for anonymous types
#define ARRAYSIZE(A)        RTL_NUMBER_OF_V2(A)
#define _ARRAYSIZE(A)       RTL_NUMBER_OF_V1(A)
#define ARRAY_SIZE(A)       (sizeof(A) / sizeof(A[0]))

// Same as FIELD_OFFSET
#ifndef _offsetof
#define _offsetof(type, field)  FIELD_OFFSET(type, field)
#endif
// Same as _ARRAYSIZE
#ifndef _countof
#define _countof(_Array)        _ARRAYSIZE(_Array)
#endif

/**
 * Get offset of a field within a structure
 * @v type      Structure type
 * @v field     Field within structure
 * @ret offset  Offset within structure
 */
#if (defined(__GNUC__) && (__GNUC__ > 3)) || defined(__clang__)
#define offsetof(type, field)   __builtin_offsetof(type, field)
#else
#define offsetof(type, field)   FIELD_OFFSET(type, field)
#endif


//
// Basic wide-char types
//
#ifndef _WCHAR_T_DEFINED
#if (defined(__clang__) || defined(__GNUC__))
typedef __WCHAR_TYPE__ wchar_t;
#else
typedef unsigned short wchar_t;
#endif
#define _WCHAR_T_DEFINED
#endif  /* _WCHAR_T_DEFINED */

#ifndef _WCTYPE_T_DEFINED
#define _WCTYPE_T_DEFINED
#if (defined(__clang__) || defined(__GNUC__))
typedef __WINT_TYPE__ wint_t;
#else
/* wint_t is unsigned short for compatibility with MS runtime */
typedef unsigned short wint_t;
#endif
typedef unsigned short wctype_t;
#endif  /* _WCTYPE_T_DEFINED */

//
// Basic Type Sizes
//

#ifndef __CHAR_BIT__
#define CHAR_BIT            8 // number of bits in a char
#define __CHAR_BIT__        CHAR_BIT
#else
#define CHAR_BIT            __CHAR_BIT__
#endif

#define BITS_PER_BYTE       CHAR_BIT

#ifndef __SIZEOF_LONG__
#define __SIZEOF_LONG__     sizeof(long)
#endif
#define BITS_PER_LONG       (BITS_PER_BYTE * __SIZEOF_LONG__)

#ifndef __SIZEOF_LONGLONG__
#define __SIZEOF_LONGLONG__ sizeof(__int64)
#endif
#define BITS_PER_LONGLONG   (BITS_PER_BYTE * __SIZEOF_LONGLONG__)

#if defined(__x86_64__) || defined(_M_AMD64)
#define __DWORDSIZE             64
#define __DWORDSIZE_COMPAT32    1
#else
#define __DWORDSIZE             32
#endif

//
// Target specific macro replacement for type "long".  In the Windows API,
// the type long is always 32 bit, even if the target is 64 bit (LLP64).
// On 64 bit Cygwin, the type long is 64 bit (LP64).  So, to get the right
// sized definitions and declarations, all usage of type long in the Windows
// headers have to be replaced by the below defined macro __LONG32.
//
#ifndef __LP64__ /* 32 bit target, 64 bit Mingw target */
#define __LONG32 long
#else /* 64 bit Cygwin target */
#define __LONG32 int
#endif


//
// Below are truly Linux-specific types that should never collide with
// any application/library that wants linux/types.h.
//
#ifdef __CHECKER__
#define __bitwise__ __attribute__((bitwise))
#else
#define __bitwise__
#endif
#define __bitwise __bitwise__

//
// aligned_u64 should be used in defining kernel<->userspace ABIs to avoid
// common 32/64-bit compat problems.
// 64-bit values align to 4-byte boundaries on x86_32 (and possibly other
// architectures) and to 8-byte boundaries on 64-bit architectures.  The new
// aligned_64 type enforces 8-byte alignment so that structs containing
// aligned_64 values have the same alignment on 32-bit and 64-bit architectures.
// No conversions are necessary between 32-bit user-space and a 64-bit kernel.
//
#define __aligned_u64 __u64 __attribute__((aligned(8)))
#define __aligned_be64 __be64 __attribute__((aligned(8)))
#define __aligned_le64 __le64 __attribute__((aligned(8)))

#if !defined(_MSC_EXTENSIONS)
#define __int64 long long
#define __int32 int
#define __int16 short
#define __int8 char
#endif // !_MSC_EXTENSIONS


/** 7.18.1.1  Exact-width integer types */
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
__extension__ typedef __int64 int64_t;
__extension__ typedef unsigned __int64 uint64_t;

typedef signed char sint8_t;
typedef signed short sint16_t;
typedef signed int sint32_t;
__extension__ typedef signed __int64 sint64_t;

/** 7.18.1.2 Minimum-width integer types */
typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;
typedef short int_least16_t;
typedef unsigned short uint_least16_t;
typedef int int_least32_t;
typedef unsigned uint_least32_t;
__extension__ typedef __int64 int_least64_t;
__extension__ typedef unsigned __int64 uint_least64_t;

/**
 * 7.18.1.3  Fastest minimum-width integer types
 * Not actually guaranteed to be fastest for all purposes
 * Here we use the exact-width types for 8 and 16-bit ints.
 */
typedef char int_fast8_t;
typedef unsigned char uint_fast8_t;
typedef short int_fast16_t;
typedef unsigned short uint_fast16_t;
typedef int int_fast32_t;
typedef unsigned int uint_fast32_t;
__extension__ typedef __int64 int_fast64_t;
__extension__ typedef unsigned __int64 uint_fast64_t;

/** 7.18.1.4  Integer types capable of holding object pointers */
#ifndef _INTPTR_T_DEFINED
#define _INTPTR_T_DEFINED
#ifndef __intptr_t_defined
#define __intptr_t_defined
#undef intptr_t
#if defined(__x86_64__) || defined(_M_X64)
__extension__ typedef __int64 intptr_t;
#else
__extension__ typedef int intptr_t;
#endif /* __x86_64__ || _M_X64 */
#endif /* __intptr_t_defined */
#endif /* _INTPTR_T_DEFINED */

#ifndef _UINTPTR_T_DEFINED
#define _UINTPTR_T_DEFINED
#ifndef __uintptr_t_defined
#define __uintptr_t_defined
#undef uintptr_t
#if defined(__x86_64__) || defined(_M_X64)
__extension__ typedef unsigned __int64 uintptr_t;
#else
__extension__ typedef unsigned int uintptr_t;
#endif /* __x86_64__ || _M_X64 */
#endif /* __uintptr_t_defined */
#endif /* _UINTPTR_T_DEFINED */

#ifndef _PTRDIFF_T_DEFINED
#define _PTRDIFF_T_DEFINED
#ifndef _PTRDIFF_T_
#define _PTRDIFF_T_
#undef ptrdiff_t
#if defined(__x86_64__) || defined(_M_X64)
__extension__ typedef __int64 ptrdiff_t;
#else
__extension__ typedef int ptrdiff_t;
#endif /* __x86_64__ || _M_X64 */
#endif /* _PTRDIFF_T_ */
#endif /* _PTRDIFF_T_DEFINED */

/** 7.18.1.5  Greatest-width integer types */
#ifndef _INTMAX_T_DEFINED
__extension__ typedef __int64 intmax_t;
__extension__ typedef unsigned __int64 uintmax_t;
#define _INTMAX_T_DEFINED
#endif

/** Size types */
#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
#undef size_t
#if (defined(__x86_64__) || defined(_M_AMD64))
__extension__ typedef unsigned __int64 size_t;
#else
__extension__ typedef unsigned int size_t;
#endif /* __x86_64__ || _M_X64 */
#endif /* _SIZE_T_DEFINED */

#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
#undef ssize_t
#if defined(__x86_64__) || defined(_M_AMD64)
__extension__ typedef __int64 ssize_t;
#else
__extension__ typedef int ssize_t;
#endif /* __x86_64__ || _M_X64 */
#endif /* _SSIZE_T_DEFINED */

#ifndef _RSIZE_T_DEFINED
typedef size_t rsize_t;
#define _RSIZE_T_DEFINED
#endif

typedef uint64_t        UINT64, *PUINT64;
typedef int64_t         INT64, *PINT64;

#ifndef _BASETSD_H_
typedef uint32_t        UINT32, *PUINT32;
typedef int32_t         INT32, *PINT32;
#endif

typedef uint16_t        UINT16, *PUINT16;
typedef int16_t         INT16, *PINT16;

typedef uint8_t         UINT8, *PUINT8;
typedef int8_t          INT8, *PINT8;

#if defined(_M_X64) || defined(__x86_64__)
typedef uint64_t        UINTN, *PUINTN;
typedef int64_t         INTN, *PINTN;

typedef uint64_t        UINT_PTR, *PUINT_PTR;
typedef int64_t         INT_PTR, *PINT_PTR;
#else
typedef uint32_t        UINTN, *PUINTN;
typedef int32_t         INTN, *PINTN;

typedef uint32_t        UINT_PTR, *PUINT_PTR;
typedef int32_t         INT_PTR, *PINT_PTR;
#endif

typedef size_t          SIZE_T, *PSIZE_T;
typedef ssize_t         SSIZE_T, *PSSIZE_T;

#define BAD_POINTER     0xFBFBFBFBFBFBFBFB
#define MAX_ADDRESS     0xFFFFFFFFFFFFFFFF

#define INVALID_INDEX   ((UINT32)(~(0)))

//
// Windows Flavour Basics
//

typedef void* PVOID;

#ifndef VOID
#define VOID void
typedef char CHAR;
//typedef short SHORT;
//typedef long LONG;
//#if !defined(MIDL_PASS)
//typedef int INT;
//#endif // !MIDL_PASS
#endif // !VOID

//typedef unsigned long   ULONG;
//typedef long long       LONGLONG;
//typedef unsigned long long ULONGLONG;


//
// The following types are guaranteed to be signed and 32 bits wide.
//

typedef signed __LONG32 LONG32, *PLONG32;

//
// The following types are guaranteed to be unsigned and 32 bits wide.
//

typedef unsigned __LONG32 ULONG32, *PULONG32;


//
// UNICODE (Wide Character) type
//

#ifndef _MAC
typedef wchar_t         WCHAR;
#else
// some Macintosh compilers don't define wchar_t in a convenient location, or define it as a char
typedef unsigned short  WCHAR;    // wc,   16-bit UNICODE character
#endif

//
//  UCS (Universal Character Set) types
//

typedef unsigned long UCSCHAR;


//
// HALF_PTR is half the size of a pointer it intended for use with
// within structures which contain a pointer and two small fields.
// UHALF_PTR is the unsigned variation.
//

#if defined(_M_AMD64) || defined(__x86_64__)

#define ADDRESS_TAG_BIT ((UINT64)0x40000000000)

typedef __int64 SHANDLE_PTR;
typedef unsigned __int64 HANDLE_PTR;
typedef unsigned int UHALF_PTR, *PUHALF_PTR;
typedef int HALF_PTR, *PHALF_PTR;

#if !defined(__midl)
__forceinline
unsigned long
HandleToULong(
    const void *h
    )
{
    return((unsigned long) (UINTN) h );
}

__forceinline
long
HandleToLong(
    const void *h
    )
{
    return((long) (INTN) h );
}

__forceinline
void *
ULongToHandle(
    const unsigned long h
    )
{
    return((void *) (UINTN) h );
}


__forceinline
void *
LongToHandle(
    const long h
    )
{
    return((void *) (INTN) h );
}


__forceinline
unsigned long
PtrToUlong(
    const void  *p
    )
{
    return((unsigned long) (UINTN) p );
}

__forceinline
unsigned int
PtrToUint(
    const void  *p
    )
{
    return((unsigned int) (UINTN) p );
}

__forceinline
unsigned short
PtrToUshort(
    const void  *p
    )
{
    return((unsigned short) (unsigned long) (UINTN) p );
}

__forceinline
long
PtrToLong(
    const void  *p
    )
{
    return((long) (INTN) p );
}

__forceinline
int
PtrToInt(
    const void  *p
    )
{
    return((int) (INTN) p );
}

__forceinline
short
PtrToShort(
    const void  *p
    )
{
    return((short) (long) (INTN) p );
}

__forceinline
void *
IntToPtr(
    const int i
    )
// Caution: IntToPtr() sign-extends the int value.
{
    return( (void *)(INTN)i );
}

__forceinline
void *
UIntToPtr(
    const unsigned int ui
    )
// Caution: UIntToPtr() zero-extends the unsigned int value.
{
    return( (void *)(UINTN)ui );
}

__forceinline
void *
LongToPtr(
    const long l
    )
// Caution: LongToPtr() sign-extends the long value.
{
    return( (void *)(INTN)l );
}

__forceinline
void *
ULongToPtr(
    const unsigned long ul
    )
// Caution: ULongToPtr() zero-extends the unsigned long value.
{
    return( (void *)(UINTN)ul );
}

#define PtrToPtr64( p )         ((void * POINTER_64) p)
#define Ptr64ToPtr( p )         ((void *) p)
#define HandleToHandle64( h )   (PtrToPtr64( h ))
#define Handle64ToHandle( h )   (Ptr64ToPtr( h ))

__forceinline
void *
Ptr32ToPtr(
    const void * POINTER_32 p
    )
{
    return((void *) (UINTN) (unsigned long) p);
}

__forceinline
void *
Handle32ToHandle(
    const void * POINTER_32 h
    )
{
    return((void *) (INTN) (long) h);
}

__forceinline
void * POINTER_32
PtrToPtr32(
    const void *p
    )
{

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-void-pointer-cast"
#endif

    return((void * POINTER_32) (unsigned long) (UINTN) p);

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

}

#define HandleToHandle32( h )       (PtrToPtr32( h ))

#endif // !_midl

#else  // !(_M_AMD64 || __x86_64__)

#define ADDRESS_TAG_BIT ((UINT32)0x80000000)

typedef unsigned short UHALF_PTR, *PUHALF_PTR;
typedef short HALF_PTR, *PHALF_PTR;
typedef _W64 long SHANDLE_PTR;
typedef _W64 unsigned long HANDLE_PTR;

#define HandleToULong( h ) ((unsigned long)(UINTN)(h) )
#define HandleToLong( h )  ((long)(INTN) (h) )
#define ULongToHandle( ul ) ((HANDLE)(UINTN) (ul) )
#define LongToHandle( h )   ((HANDLE)(INTN) (h) )
#define PtrToUlong( p ) ((unsigned long)(UINTN) (p) )
#define PtrToLong( p )  ((long)(INTN) (p) )
#define PtrToUint( p ) ((unsigned int)(UINTN) (p) )
#define PtrToInt( p )  ((int)(INTN) (p) )
#define PtrToUshort( p ) ((unsigned short)(UINTN)(p) )
#define PtrToShort( p )  ((short)(INTN)(p) )
#define IntToPtr( i )    ((VOID*)(INTN)((int)i))
#define UIntToPtr( ui )  ((VOID*)(UINTN)((unsigned int)ui))
#define LongToPtr( l )   ((VOID*)(INTN)((long)l))
#define ULongToPtr( ul ) ((VOID*)(UINTN)((unsigned long)ul))

#if !defined(__midl)
__forceinline
void * POINTER_64
PtrToPtr64(
    const void *p
    )
{
    return((void * POINTER_64) (unsigned __int64) (UINTN)p );
}

__forceinline
void *
Ptr64ToPtr(
    const void * POINTER_64 p
    )
{
    return((void *) (UINTN) (unsigned __int64) p);
}

__forceinline
void * POINTER_64
HandleToHandle64(
    const void *h
    )
{
    return((void * POINTER_64)(__int64)(INTN)h );
}

__forceinline
void *
Handle64ToHandle(
    const void * POINTER_64 h
    )
{
    return((void *) (UINTN) (unsigned __int64) h );
}
#endif

#define Ptr32ToPtr( p )         ((void *) p)
#define Handle32ToHandle( h )   (Ptr32ToPtr( h ))
#define PtrToPtr32( p )         ((void * POINTER_32) p)
#define HandleToHandle32( h )   (PtrToPtr32( h ))

#endif // (_M_AMD64 || __x86_64__)

#define HandleToUlong(h)  HandleToULong(h)
#define HandleToUInt(h)  ((unsigned int)HandleToULong(h))
#define HandleToUint(h)  ((unsigned int)HandleToUlong(h))
#define UlongToHandle(ul) ULongToHandle(ul)
#define UlongToPtr(ul) ULongToPtr(ul)
#define UintToPtr(ui)  UIntToPtr(ui)


/** 7.18.2  Limits of specified-width integer types */
#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)

/** 7.18.2.1  Limits of exact-width integer types */
#define INT8_MIN    (-128)
#define INT16_MIN   (-32768)
#define INT32_MIN   (-2147483647-1)
#define INT64_MIN   (-9223372036854775807LL-1)

#define INT8_MAX    127
#define INT16_MAX   32767
#define INT32_MAX   2147483647
#define INT64_MAX   9223372036854775807LL

#define UINT8_MAX   255
#define UINT16_MAX  65535
#define UINT32_MAX  0xffffffffU  /* 4294967295U */
#define UINT64_MAX  0xffffffffffffffffULL /* 18446744073709551615ULL */

/** 7.18.2.2  Limits of minimum-width integer types */
#define INT_LEAST8_MIN INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define INT_LEAST8_MAX INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define UINT_LEAST8_MAX UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

/** 7.18.2.3  Limits of fastest minimum-width integer types */
#define INT_FAST8_MIN INT8_MIN
#define INT_FAST16_MIN INT16_MIN
#define INT_FAST32_MIN INT32_MIN
#define INT_FAST64_MIN INT64_MIN

#define INT_FAST8_MAX INT8_MAX
#define INT_FAST16_MAX INT16_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST64_MAX INT64_MAX

#define UINT_FAST8_MAX UINT8_MAX
#define UINT_FAST16_MAX UINT16_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT64_MAX

/** 7.18.2.4  Limits of integer types capable of holding object pointers */
#if defined(__x86_64__) || defined(_M_X64)
#define INTPTR_MIN INT64_MIN
#define INTPTR_MAX INT64_MAX
#define UINTPTR_MAX UINT64_MAX
#else
#define INTPTR_MIN INT32_MIN
#define INTPTR_MAX INT32_MAX
#define UINTPTR_MAX UINT32_MAX
#endif

/** 7.18.2.5 Limits of greatest-width integer types */
#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX

/** 7.18.3 Limits of other integer types */
#if defined(__x86_64__) || defined(_M_X64)
#define PTRDIFF_MIN INT64_MIN
#define PTRDIFF_MAX INT64_MAX
#else
#define PTRDIFF_MIN INT32_MIN
#define PTRDIFF_MAX INT32_MAX
#endif

#define SIG_ATOMIC_MIN INT32_MIN
#define SIG_ATOMIC_MAX INT32_MAX

#ifndef SIZE_MAX
#if defined(__x86_64__) || defined(_M_X64)
#define SIZE_MAX UINT64_MAX
#else
#define SIZE_MAX UINT32_MAX
#endif
#endif

#ifndef WCHAR_MIN  /* also in wchar.h */
#define WCHAR_MIN 0U
#define WCHAR_MAX 0xffffU
#endif

//
// wint_t is unsigned short for compatibility with MS runtime
//
#define WINT_MIN 0U
#define WINT_MAX 0xffffU

#endif /* !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS) */

//
// stdint flavor limits
//
#define MB_LEN_MAX      4                           // UTF8 can require up to 4 bytes

#define SCHAR_MIN       (-0x7f-1)                   // max value for a signed char
#define SCHAR_MAX       0x7f                        // min value for a signed char
#define UCHAR_MAX       0xff                        // max value for an unsigned char
#define CHAR_MAX        0xff                        // max value for a char
#define CHAR_MIN        0                           // min value for a char

#define USHRT_MAX       0xffff                      // max value for an unsigned short
#define SHRT_MAX        0x7fff                      // max value for a short
#define SHRT_MIN        (-0x7fff-1)                 // min value for a short

#define UINT_MAX        0xffffffffU                 // max value for an unsigned int
#define INT_MAX         0x7fffffff                  // max value for an int
#define INT_MIN         (-0x7fffffff-1)             // min value for an int

#define ULONG_MAX       (0xffffffff)                // max value for an unsigned long
#define LONG_MAX        (2147483647L)               // max value for a long
#define LONG_MIN        (-2147483647-1)             // min value for a long

#define ULLONG_MAX      0xffffffffffffffffULL       // max unsigned long long
#define LLONG_MAX       0x7fffffffffffffffLL        // max signed long long
#define LLONG_MIN       (-0x7fffffffffffffffLL-1)   // min signed long long

#ifdef _MSC_VER
typedef __int64 quad;
typedef unsigned __int64 uquad;
#else
typedef long long quad;
typedef unsigned long long uquad;
#endif
#define QUAD_MIN        ((quad)-9223372036854775807) // minimum (signed) quad value
#define QUAD_MAX        ((quad) 9223372036854775807) // maximum (signed) quad value
#define UQUAD_MAX       ((uquad)0xffffffffffffffff) // maximum unsigned quad value

//
// Windows Flavour Limits
//
#define MAXUINT8        ((UINT8)~((UINT8)0))
#define MAXINT8         ((INT8)(MAXUINT8 >> 1))
#define MININT8         ((INT8)~MAXINT8)

#define MAXUINT16       ((UINT16)~((UINT16)0))
#define MAXINT16        ((INT16)(MAXUINT16 >> 1))
#define MININT16        ((INT16)~MAXINT16)

#define MAXUINT32       ((UINT32)~((UINT32)0))
#define MAXINT32        ((INT32)(MAXUINT32 >> 1))
#define MININT32        ((INT32)~MAXINT32)

#define MAXUINT64       ((UINT64)~((UINT64)0))
#define MAXINT64        ((INT64)(MAXUINT64 >> 1))
#define MININT64        ((INT64)~MAXINT64)

#define MAXSIZE_T       ((SIZE_T)~((SIZE_T)0))
#define MAXSSIZE_T      ((SSIZE_T)(MAXSIZE_T >> 1))
#define MINSSIZE_T      ((SSIZE_T)~MAXSSIZE_T)

#define MAXUINT         ((UINT)~((UINT)0))
#define MAXINT          ((INT)(MAXUINT >> 1))
#define MININT          ((INT)~MAXINT)

#define MAXUINTN        (~((UINTN)0))
#define MAXINTN         ((INTN)(MAXUINTN >> 1))
#define MININTN         (~MAXINTN)


/** 7.18.4  Macros for integer constants */
#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)

/** 7.18.4.1  Macros for minimum-width integer constants
 *
Accoding to Douglas Gwyn <gwyn@arl.mil>:
"This spec was changed in ISO/IEC 9899:1999 TC1; in ISO/IEC
9899:1999 as initially published, the expansion was required
to be an integer constant of precisely matching type, which
is impossible to accomplish for the shorter types on most
platforms, because C99 provides no standard way to designate
an integer constant with width less than that of type int.
TC1 changed this to require just an integer constant
*expression* with *promoted* type."

The trick used here is from Clive D W Feather.
*/

#define INT8_C( val ) (INT_LEAST8_MAX-INT_LEAST8_MAX+(val))
#define INT16_C( val ) (INT_LEAST16_MAX-INT_LEAST16_MAX+(val))
#define INT32_C( val ) (INT_LEAST32_MAX-INT_LEAST32_MAX+(val))
/** The 'trick' doesn't work in C89 for long long because,
 * without suffix, val will be evaluated as int, not intmax_t
 */
#define INT64_C( val ) val##LL

#define UINT8_C( val ) (val)
#define UINT16_C( val ) (val)
#define UINT32_C( val ) (val##U)
#define UINT64_C( val ) val##ULL

/** 7.18.4.2  Macros for greatest-width integer constants */
#define INTMAX_C( val ) val##LL
#define UINTMAX_C( val ) val##ULL

#endif /* !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS) */




//
// Time types
//
typedef unsigned int clock_t;
typedef long long int time_t;
typedef int timer_t;

//
// Major, minor numbers, dev_t's.
//
typedef int32_t __devmajor_t, __devminor_t;
#define devmajor_t __devmajor_t
#define devminor_t __devminor_t

//
// Status value types
//
#ifndef STATUS_DEFINED
#define STATUS_DEFINED

typedef long status_t;
typedef status_t STATUS;
typedef STATUS *PSTATUS;

#ifndef SUCCESS
#define SUCCESS(Status)     ((Status) >= 0)
#endif

#endif // !STATUS_DEFINED


//
// Local character comparison macros.
//
#define IsCharZero(C)       (((C) & 0x000000ff) == 0x00000000)
#define IsCharMinus1(C)     (((C) & 0x000000ff) == 0x000000ff)
#define IsCharLtrZero(C)    (((C) & 0x00000080) == 0x00000080)
#define IsCharGtrZero(C)    (!IsCharLtrZero(C) && !IsCharZero(C))

#define Add2Ptr(P,I)        ((VOID*)((UINT8*)(P) + (I)))
#define PtrOffset(B,O)      ((UINT32)((UINT32)(O) - (UINT32)(B)))

//
// Type alignment support macros.
//
#define WordAlign(P) (                  \
    ((((SIZE_T)(P)) + 1) & 0xFFFFFFFE)  \
)
#define LongAlign(P) (                  \
    ((((SIZE_T)(P)) + 3) & 0xFFFFFFFC)  \
)
#define QuadAlign(P) (                  \
    ((((SIZE_T)(P)) + 7) & 0xFFFFFFF8)  \
)

//
// Global Unique Identifier
//
#ifndef _GUID_T
#define _GUID_T

#ifndef GUID_DEFINED
#define GUID_DEFINED
#if defined(__midl)
typedef struct {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    byte           Data4[ 8 ];
} GUID;
#else
typedef struct _GUID {
    UINT32 Data1;       // 0x00
    UINT16 Data2;       // 0x04
    UINT16 Data3;       // 0x06
    UINT8  Data4[ 8 ];  // 0x08
} GUID; // sizeof=0x10
#endif // __midl
#endif // GUID_DEFINED

typedef GUID *PGUID;
typedef GUID FAR *LPGUID;
typedef CONST GUID FAR *LPCGUID;

#ifdef INITGUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID DECLSPEC_SELECTANY name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#else
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID FAR name
#endif // INITGUID

#ifndef DEFINE_GUIDEX
#define DEFINE_GUIDEX(name) EXTERN_C const GUID name
#endif // !defined(DEFINE_GUIDEX)

#ifndef STATICGUIDOF
#define STATICGUIDOF(guid) STATIC_##guid
#endif // !defined(STATICGUIDOF)

#if defined(__cplusplus) && (_MSC_VER >= 1100)
#define DEFINE_GUIDSTRUCT(g, n) struct __declspec(uuid(g)) n
#define DEFINE_GUIDNAMED(n) __uuidof(struct n)
#else // !defined(__cplusplus)
#define DEFINE_GUIDSTRUCT(g, n) DEFINE_GUIDEX(n)
#define DEFINE_GUIDNAMED(n) n
#endif // !defined(__cplusplus)

#define STATIC_GUID_NULL \
    0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#if defined(_MSC_VER)
#pragma prefast(suppress: 6244, "duplicate definition is harmless in this case")
#endif // _MSC_VER

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif

DEFINE_GUIDSTRUCT("00000000-0000-0000-0000-000000000000", GUID_NULL);
#define GUID_NULL DEFINE_GUIDNAMED(GUID_NULL)

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef GUID IID;
typedef IID *LPIID;
#define IID_NULL            GUID_NULL
#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)
typedef GUID CLSID;
typedef CLSID *LPCLSID;
#define CLSID_NULL          GUID_NULL
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)
typedef GUID FMTID;
typedef FMTID *LPFMTID;
#define FMTID_NULL          GUID_NULL
#define IsEqualFMTID(rfmtid1, rfmtid2) IsEqualGUID(rfmtid1, rfmtid2)

#ifdef __midl_proxy
#define __MIDL_CONST
#else
#define __MIDL_CONST const
#endif

#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#ifdef __cplusplus
#define REFGUID const GUID &
#else
#define REFGUID const GUID * __MIDL_CONST
#endif
#endif

#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED
#ifdef __cplusplus
#define REFIID const IID &
#else
#define REFIID const IID * __MIDL_CONST
#endif
#endif

#ifndef _REFCLSID_DEFINED
#define _REFCLSID_DEFINED
#ifdef __cplusplus
#define REFCLSID const IID &
#else
#define REFCLSID const IID * __MIDL_CONST
#endif
#endif

#ifndef _REFFMTID_DEFINED
#define _REFFMTID_DEFINED
#ifdef __cplusplus
#define REFFMTID const IID &
#else
#define REFFMTID const IID * __MIDL_CONST
#endif
#endif

#endif // !__IID_DEFINED__

#define InlineIsEqualGUID(rguid1, rguid2)  \
        (((unsigned long *)(rguid1))[0] == ((unsigned long *)(rguid2))[0] &&   \
        ((unsigned long *)(rguid1))[1] == ((unsigned long *)(rguid2))[1] &&    \
        ((unsigned long *)(rguid1))[2] == ((unsigned long *)(rguid2))[2] &&    \
        ((unsigned long *)(rguid1))[3] == ((unsigned long *)(rguid2))[3])

#define IsEqualGUID(rguid1, rguid2) (!memcmp((rguid1), (rguid2), sizeof(GUID)))

#endif // !_GUID_T

//
// UUID types
//
#ifndef _UUID_T
#define _UUID_T
typedef uint8_t uuid_t[16];
typedef UINT8 UUID[16];
#endif /* _UUID_T */

//
// Large Integer Types.
//
#ifndef _LARGE_INTEGER_T
#define _LARGE_INTEGER_T

typedef union _LARGE_INTEGER {
    struct {
        UINT32 LowPart;
        INT32 HighPart;
    };
    struct {
        UINT32 LowPart;
        INT32 HighPart;
    } u;
    INT64 QuadPart;
} LARGE_INTEGER;

typedef LARGE_INTEGER *PLARGE_INTEGER;

typedef union _ULARGE_INTEGER {
    struct {
        UINT32 LowPart;
        UINT32 HighPart;
    };
    struct {
        UINT32 LowPart;
        UINT32 HighPart;
    } u;
    UINT64 QuadPart;
} ULARGE_INTEGER;

typedef ULARGE_INTEGER *PULARGE_INTEGER;

#endif // _LARGE_INTEGER_T

#ifndef _PHYSICAL_ADDRESS_T
#define _PHYSICAL_ADDRESS_T

//
// Physical address.
//
typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;

#endif // _PHYSICAL_ADDRESS_T


//
// Helper unions for accessing low and high bits of 64 bit types
//

typedef union _int64_u {
    struct {
        uint32_t low;
        int32_t high;
    };
    struct {
        uint32_t low;
        int32_t high;
    } u;
    int64_t quad;
} int64_u;

typedef union _uint64_u {
    struct {
        uint32_t low;
        uint32_t high;
    };
    struct {
        uint32_t low;
        uint32_t high;
    } u;
    uint64_t quad;
} uint64_u;

typedef union _int128_u {
    struct {
        uint64_t low;
        int64_t high;
    };
    struct {
        uint64_t low;
        int64_t high;
    } u;
    int32_t ints[4];
} int128_u;

typedef union _uint128_u {
    struct {
        uint64_t low;
        uint64_t high;
    };
    struct {
        uint64_t low;
        uint64_t high;
    } u;
    uint32_t ints[4];
} uint128_u;


//
// EFI/Windows Flavour
//

typedef union _INT128 {
    struct {
        UINT64 Low;
        INT64 High;
    };
    struct {
        UINT64 Low;
        INT64 High;
    } u;
    INT32 Int32[4];
} INT128;

typedef union _UINT128 {
    struct {
        UINT64 Low;
        UINT64 High;
    };
    struct {
        UINT64 Low;
        UINT64 High;
    } u;
    UINT32 Uint32[4];
} UINT128;


//
// Definition for common 16bit segment:offset pointers.
//

typedef union __segoff_t {
    struct {
        uint16_t offset;
        uint16_t segment;
    };
    uint32_t segoff;
} segoff_t;

typedef union _SEGOFF {
    struct {
        UINT16 Offset;
        UINT16 Segment;
    };
    UINT32 SegmentOffset;
} SEGOFF;


//
// Locale Information
//

struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct *pthreadlocinfo;
typedef struct threadmbcinfostruct *pthreadmbcinfo;
struct __lc_time_data;

#ifndef _TAGLC_ID_DEFINED
typedef struct tagLC_ID {
    unsigned short wLanguage;
    unsigned short wCountry;
    unsigned short wCodePage;
} LC_ID, *LPLC_ID;
#define _TAGLC_ID_DEFINED
#endif /* _TAGLC_ID_DEFINED */

#ifndef _THREADLOCALEINFO
typedef struct threadlocaleinfostruct {
    int refcount;
    unsigned int lc_codepage;
    unsigned int lc_collate_cp;
    unsigned long lc_handle[6]; /* LCID */
    LC_ID lc_id[6];
    struct {
        char *locale;
        WCHAR *wlocale;
        int *refcount;
        int *wrefcount;
    } lc_category[6];
    int lc_clike;
    int mb_cur_max;
    int *lconv_intl_refcount;
    int *lconv_num_refcount;
    int *lconv_mon_refcount;
    struct lconv *lconv;
    int *ctype1_refcount;
    unsigned short *ctype1;
    const unsigned short *pctype;
    const unsigned char *pclmap;
    const unsigned char *pcumap;
    struct __lc_time_data *lc_time_curr;
} threadlocinfo;
#define _THREADLOCALEINFO
#endif /* _THREADLOCALEINFO */


//
// XxBiByte Values
//
#define KIBIBYTE            (1024)
#define MEBIBYTE            (1024*1024)
#define GIBIBYTE            (1024*1024*1024)

//
// More Helpful Macros
//

//
// Define macros to build data structure signatures from characters.
//
#define SIGNATURE_16(A,B)               (((B) << 8) | (A))
#define SIGNATURE_32(A,B,C,D)           ((SIGNATURE_16(C, D) << 16) | SIGNATURE_16(A, B))
#define SIGNATURE_64(A,B,C,D,E,F,G,H)   (((UINT64)(SIGNATURE_32(E, F, G, H)) << 32) | SIGNATURE_32(A, B, C, D))

#ifndef __MAX
#define __MAX(a, b)             ((a) > (b) ? (a) : (b))
#endif
#ifndef __MIN
#define __MIN(a, b)             ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)                (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)                (((a) < (b)) ? (a) : (b))
#endif

//
// Define generic alignment macros.
//
#define ALIGN_MASK(v,m)         (((v) + (m)) & ~(m))
#define ALIGN(v,a)              ALIGN_MASK(v, (a) - 1LL)
#define ALIGN_DOWN(v,a)         ((v) & ~((a) - 1LL))
#define IS_ALIGNED(v,a)         (ALIGN_DOWN((v), (a)) == (v))

#define DIV_ROUND_UP(n,d)       (((n) + (d) - 1) / (d))

#define LOW_U16_U64( x )        ((uint64_t)(x) & 0xFFFF)
#define MID_U16_U64( x )        (((uint64_t)(x) >> 16) & 0xFFFF)
#define HIGH_U32_U64( x )       ((uint64_t)(x) >> 32)

#define BYTE1(a)                ((UINT8)((a)&0xFF))
#define BYTE2(a)                ((UINT8)(((a)>>8)&0xFF))
#define BYTE3(a)                ((UINT8)(((a)>>16)&0xFF))
#define BYTE4(a)                ((UINT8)(((a)>>24)&0xFF))
#define BYTE5(a)                ((UINT8)(((a)>>32)&0xFF))
#define BYTE6(a)                ((UINT8)(((a)>>40)&0xFF))
#define BYTE7(a)                ((UINT8)(((a)>>48)&0xFF))
#define BYTE8(a)                ((UINT8)(((a)>>56)&0xFF))

#define WORD1(a)                ((UINT16)((UINT32)(a)&0xFFFF))
#define WORD2(a)                ((UINT16)(((UINT32)(a)>>16)&0xFFFF))
#define WORD3(a)                ((UINT16)(((UINT64)(a)>>32)&0xFFFF))
#define WORD4(a)                ((UINT16)(((UINT64)(a)>>48)&0xFFFF))

#define LONG1(a)                ((UINT32)(((UINT64)(a))&0xFFFFFFFF))
#define LONG2(a)                ((UINT32)((((UINT64)(a))>>32)&0xFFFFFFFF))

#define DWORD1(a)               ((UINT32)(((UINT64)(a))&0xFFFFFFFF))
#define DWORD2(a)               ((UINT32)((((UINT64)(a))>>32)&0xFFFFFFFF))

#define BIT(n)                  (1U << (n))
#define BIT_MASK_LONG(nr)       (1UL << ((nr) % BITS_PER_LONG))
#define BIT_MASK_LONGLONG(nr)   (1ULL << ((nr) % BITS_PER_LONGLONG))
#define BIT_LONG(nr)            ((nr) / BITS_PER_LONG)
#define BIT_LONGLONG(nr)        ((nr) / BITS_PER_LONGLONG)
#define BITS_TO_BYTES(nr)       DIV_ROUND_UP((nr), BITS_PER_BYTE)
#define BITS_TO_LONGS(nr)       DIV_ROUND_UP((nr), BITS_PER_LONG)
#define BITS_TO_LONGLONGS(nr)   DIV_ROUND_UP((nr), BITS_PER_LONGLONG)

#define BIT0                    0x00000001
#define BIT1                    0x00000002
#define BIT2                    0x00000004
#define BIT3                    0x00000008
#define BIT4                    0x00000010
#define BIT5                    0x00000020
#define BIT6                    0x00000040
#define BIT7                    0x00000080
#define BIT8                    0x00000100
#define BIT9                    0x00000200
#define BIT10                   0x00000400
#define BIT11                   0x00000800
#define BIT12                   0x00001000
#define BIT13                   0x00002000
#define BIT14                   0x00004000
#define BIT15                   0x00008000
#define BIT16                   0x00010000
#define BIT17                   0x00020000
#define BIT18                   0x00040000
#define BIT19                   0x00080000
#define BIT20                   0x00100000
#define BIT21                   0x00200000
#define BIT22                   0x00400000
#define BIT23                   0x00800000
#define BIT24                   0x01000000
#define BIT25                   0x02000000
#define BIT26                   0x04000000
#define BIT27                   0x08000000
#define BIT28                   0x10000000
#define BIT29                   0x20000000
#define BIT30                   0x40000000
#define BIT31                   0x80000000
#define BIT32                   0x0000000100000000ULL
#define BIT33                   0x0000000200000000ULL
#define BIT34                   0x0000000400000000ULL
#define BIT35                   0x0000000800000000ULL
#define BIT36                   0x0000001000000000ULL
#define BIT37                   0x0000002000000000ULL
#define BIT38                   0x0000004000000000ULL
#define BIT39                   0x0000008000000000ULL
#define BIT40                   0x0000010000000000ULL
#define BIT41                   0x0000020000000000ULL
#define BIT42                   0x0000040000000000ULL
#define BIT43                   0x0000080000000000ULL
#define BIT44                   0x0000100000000000ULL
#define BIT45                   0x0000200000000000ULL
#define BIT46                   0x0000400000000000ULL
#define BIT47                   0x0000800000000000ULL
#define BIT48                   0x0001000000000000ULL
#define BIT49                   0x0002000000000000ULL
#define BIT50                   0x0004000000000000ULL
#define BIT51                   0x0008000000000000ULL
#define BIT52                   0x0010000000000000ULL
#define BIT53                   0x0020000000000000ULL
#define BIT54                   0x0040000000000000ULL
#define BIT55                   0x0080000000000000ULL
#define BIT56                   0x0100000000000000ULL
#define BIT57                   0x0200000000000000ULL
#define BIT58                   0x0400000000000000ULL
#define BIT59                   0x0800000000000000ULL
#define BIT60                   0x1000000000000000ULL
#define BIT61                   0x2000000000000000ULL
#define BIT62                   0x4000000000000000ULL
#define BIT63                   0x8000000000000000ULL


#ifdef __cplusplus
}
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif // _TYPES_H_
