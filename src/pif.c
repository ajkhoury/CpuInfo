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
 * @file pif.c
 * @author Aidan Khoury (ajkhoury)
 * @date 11/16/2018
 */

#include "pif.h"

#include <string.h>

typedef struct _CPUID_INFO {
    UINT32 Eax, Ebx, Ecx, Edx;
} CPUID_INFO, *PCPUID_INFO;

typedef enum _CPU_VENDOR {
    CpuVendorUnsupported = 0,
    CpuVendorIntel = 1,
    CpuVendorAmd = 2,
} CPU_VENDOR;


static UINT32 CpuidMaxFunction = 0;
static UINT32 CpuidMaxExtendedFunction = 0;
static PCPUID_INFO CpuidCache = NULL;
static PCPUID_INFO CpuidExtendedCache = NULL;

static CPU_VENDOR CpuVendor = CpuVendorUnsupported;
static CHAR CpuVendorString[32] = { 0 };
static CHAR CpuBrandString[64] = { 0 };

// Defined in pif.h and intiialized in PifInitialize.
UINT32 CpuidFn_00000001h_0_Ecx = 0;
UINT32 CpuidFn_00000001h_0_Edx = 0;

UINT32 CpuidFn_00000007h_0_Ebx = 0;
UINT32 CpuidFn_00000007h_0_Ecx = 0;
UINT32 CpuidFn_00000007h_0_Edx = 0;

UINT32 CpuidFn_0000000Dh_1_Ebx = 0;

UINT32 CpuidFn_80000001h_0_Ecx = 0;
UINT32 CpuidFn_80000001h_0_Edx = 0;

UINT32 CpuidFn_80000008h_0_Ebx = 0;


#define CPU_INFO(X) \
    CpuidCache[(X)-CPUID_MAX_FUNCTION]

#define CPU_EXTENDED_INFO(X) \
    CpuidExtendedCache[(X)-CPUID_MAX_EXTENDED_FUNCTION]


FORCEINLINE
VOID
PifpDestroy(
    VOID
)
{
    if (CpuidExtendedCache != NULL)
    {
        free( CpuidExtendedCache );
        CpuidExtendedCache = NULL;
    }

    if (CpuidCache != NULL)
    {
        free( CpuidCache );
        CpuidCache = NULL;
    }

    CpuidMaxExtendedFunction = 0;
    CpuidMaxFunction = 0;
}


