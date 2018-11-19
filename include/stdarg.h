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
 * @file stdarg.h
 * @author Aidan Khoury (ajkhoury)
 * @date 11/16/2018 
 */

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef _INC_STDARG
#define _INC_STDARG

#include "vadefs.h"

#if defined(_MSC_VER)
#pragma pack(push, 8)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef va_start
#define va_start    __base_va_start
#define va_arg      __base_va_arg
#define va_end      __base_va_end
/* Copy va_list into another variable of this type */
#define va_copy(dest, src)  ((dest) = (src))
#endif // va_start

#if defined(__cplusplus)
} // extern "C"
#endif

#if defined(_MSC_VER)
#pragma pack(pop)
#endif

#endif // _INC_STDARG