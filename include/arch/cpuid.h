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
 * @file cpuid.h
 * @author Aidan Khoury (ajkhoury)
 * @date 11/16/2018 
 */

#ifndef _ARCH_CPUID_H_
#define _ARCH_CPUID_H_

/**
 * CPUID Info Types
 */
#define CPUID_REG_EAX                               0
#define CPUID_REG_EBX                               1
#define CPUID_REG_ECX                               2
#define CPUID_REG_EDX                               3
#define CPUID_REG_MAX                               4

#define CPUID_SIGNATURE                             0x00
#define CPUID_MAX_FUNCTION                          0x00

#define CPUID_VERSION_INFO                          0x01
#define CPUID_FEATURES                              0x01

#define CPUID_CACHE_INFO                            0x02

#define CPUID_SERIAL_NUMBER                         0x03

#define CPUID_CACHE_PARAMS                          0x04

#define CPUID_MONITOR_MWAIT                         0x05

#define CPUID_THERMAL_POWER_MANAGEMENT              0x06

#define CPUID_STRUCTURED_EXTENDED_FEATURES          0x07
#define CPUID_STRUCTURED_EXTENDED_FEATURES_SUB_LEAF_INFO 0x00

#define CPUID_DIRECT_CACHE_ACCESS_INFO              0x09

#define CPUID_ARCHITECTURAL_PERFORMANCE_MONITORING  0x0A

#define CPUID_EXTENDED_TOPOLOGY                     0x0B
#define CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_INVALID  0x00
#define CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_SMT      0x01
#define CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_CORE     0x02

#define CPUID_EXTENDED_STATE                        0x0D
#define CPUID_EXTENDED_STATE_MAIN_LEAF              0x00
#define CPUID_EXTENDED_STATE_SUB_LEAF               0x01
#define CPUID_EXTENDED_STATE_SIZE_OFFSET            0x02

#define CPUID_INTEL_RDT_MONITORING                  0x0F
#define CPUID_INTEL_RDT_MONITORING_ENUMERATION_SUB_LEAF 0x00
#define CPUID_INTEL_RDT_MONITORING_L3_CACHE_SUB_LEAF 0x01

#define CPUID_INTEL_RDT_ALLOCATION                  0x10
#define CPUID_INTEL_RDT_ALLOCATION_ENUMERATION_SUB_LEAF 0x00
#define CPUID_INTEL_RDT_ALLOCATION_L3_CACHE_SUB_LEAF 0x01
#define CPUID_INTEL_RDT_ALLOCATION_L2_CACHE_SUB_LEAF 0x02

#define CPUID_INTEL_SGX                             0x12
#define CPUID_INTEL_SGX_CAPABILITIES_0_SUB_LEAF     0x00
#define CPUID_INTEL_SGX_CAPABILITIES_1_SUB_LEAF     0x01
#define CPUID_INTEL_SGX_CAPABILITIES_RESOURCES_SUB_LEAF 0x02

#define CPUID_INTEL_PROCESSOR_TRACE                 0x14
#define CPUID_INTEL_PROCESSOR_TRACE_MAIN_LEAF       0x00
#define CPUID_INTEL_PROCESSOR_TRACE_SUB_LEAF        0x01

#define CPUID_TIME_STAMP_COUNTER                    0x15

#define CPUID_PROCESSOR_FREQUENCY                   0x16

#define CPUID_SOC_VENDOR                            0x17
#define CPUID_SOC_VENDOR_MAIN_LEAF                  0x00
#define CPUID_SOC_VENDOR_BRAND_STRING1              0x01
#define CPUID_SOC_VENDOR_BRAND_STRING2              0x02
#define CPUID_SOC_VENDOR_BRAND_STRING3              0x03

#define CPUID_HV_VENDOR_INFO                        0x40000000
#define CPUID_HV_INTERFACE_INFO                     0x40000001
#define CPUID_HV_VERSION_INFO                       0x40000002
#define CPUID_HV_TIMER_INFO                         0x40000010

