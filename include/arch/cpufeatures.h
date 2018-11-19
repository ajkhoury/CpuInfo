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
 * @file cpufeatures.h
 * @authors LLVM Team
 *          Aidan Khoury (ajkhoury)
 * @date 11/11/2017
 */

#ifndef _CPUFEATURES_H_
#define _CPUFEATURES_H_

//
// CPUID Feature Bits
//

//
// Features in ECX for leaf 0x00000001
//
#define X86_FEATURE_SSE3        0x00000001      // SSE3 supported
#define X86_FEATURE_PCLMULQDQ   0x00000002      // pclmulqdq instruction supported
#define X86_FEATURE_PCLMUL      X86_FEATURE_PCLMULQDQ  // for gcc compat
#define X86_FEATURE_DTES64      0x00000004      // 64-bit DS area supported
#define X86_FEATURE_MONITOR     0x00000008      // monitor/mwait instructions supported
#define X86_FEATURE_DS_CPL      0x00000010      // cpl qualified debug store
#define X86_FEATURE_VMX         0x00000020      // virtual machine extensions support
#define X86_FEATURE_SMX         0x00000040      // safer mode support
#define X86_FEATURE_EIST        0x00000080      // Intel Enhanced SpeedStep support
#define X86_FEATURE_TM2         0x00000100      // thermal monitor (2)
#define X86_FEATURE_SSSE3       0x00000200      // SSSE3 support
#define X86_FEATURE_CNXTID      0x00000400      // L1 data cache adaptive or shared mode support
#define X86_FEATURE_SDBG        0x00000800      // IA32_DEBUG_INTERFACE MSR support
#define X86_FEATURE_FMA         0x00001000      // fused multiply-add extensions supported
#define X86_FEATURE_CX16        0x00002000      // cmpxchg16b instruction supported
#define X86_FEATURE_CMPXCHG16B  X86_FEATURE_CX16// for gcc compat
#define X86_FEATURE_XTPR        0x00004000      // xTPR update control supported
#define X86_FEATURE_PDCM        0x00008000      // perfmon and debug capability support
#define X86_FEATURE_UNUSED0     0x00010000      // unused bit
#define X86_FEATURE_PCID        0x00020000      // process context identifiers supported
#define X86_FEATURE_DCA         0x00040000      // direct cache access for DMA writes supported
#define X86_FEATURE_SSE41       0x00080000      // SSE4.1 supported
#define X86_FEATURE_SSE4_1      X86_FEATURE_SSE41 // for gcc compat
#define X86_FEATURE_SSE42       0x00100000      // SSE4.2 supported
#define X86_FEATURE_SSE4_2      X86_FEATURE_SSE42 // for gcc compat
#define X86_FEATURE_X2APIC      0x00200000      // x2APIC feature support
#define X86_FEATURE_MOVBE       0x00400000      // movbe instruction supported
#define X86_FEATURE_POPCNT      0x00800000      // popcnt instruction supported
#define X86_FEATURE_TSC_DEADLINE 0x01000000     // local APIC timer supports one-shot operation using TSC deadline value
#define X86_FEATURE_AESNI       0x02000000      // AESNI instruction extensions supported
#define X86_FEATURE_AES         X86_FEATURE_AESNI // for gcc compat
#define X86_FEATURE_XSAVE       0x04000000      // xsave/xrstor feature, the xsetbv/xgetbv instructions, and XCR0
#define X86_FEATURE_OSXSAVE     0x08000000      // OS has set CR4.OSXSAVE
#define X86_FEATURE_AVX         0x10000000      // AVX instruction extensions supported
#define X86_FEATURE_F16C        0x20000000      // 16-bit floating-point conversion instructions supported
#define X86_FEATURE_RDRND       0x40000000      // rdrand instruction supported
#define X86_FEATURE_HYPERVISOR  0x80000000      // reserved for use by hypervisor to indicate guest status
//
// Features in EDX for leaf 0x00000001
//
#define X86_FEATURE_FPU         0x00000001      // FPU is on chip
#define X86_FEATURE_VME         0x00000002      // virtual 8086 mode enhancement
#define X86_FEATURE_DE          0x00000004      // debugging extension
#define X86_FEATURE_PSE         0x00000008      // page size extension
#define X86_FEATURE_TSC         0x00000010      // time stamp counter
#define X86_FEATURE_MSR         0x00000020      // rdmsr and wrmsr support
#define X86_FEATURE_PAE         0x00000040      // physical address extension
#define X86_FEATURE_MCE         0x00000080      // machine check exception
#define X86_FEATURE_CX8         0x00000100      // cmpxchg8b instruction supported
#define X86_FEATURE_CMPXCHG8B   X86_FEATURE_CX8 // for gcc compat
#define X86_FEATURE_APIC        0x00000200      // APIC on chip
#define X86_FEATURE_UNUSED1     0x00000400      // unused bit
#define X86_FEATURE_SEP         0x00000800      // sysenter/sysesxit instructions
#define X86_FEATURE_MTRR        0x00001000      // memory type range registers
#define X86_FEATURE_PGE         0x00002000      // global page TB support
#define X86_FEATURE_MCA         0x00004000      // machine check architecture
#define X86_FEATURE_CMOV        0x00008000      // cmov instruction supported
#define X86_FEATURE_PAT         0x00010000      // physical attributes table
#define X86_FEATURE_PSE36       0x00020000      // 36-bit page size extension (2)
#define X86_FEATURE_PSN         0x00040000      // processor serial number
#define X86_FEATURE_CLFSH       0x00080000      // cache line flush
#define X86_FEATURE_UNUSED2     0x00100000      // unused bit
#define X86_FEATURE_DS          0x00200000      // debug store
#define X86_FEATURE_ACPI        0x00400000      // ACPI thermal monitors and software controlled clock facilities.
#define X86_FEATURE_MMX         0x00800000      // MMX technology supported
#define X86_FEATURE_FXSR        0x01000000      // fxsr instruction supported
#define X86_FEATURE_FXSAVE      X86_FEATURE_FXSR // for gcc compat
#define X86_FEATURE_SSE         0x02000000      // SSE supported
#define X86_FEATURE_SSE2        0x04000000      // SSE2 supported
#define X86_FEATURE_SS          0x08000000      // self snoop
#define X86_FEATURE_HTT         0x10000000      // symmetric multithreading (hyper-threading) supported
#define X86_FEATURE_TM          0x20000000      // thermal monitor
#define X86_FEATURE_IA64        0x40000000      // IA-64 processor emulating x86
#define X86_FEATURE_PBE         0x80000000      // pending break enable wakeup support
//
// Features in EBX for leaf 0x00000007 sub-leaf 0
//
#define X86_FEATURE_FSGSBASE    0x00000001      // FS and GS base read write instruction support
#define X86_FEATURE_TSCADJUST   0x00000002      // IA32_TSC_ADJUST MSR is supported
#define X86_FEATURE_SGX         0x00000004      // software guard extensions support
#define X86_FEATURE_BMI1        0x00000008      // bit manipulation group 1 instruction support
#define X86_FEATURE_BMI         X86_FEATURE_BMI1// for gcc compat
#define X86_FEATURE_HLE         0x00000010      // tsx hardware lock elision support
#define X86_FEATURE_AVX2        0x00000020      // AVX2 instruction subset support
#define X86_FEATURE_FDP_EXCPTN_ONLY 0x00000040  // FPU Data Pointer updated only on x87 exceptions
#define X86_FEATURE_SMEP        0x00000080      // supervisor mode execution protection
#define X86_FEATURE_BMI2        0x00000100      // bit manipulation group 2 instruction support
#define X86_FEATURE_ERMS        0x00000200      // enhanced rep movsb/stosb support
#define X86_FEATURE_INVPCID     0x00000400      // invpcid instruction support
#define X86_FEATURE_RTM         0x00000800      // tsx restricted transactional memory support
#define X86_FEATURE_PQM         0x00001000      // platform quality of service monitoring support
#define X86_FEATURE_RDTM        0x00001000      // resource director technology monitoring support
#define X86_FEATURE_DEPFPUCS    0x00002000      // deprecate FPU CS and FPU DS values
#define X86_FEATURE_MPX         0x00004000      // memory protection extensions
#define X86_FEATURE_PQE         0x00008000      // platform quality of service enforcement
#define X86_FEATURE_RDTA        0x00008000      // resource director technology allocation support
#define X86_FEATURE_AVX512F     0x00010000      // AVX-512 foundation support
#define X86_FEATURE_AVX512DQ    0x00020000      // AVX-512 doubleword and quadword instructions supported
#define X86_FEATURE_RDSEED      0x00040000      // rdseed instruction supported
#define X86_FEATURE_ADX         0x00080000      // multi-precision add-carry instruction extensions supported
#define X86_FEATURE_SMAP        0x00100000      // supervisor mode access prevention
#define X86_FEATURE_AVX512IFMA  0x00200000      // AVX-512 integer fused multiply-add instructions supported
#define X86_FEATURE_PCOMMIT     0x00400000      // pcommit instruction supported
#define X86_FEATURE_CLFLUSHOPT  0x00800000      // clflushopt instruction supported
#define X86_FEATURE_CLWB        0x01000000      // clwb instruction supported
#define X86_FEATURE_INTEL_PT    0x02000000      // Intel processor trace support
#define X86_FEATURE_AVX512PF    0x04000000      // AVX-512 prefetch instructions supported
#define X86_FEATURE_AVX512ER    0x08000000      // AVX-512 exponential and reciprocal instructions supported
#define X86_FEATURE_AVX512CD    0x10000000      // AVX-512 conflict detection instructions supported
#define X86_FEATURE_SHA         0x20000000      // SHA extensions supported
#define X86_FEATURE_AVX512BW    0x40000000      // AVX-512 byte and word instructions supported
#define X86_FEATURE_AVX512VL    0x80000000      // AVX-512 vector length extensions supported
//
// Features in ECX for leaf 0x00000007 sub-leaf 0
//
#define X86_FEATURE_PREFTCHWT1  0x00000001      // prefetchwt1 instruction supported
#define X86_FEATURE_AVX512VBMI1 0x00000002      // AVX-512 vector bit manipulation instructions 1 supported
#define X86_FEATURE_AVX512VBMI  X86_FEATURE_AVX512VBMI1 // for compat
#define X86_FEATURE_UMIP        0x00000004      // user-mode instruction prevention supported
#define X86_FEATURE_PKU         0x00000008      // memory protection keys for user-mode pages
#define X86_FEATURE_OSPKE       0x00000010      // pku enabled by OS
#define X86_FEATURE_AVX512VBMI2 0x00000040      // AVX-512 vector bit manipulation instructions 2 supported
#define X86_FEATURE_GFNI        0x00000100      // galois field instructions supported
#define X86_FEATURE_VAES        0x00000200      // vector AES instruction set supported
#define X86_FEATURE_VPCLMULQDQ  0x00000400      // clmul instruction set supported
#define X86_FEATURE_AVX512VNNI  0x00000800      // AVX-512 vector neural network instructions supported
#define X86_FEATURE_AVX512BITALG 0x00001000     // AVX-512 bitalg instructions supported
#define X86_FEATURE_AVX512VPOPCNTDQ 0x00004000  // AVX-512 vector population count double and quadword instructions
#define X86_FEATURE_MAWAU_SHIFT 17              // starting bit position of userspace mpx address-width adjust value
#define X86_FEATURE_MAWAU_MASK  0x003E0000      // value of MAWAU used by the bndldx and bndstx instructions
#define X86_FEATURE_RDPID       0x00400000      // read processor id (rdpid) instruction supported
#define X86_FEATURE_SGXLC       0x40000000      // software guard extensions launch configuration supported
//
// Features in EDX for leaf 0x00000007 sub-leaf 0
//
#define X86_FEATURE_AVX5124VNNIW 0x00000004     // AVX-512 4-register neural network instructions supported
#define X86_FEATURE_AVX5124FMAPS 0x00000008     // AVX-512 4-register multiply accumulation single precision
#define X86_FEATURE_PCONFIG     0x00000008      // platform configuration (memory encryption technologies instructions)
#define X86_FEATURE_IBRS_IBPB   0x04000000      // indirect branch restricted speculation and indirect branch
                                                // prediction barrier supported