STATUS
PIFAPI
PifInitialize(
    VOID
)
{
    UINT32 Index;
    CPUID_INFO CpuInfo;
    STATUS Status;

    //
    // Get the number of the highest valid ID.
    //
    __cpuid( (int*)&CpuInfo, CPUID_MAX_FUNCTION );
    CpuidMaxFunction = CpuInfo.Eax;

    //
    // Allocate memory for the CPUID functions data.
    //
    CpuidCache = malloc( sizeof( CPUID_INFO ) * (CpuidMaxFunction - CPUID_MAX_FUNCTION + 1) );
    if (!CpuidCache)
    {
        return E_NOMEM;
    }

    Status = STATUS_OK;

    //
    // Cache the data for the CPUID functions.
    //
    for (Index = CPUID_MAX_FUNCTION; Index <= CpuidMaxFunction; ++Index)
    {
        __cpuidex( (int*)&CPU_INFO( Index ), Index, 0 );
    }

    //
    // Capture CPU vendor.
    //
    memset( CpuVendorString, 0, sizeof( CpuVendorString ) );
    *(int*)(CpuVendorString + sizeof(int) * 0) = CPU_INFO( CPUID_SIGNATURE ).Ebx;
    *(int*)(CpuVendorString + sizeof(int) * 1) = CPU_INFO( CPUID_SIGNATURE ).Edx;
    *(int*)(CpuVendorString + sizeof(int) * 2) = CPU_INFO( CPUID_SIGNATURE ).Ecx;

    if (strncmp( CpuVendorString, "GenuineIntel", 12 ) == 0)
    {
        CpuVendor = CpuVendorIntel;
    }
    else if (strncmp( CpuVendorString, "AuthenticAMD", 12 ) == 0)
    {
        CpuVendor = CpuVendorAmd;
    }

    //
    // Load bitset with flags for CPUID function 0x00000001 sub-function 0.
    //
    if (CpuidMaxFunction >= CPUID_FEATURES)
    {
        CpuidFn_00000001h_0_Ecx = CPU_INFO( CPUID_FEATURES ).Ecx;
        CpuidFn_00000001h_0_Edx = CPU_INFO( CPUID_FEATURES ).Edx;
    }

    //
    // Load bitset with flags for CPUID function 0x00000007 sub-function 0.
    //
    if (CpuidMaxFunction >= CPUID_STRUCTURED_EXTENDED_FEATURES)
    {
        CpuidFn_00000007h_0_Ebx = CPU_INFO( CPUID_STRUCTURED_EXTENDED_FEATURES ).Ebx;
        CpuidFn_00000007h_0_Ecx = CPU_INFO( CPUID_STRUCTURED_EXTENDED_FEATURES ).Ecx;
        CpuidFn_00000007h_0_Edx = CPU_INFO( CPUID_STRUCTURED_EXTENDED_FEATURES ).Edx;
    }

    //
    // Load bitset with flags in EBX for CPUID function 0x0000000D sub-function 1.
    //
    if (CpuidMaxFunction >= CPUID_EXTENDED_STATE)
    {
        __cpuidex( (int*)&CpuInfo, CPUID_EXTENDED_STATE, CPUID_EXTENDED_STATE_SUB_LEAF );
        CpuidFn_0000000Dh_1_Ebx = CpuInfo.Ebx;
    }

    //
    // Get the number of the highest valid extended ID.
    //
    __cpuid( (int*)&CpuInfo, CPUID_MAX_EXTENDED_FUNCTION );
    CpuidMaxExtendedFunction = CpuInfo.Eax;

    //
    // Allocate memory for the extended CPUID functions data.
    //
    CpuidExtendedCache = malloc( sizeof( CPUID_INFO ) * (CpuidMaxExtendedFunction - CPUID_MAX_EXTENDED_FUNCTION + 1) );
    if (!CpuidExtendedCache)
    {
        Status = E_NOMEM;
        goto Exit;
    }

    //
    // Cache the data for the extended CPUID functions.
    //
    for (Index = CPUID_MAX_EXTENDED_FUNCTION; Index <= CpuidMaxExtendedFunction; ++Index)
    {
        __cpuidex( (int*)&CPU_EXTENDED_INFO( Index ), Index, 0);
    }

    //
    // Load bitset with flags for function 0x80000001.
    //
    if (CpuidMaxExtendedFunction >= CPUID_EXTENDED_FEATURES)
    {
        CpuidFn_80000001h_0_Ecx = CPU_EXTENDED_INFO( CPUID_EXTENDED_FEATURES ).Ecx;
        CpuidFn_80000001h_0_Edx = CPU_EXTENDED_INFO( CPUID_EXTENDED_FEATURES ).Edx;
    }

    //
    // Interpret CPU brand string, if reported.
    //
    memset( CpuBrandString, 0, sizeof( CpuBrandString ) );
    if (CpuidMaxExtendedFunction >= CPUID_BRAND_STRING3)
    {
        memcpy( CpuBrandString + sizeof(CPUID_INFO) * 0, &CPU_EXTENDED_INFO( CPUID_BRAND_STRING1 ), sizeof( CPUID_INFO ) );
        memcpy( CpuBrandString + sizeof(CPUID_INFO) * 1, &CPU_EXTENDED_INFO( CPUID_BRAND_STRING2 ), sizeof( CPUID_INFO ) );
        memcpy( CpuBrandString + sizeof(CPUID_INFO) * 2, &CPU_EXTENDED_INFO( CPUID_BRAND_STRING3 ), sizeof( CPUID_INFO ) );
    }

    //
    // Load bitset with flags in EBX for function 0x80000008.
    //
    if (CpuidMaxExtendedFunction >= CPUID_EXTENDED_FEATURES_EXTENSION)
    {
        CpuidFn_80000008h_0_Ebx = CPU_EXTENDED_INFO( CPUID_EXTENDED_FEATURES_EXTENSION ).Ebx;
    }

Exit:
    //
    // Destroy resources allocated during initialization.
    //
    PifpDestroy( );

    return Status;
}

STATUS
PIFAPI
PifGetVendorString(
    OUT CHAR *VendorString,
    IN SIZE_T VendorStringMaxSize
)
{
    return strcpy_s( VendorString, VendorStringMaxSize, CpuVendorString );
}

STATUS
PIFAPI
PifGetBrandString(
    OUT CHAR *BrandString,
    IN SIZE_T BrandStringMaxSize
)
{
    return strcpy_s( BrandString, BrandStringMaxSize, CpuBrandString );
}
