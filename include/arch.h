/**
 * CpuInfo
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
 * @file arch.h
 * @author Aidan Khoury (ajkhoury)
 * @date 11/15/2018 
 */

#ifndef _ARCH_H_
#define _ARCH_H_
#if defined(_MSC_VER)
#pragma once
#endif

#include "types.h"

/**
 * Page Size/Shift
 */
#ifndef PAGE_SIZE
#define PAGE_SIZE           0x1000
#endif
#ifndef PAGE_SHIFT
#define PAGE_SHIFT          12
#endif
#ifndef PAGE_MASK
#define PAGE_MASK           (~(PAGE_SIZE-1))
#endif

#define PAGE_4KB_SIZE       (4 * KIBIBYTE)
#define PAGE_4KB_MASK       (PAGE_4KB_SIZE - 1)

#define PAGE_2MB_SIZE       (2 * MEBIBYTE)
#define PAGE_2MB_MASK       (PAGE_2MB_SIZE - 1)

#define PAGE_4MB_SIZE       (4 * MEBIBYTE)
#define PAGE_4MB_MASK       (PAGE_4MB_SIZE - 1)

#define PAGE_1GB_SIZE       (1 * GIBIBYTE)
#define PAGE_1GB_MASK       (PAGE_1GB_SIZE - 1)

// Include x86 architecture defintions.
#include "arch/x86.h"

// Include CPUID defintions.
#include "arch/cpuid.h"

// Include intrinsics.
#include <intrin.h>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4214) // nonstandard extension used: bit field types other than int
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Enables CPU interrupts.
 */
#define EnableInterrupts( )      _enable()

/**
 * Disables CPU interrupts.
 */
#define DisableInterrupts( )     _disable()

// .....
// .....
// .....

#ifdef __cplusplus
} // extern "C" {
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#endif // _ARCH_H_