#define X86_FEATURE_STIBP       0x08000000      // single thread indirect branch predictors supported
#define X86_FEATURE_ARCH_CAPS   0x20000000      // IA32_ARCH_CAPABILITIES MSR supported
#define X86_FEATURE_SSBD        0x80000000      // speculative store bypass disable supported
//
// Features in EAX for leaf 0x0000000D sub-leaf 1
//
#define X86_FEATURE_XSAVEOPT    0x00000001      // xsaveopt instruction supported
#define X86_FEATURE_XSAVEC      0x00000002      // xsavec instruction supported
#define X86_FEATURE_XGETBV_ECX_1 0x00000004     // xgetbv with ECX=1 is supported
#define X86_FEATURE_XSAVES      0x00000008      // xsaves/xrstors instructions supported
//
// Features in EBX for leaf 0x00000014 sub-leaf 0 (Intel PT capabilities)
//
#define X86_FEATURE_PT_CR3_FILTER 0x00000001    // IA32_RTIT_CTL.CR3Filter and IA32_RTIT_CR3_MATCH supported
#define X86_FEATURE_PT_PSB_CYC  0x00000002      // configurable PSB and cycle-accurate mode support
#define X86_FEATURE_PT_IP_FILTER 0x00000004     // IP filtering, TraceStop filtering and preservation of Intel PT MSRs
#define X86_FEATURE_PT_MTC      0x00000008      // MTC timing packet and suppression of COFI-based packets supported
#define X86_FEATURE_PT_PTWRITE  0x00000010      // ptwrite instruction support
#define X86_FEATURE_PT_PET      0x00000020      // Power Event Trace packet generation supported
//
// Features in ECX for leaf 0x80000001
//
#define X86_FEATURE_LAHF_LM     0x00000001      // lahf and sahf instruction support in 64-bit mode
#define X86_FEATURE_CMP_LEGACY  0x00000002      // hyperthreading not valid
#define X86_FEATURE_SVM         0x00000004      // secure virtual machine
#define X86_FEATURE_EXTAPIC     0x00000008      // extended APIC space supported
#define X86_FEATURE_ALTMOVCR8   0x00000010      // LOCK MOV CR0 means MOV CR8 (CR8 in 32-bit mode)
#define X86_FEATURE_ABM         0x00000020      // lzcnt instruction support
#define X86_FEATURE_LZCNT       X86_FEATURE_ABM // for gcc compat
#define X86_FEATURE_SSE4A       0x00000040      // SSE4A supported
#define X86_FEATURE_MISALIGNSSE 0x00000080      // misaligned SSE mode support
#define X86_FEATURE_PRFCHW      0x00000100      // prefetch and prefetchw instruction support
#define X86_FEATURE_OSWV        0x00000200      // OS-visible workaround support
#define X86_FEATURE_IBS         0x00000400      // instruction based sampling
#define X86_FEATURE_XOP         0x00000800      // extended operation support
#define X86_FEATURE_SKINIT      0x00001000      // skinit and stgi instructions are supported
#define X86_FEATURE_WDT         0x00002000      // watchdog timer support
#define X86_FEATURE_LWP         0x00008000      // lightweight profiling support
#define X86_FEATURE_FMA4        0x00010000      // four-operand FMA instruction support
#define X86_FEATURE_TCE         0x00020000      // translation cache extension support
#define X86_FEATURE_NODEID      0x00040000      // NodeID MSR supported
#define X86_FEATURE_TBM         0x00200000      // trailing bit manipulation instruction support
#define X86_FEATURE_TOPOEXT     0x00400000      // topology extensions support
#define X86_FEATURE_PERFCTR_CORE 0x00800000     // processor performance counter extensions support
#define X86_FEATURE_PERFCTR_NB  0x01000000      // NB performance counter extensions support
#define X86_FEATURE_DBX         0x04000000      // data access breakpoint extension
#define X86_FEATURE_PERFTSC     0x08000000      // performance time-stamp counter
#define X86_FEATURE_PCX_L2I     0x10000000      // L2I performance counter extensions
#define X86_FEATURE_MONITORX    0x20000000      // monitorx/mwaitx instructions supported
//
// Features in EDX for leaf 0x80000001
//
#define X86_FEATURE_FPUEXT      0x00000001      // FPU is on chip
#define X86_FEATURE_VMEEXT      0x00000002      // virtual 8086 mode enhancement
#define X86_FEATURE_DEEXT       0x00000004      // debugging extension
#define X86_FEATURE_PSEEXT      0x00000008      // page size extension
#define X86_FEATURE_TSCEXT      0x00000010      // time stamp counter
#define X86_FEATURE_MSREXT      0x00000020      // rdmsr and wrmsr support
#define X86_FEATURE_PAEEXT      0x00000040      // physical address extension
#define X86_FEATURE_MCEEXT      0x00000080      // machine check exception
#define X86_FEATURE_CX8EXT      0x00000100      // cmpxchg8b instruction supported
#define X86_FEATURE_APICEXT     0x00000200      // APIC on chip
#define X86_FEATURE_SEPEXT      0x00000800      // syscall/sysret instructions
#define X86_FEATURE_MTRREXT     0x00001000      // memory type range registers
#define X86_FEATURE_PGEEXT      0x00002000      // global page TB support
#define X86_FEATURE_MCAEXT      0x00004000      // machine check architecture
#define X86_FEATURE_CMOVEXT     0x00008000      // cmov instruction supported
#define X86_FEATURE_PATEXT      0x00010000      // physical attributes table
#define X86_FEATURE_PSE36EXT    0x00020000      // 36-bit page size extension (2)
#define X86_FEATURE_MP          0x00080000      // multi-processor capable
#define X86_FEATURE_NOEXECUTE   0x00100000      // no execute protection
#define X86_FEATURE_MMXEXT      0x00400000      // MMX extensions
#define X86_FEATURE_MMXINT      0x00800000      // MMX technology supported
#define X86_FEATURE_FXSREXT     0x01000000      // fxsr instruction supported
#define X86_FEATURE_FFXSR       0x02000000      // fast floating save/restore
#define X86_FEATURE_PDPE1GB     0x04000000      // 1-gibibyte pages supported
#define X86_FEATURE_RDTSCP      0x08000000      // rdtscp and IA32_TSC_AUX MSR supported
#define X86_FEATURE_LONGMODE    0x20000000      // long mode supported
#define X86_FEATURE_LM          X86_FEATURE_LONGMODE // for gcc compat
#define X86_FEATURE_3DNOWEXT    0x40000000      // 3DNOW extensions
#define X86_FEATURE_3DNOW       0x80000000      // 3DNOW supported
//
// Features in EBX for leaf 0x80000008
//
#define X86_FEATURE_CLZERO      0x00000001      // clzero instruction supported
#define X86_FEATURE_INSTRETCNT  0x00000002      // Instruction Retired Counter MSR available
#define X86_FEATURE_RSTRFPERRPTRS 0x00000004    // FP Error Pointers Restored by XRSTOR
#define X86_FEATURE_WBNOINVD    0x00000200      // wbnoinvd instruction supported

#endif // _CPUFEATURES_H_