#define CPUID_MAX_EXTENDED_FUNCTION                 0x80000000
#define CPUID_EXTENDED_FEATURES                     0x80000001

#define CPUID_BRAND_STRING1                         0x80000002
#define CPUID_BRAND_STRING2                         0x80000003
#define CPUID_BRAND_STRING3                         0x80000004

#define CPUID_EXTENDED_CACHE_INFO                   0x80000006
#define CPUID_EXTENDED_CACHE_INFO_ECX_L2_ASSOCIATIVITY_DISABLED 0x00
#define CPUID_EXTENDED_CACHE_INFO_ECX_L2_ASSOCIATIVITY_DIRECT_MAPPED 0x01
#define CPUID_EXTENDED_CACHE_INFO_ECX_L2_ASSOCIATIVITY_2_WAY 0x02
#define CPUID_EXTENDED_CACHE_INFO_ECX_L2_ASSOCIATIVITY_4_WAY 0x04
#define CPUID_EXTENDED_CACHE_INFO_ECX_L2_ASSOCIATIVITY_8_WAY 0x06
#define CPUID_EXTENDED_CACHE_INFO_ECX_L2_ASSOCIATIVITY_16_WAY 0x08
#define CPUID_EXTENDED_CACHE_INFO_ECX_L2_ASSOCIATIVITY_FULL 0x0F

#define CPUID_EXTENDED_TIME_STAMP_COUNTER           0x80000007

#define CPUID_VIR_PHY_ADDRESS_SIZE                  0x80000008
#define CPUID_EXTENDED_FEATURES_EXTENSION           0x80000008


/**
 * CPUID Vendor Signatures
 */

//
// MS HV:   "Microsoft Hv"
//
#define CPUID_SIGNATURE_MS_HV_EBX       0x7263694D // 'Micr'
#define CPUID_SIGNATURE_MS_HV_EDX       0x666F736F // 'osof'
#define CPUID_SIGNATURE_MS_HV_ECX       0x76482074 // 't Hv'
#define CPUID_IS_MS_HV_VENDOR(EBX, ECX, EDX)    \
    ((((ECX) ^ CPUID_SIGNATURE_MS_HV_ECX) |     \
      ((EDX) ^ CPUID_SIGNATURE_MS_HV_EDX) |     \
      ((EBX) ^ CPUID_SIGNATURE_MS_HV_EBX)) == 0)
//
// INTEL:   "GenuineIntel"
//
#define CPUID_SIGNATURE_INTEL_EBX       0x756E6547 // 'Genu'
#define CPUID_SIGNATURE_INTEL_EDX       0x49656E69 // 'ineI'
#define CPUID_SIGNATURE_INTEL_ECX       0x6C65746E // 'ntel'
#define CPUID_IS_INTEL_VENDOR(EBX, ECX, EDX)    \
    ((((ECX) ^ CPUID_SIGNATURE_INTEL_ECX) |     \
      ((EDX) ^ CPUID_SIGNATURE_INTEL_EDX) |     \
      ((EBX) ^ CPUID_SIGNATURE_INTEL_EBX)) == 0)
//
// AMD:     "AuthenticAMD"
//
#define CPUID_SIGNATURE_AMD_EBX         0x68747541 // 'Auth'
#define CPUID_SIGNATURE_AMD_EDX         0x69746E65 // 'enti'
#define CPUID_SIGNATURE_AMD_ECX         0x444D4163 // 'cAMD'
#define CPUID_IS_AMD_VENDOR(EBX, ECX, EDX)      \
    ((((ECX) ^ CPUID_SIGNATURE_AMD_ECX) |       \
      ((EDX) ^ CPUID_SIGNATURE_AMD_EDX) |       \
      ((EBX) ^ CPUID_SIGNATURE_AMD_EBX)) == 0)
