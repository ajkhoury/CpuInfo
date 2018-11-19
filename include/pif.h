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
 * @file pif.h
 * @author Aidan Khoury (ajkhoury)
 * @date 11/16/2018 
 */

#ifndef _PIF_H_
#define _PIF_H_

#include "arch.h"

// Processor Identification and Features API calling convention.
#define PIFAPI BLAPI

extern UINT32 CpuidFn_00000001h_0_Ecx;
extern UINT32 CpuidFn_00000001h_0_Edx;

extern UINT32 CpuidFn_00000007h_0_Ebx;
extern UINT32 CpuidFn_00000007h_0_Ecx;
extern UINT32 CpuidFn_00000007h_0_Edx;

extern UINT32 CpuidFn_0000000Dh_1_Ebx;

extern UINT32 CpuidFn_80000001h_0_Ecx;
extern UINT32 CpuidFn_80000001h_0_Edx;

extern UINT32 CpuidFn_80000008h_0_Ebx;


STATUS
PIFAPI
PifInitialize(
    VOID
    );

VOID
PIFAPI
PifDestroy(
    VOID
    );

STATUS
PIFAPI
PifGetVendorString(
    OUT CHAR *VendorString,
    IN SIZE_T VendorStringMaxSize
    );

STATUS
PIFAPI
PifGetBrandString(
    OUT CHAR *BrandString,
    IN SIZE_T BrandStringMaxSize
    );

#define HasSSE3()           ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_SSE3))
#define HasPCLMULQDQ()      ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_PCLMULQDQ))
#define HasMONITOR()        ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_MONITOR))
#define HasMWAIT()          ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_MONITOR))
#define HasVMX()            ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_VMX))
#define HasSMX()            ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_SMX))
#define HasEIST()           ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_EIST))
#define HasSSSE3()          ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_SSSE3))
#define HasFMA()            ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_FMA))
#define HasCMPXCHG16B()     ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_CMPXCHG16B))
#define HasSSE41()          ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_SSE41))
#define HasSSE42()          ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_SSE42))
#define HasMOVBE()          ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_MOVBE))
#define HasPOPCNT()         ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_POPCNT))
#define HasAES()            ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_AES))
#define HasXSAVE()          ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_XSAVE))
#define HasOSXSAVE()        ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_OSXSAVE))
#define HasAVX()            ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_AVX))
#define HasF16C()           ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_F16C))
#define HasRDRAND()         ((BOOLEAN)(CpuidFn_00000001h_0_Ecx & X86_FEATURE_RDRND))

#define HasMSR()            ((BOOLEAN)(CpuidFn_00000001h_0_Edx & X86_FEATURE_MSR))
#define HasCMPXCHG8B()      ((BOOLEAN)(CpuidFn_00000001h_0_Edx & X86_FEATURE_CMPXCHG8B))
#define HasSEP()            ((BOOLEAN)(CpuidFn_00000001h_0_Edx & X86_FEATURE_SEP))
#define HasCMOV()           ((BOOLEAN)(CpuidFn_00000001h_0_Edx & X86_FEATURE_CMOV))
#define HasCLFSH()          ((BOOLEAN)(CpuidFn_00000001h_0_Edx & X86_FEATURE_CLFSH))
#define HasMMX()            ((BOOLEAN)(CpuidFn_00000001h_0_Edx & X86_FEATURE_MMX))
#define HasFXSR()           ((BOOLEAN)(CpuidFn_00000001h_0_Edx & X86_FEATURE_FXSR))
#define HasFXSAVE()         ((BOOLEAN)(CpuidFn_00000001h_0_Edx & X86_FEATURE_FXSAVE))
#define HasSSE()            ((BOOLEAN)(CpuidFn_00000001h_0_Edx & X86_FEATURE_SSE))
#define HasSSE2()           ((BOOLEAN)(CpuidFn_00000001h_0_Edx & X86_FEATURE_SSE2))

#define HasFSGSBASE()       ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_FSGSBASE))
#define HasTSCADJUST()      ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_TSCADJUST))
#define HasSGX()            ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_SGX))
#define HasBMI1()           ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_BMI1))
#define HasBMI()            ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_BMI))
#define HasHLE()            ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_HLE))
#define HasAVX2()           ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_AVX2))
#define HasBMI2()           ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_BMI2))
#define HasERMS()           ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_ERMS))
#define HasINVPCID()        ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_INVPCID))
#define HasRTM()            ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_RTM))
#define HasMPX()            ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_MPX))
#define HasAVX512F()        ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_AVX512F))
#define HasRDSEED()         ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_RDSEED))
#define HasADX()            ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_ADX))
#define HasAVX512PF()       ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_AVX512PF))
#define HasAVX512ER()       ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_AVX512ER))
#define HasAVX512CD()       ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_AVX512CD))
#define HasSHA()            ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_SHA))
#define HasAVX512BW()       ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_AVX512BW))
#define HasAVX512VL()       ((BOOLEAN)(CpuidFn_00000007h_0_Ebx & X86_FEATURE_AVX512VL))

#define HasPREFETCHWT1()    ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_PREFTCHWT1))
#define HasAVX512VBMI1()    ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_AVX512VBMI1))
#define HasAVX512VBMI()     ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_AVX512VBMI))
#define HasUMIP()           ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_UMIP))
#define HasPKU()            ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_PKU))
#define HasAVX512VBMI2()    ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_AVX512VBMI2))
#define HasGFNI()           ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_GFNI))
#define HasVAES()           ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_VAES))
#define HasVPCLMULQDQ()     ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_VPCLMULQDQ))
#define HasAVX512VNNI()     ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_AVX512VNNI))
#define HasAVX512BITALG()   ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_AVX512BITALG))
#define HasAVX512VPOPCNTDQ() ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_AVX512VPOPCNTDQ))
#define HasRDPID()          ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_RDPID))
#define HasSGXLC()          ((BOOLEAN)(CpuidFn_00000007h_0_Ecx & X86_FEATURE_SGXLC))

#define HasLAHF_LM()        ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_LAHF_LM))
#define HasSVM()            ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_SVM))
#define HasABM()            ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_ABM))
#define HasLZCNT()          ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_LZCNT))
#define HasSSE4a()          ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_SSE4A))
#define HasSSE4A()          ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_SSE4A))
#define HasMisalignedSSE()  ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_MISALIGNSSE))
#define HasPRFCHW()         ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_PRFCHW))
#define HasPREFETCHW()      ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_PRFCHW))
#define HasXOP()            ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_XOP))
#define HasSKINIT()         ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_SKINIT))
#define HasFMA4()           ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_FMA4))
#define HasTCE()            ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_TCE))
#define HasTBM()            ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_TBM))
#define HasDBX()            ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_DBX))
#define HasMONITORX()       ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_MONITORX))
#define HasMWAITX()         ((BOOLEAN)(CpuidFn_80000001h_0_Ecx & X86_FEATURE_MONITORX))

#define HasSYSCALL()        ((BOOLEAN)(CpuidFn_80000001h_0_Edx & X86_FEATURE_SEPEXT))
#define HasMTRR()           ((BOOLEAN)(CpuidFn_80000001h_0_Edx & X86_FEATURE_MTRREXT))
#define HasNOEXECUTE()      ((BOOLEAN)(CpuidFn_80000001h_0_Edx & X86_FEATURE_NOEXECUTE))
#define HasMMXEXT()         ((BOOLEAN)(CpuidFn_80000001h_0_Edx & X86_FEATURE_MMXEXT))
#define HasRDTSCP()         ((BOOLEAN)(CpuidFn_80000001h_0_Edx & X86_FEATURE_RDTSCP))
#define HasLONGMODE()       ((BOOLEAN)(CpuidFn_80000001h_0_Edx & X86_FEATURE_LONGMODE))
#define Has3DNOWEXT()       ((BOOLEAN)(CpuidFn_80000001h_0_Edx & X86_FEATURE_3DNOWEXT))
#define Has3DNOW()          ((BOOLEAN)(CpuidFn_80000001h_0_Edx & X86_FEATURE_3DNOW))

#define IsFeatureSupported(_XX) \
    Has##_XX( )

#define IsFeatureSupportedMessage(_XX) \
    printf( #_XX " is%s\n", (Has##_XX( )) ? " supported" : " not supported" )

#endif // _PIF_H_