//
// CENTAUR: "CentaurHauls"
//
#define CPUID_SIGNATURE_CENTAUR_EBX     0x746E6543 // 'Cent'
#define CPUID_SIGNATURE_CENTAUR_EDX     0x48727561 // 'aurH'
#define CPUID_SIGNATURE_CENTAUR_ECX     0x736C7561 // 'auls'
#define CPUID_IS_CENTAUR_VENDOR(EBX, ECX, EDX)  \
    ((((ECX) ^ CPUID_SIGNATURE_CENTAUR_ECX) |   \
      ((EDX) ^ CPUID_SIGNATURE_CENTAUR_EDX) |   \
      ((EBX) ^ CPUID_SIGNATURE_CENTAUR_EBX)) == 0)
//
// CYRIX:   "CyrixInstead"
//
#define CPUID_SIGNATURE_CYRIX_EBX       0x69727943 // 'Cyri'
#define CPUID_SIGNATURE_CYRIX_EDX       0x736E4978 // 'xIns'
#define CPUID_SIGNATURE_CYRIX_ECX       0x64616574 // 'tead'
#define CPUID_IS_CYRIX_VENDOR(EBX, ECX, EDX)    \
    ((((ECX) ^ CPUID_SIGNATURE_CYRIX_ECX) |     \
      ((EDX) ^ CPUID_SIGNATURE_CYRIX_EDX) |     \
      ((EBX) ^ CPUID_SIGNATURE_CYRIX_EBX)) == 0)
//
// TM1:     "TransmetaCPU"
//
#define CPUID_SIGNATURE_TM1_EBX         0x6E617254
#define CPUID_SIGNATURE_TM1_EDX         0x74656D73
#define CPUID_SIGNATURE_TM1_ECX         0x55504361
//
// TM2:     "GenuineTMx86"
//
#define CPUID_SIGNATURE_TM2_EBX         0x756E6547
#define CPUID_SIGNATURE_TM2_EDX         0x54656E69
#define CPUID_SIGNATURE_TM2_ECX         0x3638784D
//
// NSC:     "Geode by NSC"
//
#define CPUID_SIGNATURE_NSC_EBX         0x646F6547
#define CPUID_SIGNATURE_NSC_EDX         0x43534E20
#define CPUID_SIGNATURE_NSC_ECX         0x79622065
//
// NEXGEN:  "NexGenDriven"
//
#define CPUID_SIGNATURE_NEXGEN_EBX      0x4778654E
#define CPUID_SIGNATURE_NEXGEN_EDX      0x72446E65
#define CPUID_SIGNATURE_NEXGEN_ECX      0x6E657669
//
// RISE:    "RiseRiseRise"
//
#define CPUID_SIGNATURE_RISE_EBX        0x65736952
#define CPUID_SIGNATURE_RISE_EDX        0x65736952
#define CPUID_SIGNATURE_RISE_ECX        0x65736952
//
// SIS:     "SiS SiS SiS "
//
#define CPUID_SIGNATURE_SIS_EBX         0x20536953
#define CPUID_SIGNATURE_SIS_EDX         0x20536953
#define CPUID_SIGNATURE_SIS_ECX         0x20536953
//
// UMC:     "UMC UMC UMC "
//
#define CPUID_SIGNATURE_UMC_EBX         0x20434D55
#define CPUID_SIGNATURE_UMC_EDX         0x20434D55
#define CPUID_SIGNATURE_UMC_ECX         0x20434D55
//
// VIA:     "VIA VIA VIA "
//
#define CPUID_SIGNATURE_VIA_EBX         0x20414956
#define CPUID_SIGNATURE_VIA_EDX         0x20414956
#define CPUID_SIGNATURE_VIA_ECX         0x20414956
//
// VORTEX:  "Vortex86 SoC"
//
#define CPUID_SIGNATURE_VORTEX_EBX      0x74726F56
#define CPUID_SIGNATURE_VORTEX_EDX      0x36387865
#define CPUID_SIGNATURE_VORTEX_ECX      0x436F5320



#endif // _ARCH_CPUID_H_
