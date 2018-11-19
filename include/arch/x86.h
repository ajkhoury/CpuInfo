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
 * @file x86.h
 * @author Aidan Khoury (ajkhoury)
 * @date 11/16/2018 
 */

#ifndef _ARCH_X86_H_
#define _ARCH_X86_H_
#if defined(_MSC_VER)
#pragma once
#endif

// Include architecture dependent definitions.
#include "arch/i386.h"
#include "arch/x64.h"

// Include architectureal MSRs
#include "arch/msr.h"

// Include CPU feature bits.
#include "cpufeatures.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4214) // nonstandard extension used: bit field types other than int
#endif

/**
 * Code and Data Segment Descriptor Types.
 *
 * Intel SDM Vol. 3A - 3.4.5.1 Code- and Data-Segment Descriptor Types (Table 3-1. Code- and Data-Segment Types)
 * AMD APM Vol. 2 r3.30 - 4.7.2 Code-Segment Descriptors (Table 4-3. Code-Segment Descriptor Types)
 *                        4.7.3 Data-Segment Descriptors (Table 4-4. Data-Segment Descriptor Types)
 */
#define X86_GDT_SEG_DATA_RD         0x0     //!< Read-Only
#define X86_GDT_SEG_DATA_RDA        0x1     //!< Read-Only, accessed
#define X86_GDT_SEG_DATA_RDWR       0x2     //!< Read/Write
#define X86_GDT_SEG_DATA_RDWRA      0x3     //!< Read/Write, accessed
#define X86_GDT_SEG_DATA_RDEXPD     0x4     //!< Read-Only, expand-down
#define X86_GDT_SEG_DATA_RDEXPDA    0x5     //!< Read-Only, expand-down, accessed
#define X86_GDT_SEG_DATA_RDWREXPD   0x6     //!< Read/Write, expand-down
#define X86_GDT_SEG_DATA_RDWREXPDA  0x7     //!< Read/Write, expand-down, accessed
#define X86_GDT_SEG_CODE_EX         0x8     //!< Execute-Only
#define X86_GDT_SEG_CODE_EXA        0x9     //!< Execute-Only, accessed
#define X86_GDT_SEG_CODE_EXRD       0xA     //!< Execute/Read
#define X86_GDT_SEG_CODE_EXRDA      0xB     //!< Execute/Read, accessed
#define X86_GDT_SEG_CODE_EXC        0xC     //!< Execute-Only, conforming
#define X86_GDT_SEG_CODE_EXCA       0xD     //!< Execute-Only, conforming, accessed
#define X86_GDT_SEG_CODE_EXRDC      0xE     //!< Execute/Read, conforming
#define X86_GDT_SEG_CODE_EXRDCA     0xF     //!< Execute/Read, conforming, accessed

/** Segment Constants */
#define X86_GDT_TYPE_CODE           0x18    //!< A Code Segment type.
#define X86_GDT_TYPE_DATA           0x10    //!< A Data Segment type.
#define X86_GDT_TYPE_TSS16          0x01    //!< A 16-bit Task State Segment type.
#define X86_GDT_TYPE_TSS32          0x09    //!< A 32-bit Task State Segment type.
#define X86_GDT_TYPE_EXP_DOWN       0x04    //!< The segment grows downward.
#define X86_GDT_TYPE_CONFORMING     0x04    //!< A conforming code segment.
#define X86_GDT_TYPE_RW             0x02    //!< A readable/writable segment.

/** A standard code segment */
#define X86_GDT_CODE_SEG            (X86_GDT_TYPE_CODE | X86_GDT_TYPE_RW)
/** A standard data segment */
#define X86_GDT_DATA_SEG            (X86_GDT_TYPE_DATA | X86_GDT_TYPE_RW)
/** A standard stack segment */
#define X86_GDT_STACK_SEG           (X86_GDT_DATA_SEG)
/** A standard TSS segment */
#define X86_GDT_TSS_SEG             (X86_GDT_TYPE_TSS32)
/** A real-mode area segment */
#define X86_GDT_REAL_SEG            (X86_GDT_TYPE_CODE | X86_GDT_TYPE_RW)

/** Descriptor Privilege Level 0 (maximum privilege) */
#define X86_DPL_0                   0x00
/** Descriptor Privilege Level 1 */
#define X86_DPL_1                   0x20
/** Descriptor Privilege Level 2 */
#define X86_DPL_2                   0x40
/** Descriptor Privilege Level 3 (minimum privilege) */
#define X86_DPL_3                   0x60

/** Get a selector offset in the GDT from gate index */
#define GDT_SELECTOR(gate)                                  \
    ((gate) << 3)

/** Get a GDT segment descriptor entry. */
#define GDT_DESC(gdtbase, sel)                              \
    ((gdtbase) + ((sel) & ~3))

/** Get GDT descriptor base address */
#define GDT_BASE(desc)                                      \
    (((desc)->Bytes.BaseLow) +                              \
        ((((desc)->Bytes.BaseMiddle) +                      \
            (((desc)->Bytes.BaseHigh) << 8)) << 16))

/** Get GDT descriptor limit value */
#define GDT_LIMIT(desc)                                     \
    (((desc)->Bytes.LimitLow) +                             \
        ((UINT32)(((desc)->Bytes.Flags2) & 0x0F) << 16))

/** Determine if GDT segment type is a data segment. */
#define GDT_DATA_SEG(Type) \
    ((Type) >= X86_GDT_SEG_DATA_RD && (Type) <= X86_GDT_SEG_DATA_RDWREXPDA)

/** Determine if GDT segment type is a code segment. */
#define GDT_CODE_SEG(Type) \
    ((Type) >= X86_GDT_SEG_CODE_EX && (Type) <= X86_GDT_SEG_CODE_EXRDCA)

/** Ring/Requested Privilege Level */
#define RPL_0 0
#define RPL_1 1
#define RPL_2 2
#define RPL_3 3

/** Current Privilege Level */
#define CPL_0 RPL_0
#define CPL_1 RPL_1
#define CPL_2 RPL_2
#define CPL_3 RPL_3

/**
 * x86 System Descriptor Types.
 *
 * When the S (descriptor type) flag in a segment descriptor is clear, the descriptor type is a system descriptor. The
 * processor recognizes the following types of system descriptors
 *
 * Intel SDM Vol. 3A - 3.5 SYSTEM DESCRIPTOR TYPES (Table 3-2. System-Segment and Gate-Descriptor Types)
 * AMD APM Vol. 2 r3.30 - 4.8.3 System Descriptors (Table 4-6. System-Segment Descriptor Types-Long Mode)
 */
/**                      P  DPL S Type           */
/** 32/64 bit Interrupt: 1  000 0 1110  ->  0x8E */
/** 32/64 bit Trap:      1  000 0 1111  ->  0x8F */
#define X86_DESCRIPTOR_TYPE_TSS_16          0x01
#define X86_DESCRIPTOR_TYPE_LDT             0x02
#define X86_DESCRIPTOR_TYPE_TSS_BUSY_16     0x03
#define X86_DESCRIPTOR_TYPE_CALL_16         0x04
#define X86_DESCRIPTOR_TYPE_TASK            0x05
#define X86_DESCRIPTOR_TYPE_INTERRUPT_16    0x06
#define X86_DESCRIPTOR_TYPE_TRAP_16         0x07
#define X86_DESCRIPTOR_TYPE_TSS             0x09
#define X86_DESCRIPTOR_TYPE_TSS_BUSY        0x0B
#define X86_DESCRIPTOR_TYPE_CALL            0x0C
#define X86_DESCRIPTOR_TYPE_INTERRUPT       0x0E
#define X86_DESCRIPTOR_TYPE_TRAP            0x0F

/**
 * x86 Trap Interrupts/Exceptions.
 *
 * Intel SDM Vol. 3A - 6.15 EXCEPTION AND INTERRUPT REFERENCE
 * AMD APM Vol. 2 r3.30 - 8.2 Vectors (Table 8-1. Interrupt Vector Source and Cause)
 */
#define X86_TRAP_DE            0   // Divide Error
#define X86_TRAP_DB            1   // Debug
#define X86_TRAP_NMI           2   // Non-maskable Interrupt
#define X86_TRAP_BP            3   // Breakpoint
#define X86_TRAP_OF            4   // Overflow
#define X86_TRAP_BR            5   // Bound Range Exceeded
#define X86_TRAP_UD            6   // Invalid Opcode
#define X86_TRAP_NM            7   // Device Not Available
#define X86_TRAP_DF            8   // Double Fault
#define X86_TRAP_OLD_MF        9   // Coprocessor Segment Overrun
#define X86_TRAP_TS            10  // Invalid TSS
#define X86_TRAP_NP            11  // Segment Not Present
#define X86_TRAP_SS            12  // Stack Segment Fault
#define X86_TRAP_GP            13  // General Protection Fault
#define X86_TRAP_PF            14  // Page Fault
#define X86_TRAP_SPURIOUS      15  // Spurious Interrupt
#define X86_TRAP_MF            16  // x87 Floating-Point Exception
#define X86_TRAP_AC            17  // Alignment Check
#define X86_TRAP_MC            18  // Machine Check
#define X86_TRAP_XF            19  // SIMD Floating-Point Exception
#define X86_TRAP_VE            20  // Intel Virtualization Exception
#define X86_TRAP_VC            29  // AMD VMM Communication Exception
#define X86_TRAP_SX            30  // AMD Security Exception
#define X86_TRAP_IRET          32  // IRET Exception


/**
 * x86 Memory Cache Types.
 *
 * Intel SDM Vol. 3A - 11.3 METHODS OF CACHING AVAILABLE
 * Intel SDM Vol. 3A - 11.11 MEMORY TYPE RANGE REGISTERS (MTRRS)
 *                           (Table 11-8. Memory Types That Can Be Encoded in MTRRs)
 * Intel SDM Vol. 3C - 28.2.6 EPT and Memory Typing
 *
 * AMD APM Vol. 2 r3.30 - 7.4 Memory Types
 * AMD APM Vol. 2 r3.30 - 7.7.1 MTRR Type Fields (Table 7-5. MTRR Type Field Encodings)
 */
#define X86_MEMORY_TYPE_UNCACHEABLE 0
#define X86_MEMORY_TYPE_WRITE_COMBINING 1
#define X86_MEMORY_TYPE_WRITE_THROUGH 4
#define X86_MEMORY_TYPE_WRITE_PROTECTED 5
#define X86_MEMORY_TYPE_WRITE_BACK 6
#define X86_MEMORY_TYPE_UNCACHED 7

#define X86_MEMORY_TYPE_UC X86_MEMORY_TYPE_UNCACHEABLE
#define X86_MEMORY_TYPE_WC X86_MEMORY_TYPE_WRITE_COMBINING
#define X86_MEMORY_TYPE_WT X86_MEMORY_TYPE_WRITE_THROUGH
#define X86_MEMORY_TYPE_WP X86_MEMORY_TYPE_WRITE_PROTECTED
#define X86_MEMORY_TYPE_WB X86_MEMORY_TYPE_WRITE_BACK

typedef enum _X86_MEMORY_TYPE {
    MemoryTypeUncacheable = 0,
    MemoryTypeWriteCombining = 1,
    MemoryTypeWriteThrough = 4,
    MemoryTypeWriteProtected = 5,
    MemoryTypeWriteBack = 6,
    MemoryTypeUncached = 7,
} _X86_MEMORY_TYPE, *PX86_MEMORY_TYPE;


//
// The following structures must by 1-byte packed/aligned.
//
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif

/**
 * x86 Register Values.
 */
typedef union _X86_REGISTER {
    struct {
        UINT8 Low;
        UINT8 High;
    } Uint8;
    struct {
        INT8 Low;
        INT8 High;
    } Int8;
    UINT16 Uint16;
    UINT32 Uint32;
    PVOID Pointer;
    UINT64 Uint64;
} PACKED X86_REGISTER;
C_ASSERT(FIELD_OFFSET(X86_REGISTER, Uint8.Low) == 0 && FIELD_SIZEOF(X86_REGISTER, Uint8.Low) == 1);
C_ASSERT(FIELD_OFFSET(X86_REGISTER, Uint8.High) == 1 && FIELD_SIZEOF(X86_REGISTER, Uint8.High) == 1);
C_ASSERT(FIELD_OFFSET(X86_REGISTER, Int8.Low) == 0 && FIELD_SIZEOF(X86_REGISTER, Int8.Low) == 1);
C_ASSERT(FIELD_OFFSET(X86_REGISTER, Int8.High) == 1 && FIELD_SIZEOF(X86_REGISTER, Int8.High) == 1);
C_ASSERT(FIELD_OFFSET(X86_REGISTER, Uint16) == 0 && FIELD_SIZEOF(X86_REGISTER, Uint16) == 2);
C_ASSERT(FIELD_OFFSET(X86_REGISTER, Uint32) == 0 && FIELD_SIZEOF(X86_REGISTER, Uint32) == 4);
C_ASSERT(FIELD_OFFSET(X86_REGISTER, Pointer) == 0 && FIELD_SIZEOF(X86_REGISTER, Pointer) == sizeof(void*));
C_ASSERT(FIELD_OFFSET(X86_REGISTER, Uint64) == 0 && FIELD_SIZEOF(X86_REGISTER, Uint64) == 8);

/**
 * Structures for the 16-bit real mode thunks.
 */
typedef struct _X86_BYTE_REGS {
    UINT32 Reserved1;
    UINT32 Reserved2;
    UINT32 Reserved3;
    UINT32 Reserved4;
    UINT8 BL;
    UINT8 BH;
    UINT16 Reserved5;
    UINT8 DL;
    UINT8 DH;
    UINT16 Reserved6;
    UINT8 CL;
    UINT8 CH;
    UINT16 Reserved7;
    UINT8 AL;
    UINT8 AH;
    UINT16 Reserved8;
} PACKED X86_BYTE_REGS;

typedef struct _X86_WORD_REGS {
    UINT16 DI;
    UINT16 Reserved1;
    UINT16 SI;
    UINT16 Reserved2;
    UINT16 BP;
    UINT16 Reserved3;
    UINT16 SP;
    UINT16 Reserved4;
    UINT16 BX;
    UINT16 Reserved5;
    UINT16 DX;
    UINT16 Reserved6;
    UINT16 CX;
    UINT16 Reserved7;
    UINT16 AX;
    UINT16 Reserved8;
} PACKED X86_WORD_REGS;

typedef struct _X86_DWORD_REGS {
    UINT32 EDI;
    UINT32 ESI;
    UINT32 EBP;
    UINT32 ESP;
    UINT32 EBX;
    UINT32 EDX;
    UINT32 ECX;
    UINT32 EAX;
    UINT16 DS;
    UINT16 ES;
    UINT16 FS;
    UINT16 GS;
    UINT32 EFLAGS;
    UINT32 Eip;
    UINT16 CS;
    UINT16 SS;
} PACKED X86_DWORD_REGS;

typedef union _X86_REGISTER_SET {
    X86_DWORD_REGS E;
    X86_WORD_REGS X;
    X86_BYTE_REGS H;
} X86_REGISTER_SET;


/**
 * Byte packed structure for x86 EFLAGS/RFLAGS Register.
 *
 * 16-bits on i8086.
 * 32-bits on i386.
 * 64-bits on amd64/ia-32e (upper 32-bits are reserved).
 *
 * Intel SDM Vol.  1 - 3.4.3 EFLAGS Register (Figure 3-8. EFLAGS Register)
 * Intel SDM Vol. 3A - 2.3 SYSTEM FLAGS AND FIELDS IN THE EFLAGS REGISTER (Figure 2-5. System Flags in the EFLAGS Register)
 * AMD APM Vol. 2 r3.30 - 3.1.6 RFLAGS Register (Figure 3-7. RFLAGS Register)
 * AMD APM Vol. 1 r3.22 - 3.1.4 Flags Register (Figure 3-5. rFLAGS Register-Flags Visible to Application Software)
 */
typedef union _X86_FLAGS {
    struct {
        UINT32 CF:1;            //!<   0   Carry Flag.
        UINT32 Reserved0:1;     //!<   1   Reserved.
        UINT32 PF:1;            //!<   2   Parity Flag.
        UINT32 Reserved1:1;     //!<   3   Reserved.
        UINT32 AF:1;            //!<   4   Auxiliary Carry Flag.
        UINT32 Reserved2:1;     //!<   5   Reserved.
        UINT32 ZF:1;            //!<   6   Zero Flag.
        UINT32 SF:1;            //!<   7   Sign Flag.
        UINT32 TF:1;            //!<   8   Trap Flag.
        UINT32 IF:1;            //!<   9   Interrupt Enable Flag.
        UINT32 DF:1;            //!<  10   Direction Flag.
        UINT32 OF:1;            //!<  11   Overflow Flag.
        UINT32 IOPL:2;          //!< 13:12 I/O Privilege Level.
        UINT32 NT:1;            //!<  14   Nested Task.
        UINT32 Reserved3:1;     //!<  15   Reserved.
        UINT32 RF:1;            //!<  16   Resume Flag.
        UINT32 VM:1;            //!<  17   Virtual 8086 Mode.
        UINT32 AC:1;            //!<  18   Alignment Check.
        UINT32 VIF:1;           //!<  19   Virtual Interrupt Flag.
        UINT32 VIP:1;           //!<  20   Virtual Interrupt Pending.
        UINT32 ID:1;            //!<  21   Identification Flag.
        UINT32 Reserved4:10;    //!< 31:22 Reserved.
        UINT32 Reserved5;       //!< 63:32 Reserved.
    } Bits;
    UINT16 Uint16;
    UINT32 Uint32;
    UINT64 Uint64;
} PACKED X86_EFLAGS;
C_ASSERT(FIELD_OFFSET(X86_EFLAGS, Uint16) == 0 && FIELD_SIZEOF(X86_EFLAGS, Uint16) == 2);
C_ASSERT(FIELD_OFFSET(X86_EFLAGS, Uint32) == 0 && FIELD_SIZEOF(X86_EFLAGS, Uint32) == 4);
C_ASSERT(FIELD_OFFSET(X86_EFLAGS, Uint64) == 0 && FIELD_SIZEOF(X86_EFLAGS, Uint64) == 8);

#define X86_FLAGS_CF_BIT      0 // Carry Flag
#define X86_FLAGS_CF          (1 << X86_FLAGS_CF_BIT)
#define X86_FLAGS_FIXED_BIT   1 // Bit 1 - always on
#define X86_FLAGS_FIXED       (1 << X86_FLAGS_FIXED_BIT)
#define X86_FLAGS_PF_BIT      2 // Parity Flag
#define X86_FLAGS_PF          (1 << X86_FLAGS_PF_BIT)
#define X86_FLAGS_AF_BIT      4 // Auxiliary Carry Flag
#define X86_FLAGS_AF          (1 << X86_FLAGS_AF_BIT)
#define X86_FLAGS_ZF_BIT      6 // Zero Flag
#define X86_FLAGS_ZF          (1 << X86_FLAGS_ZF_BIT)
#define X86_FLAGS_SF_BIT      7 // Sign Flag
#define X86_FLAGS_SF          (1 << X86_FLAGS_SF_BIT)
#define X86_FLAGS_TF_BIT      8 // Trap Flag
#define X86_FLAGS_TF          (1 << X86_FLAGS_TF_BIT)
#define X86_FLAGS_IF_BIT      9 // Interrupt Flag
#define X86_FLAGS_IF          (1 << X86_FLAGS_IF_BIT)
#define X86_FLAGS_DF_BIT      10 // Direction Flag
#define X86_FLAGS_DF          (1 << X86_FLAGS_DF_BIT)
#define X86_FLAGS_OF_BIT      11 // Overflow Flag
#define X86_FLAGS_OF          (1 << X86_FLAGS_OF_BIT)
#define X86_FLAGS_IOPL_BIT    12 // I/O Privilege Level (2 bits)
#define X86_FLAGS_IOPL        (3 << X86_FLAGS_IOPL_BIT)
#define X86_FLAGS_IOPL_3      (3 << X86_FLAGS_IOPL_BIT)
#define X86_FLAGS_IOPL_2      (2 << X86_FLAGS_IOPL_BIT)
#define X86_FLAGS_IOPL_1      (1 << X86_FLAGS_IOPL_BIT)
#define X86_FLAGS_IOPL_0      (0 << X86_FLAGS_IOPL_BIT) // is just 0
#define X86_FLAGS_NT_BIT      14 // Nested Task
#define X86_FLAGS_NT          (1 << X86_FLAGS_NT_BIT)
#define X86_FLAGS_RF_BIT      16 // Resume Flag
#define X86_FLAGS_RF          (1 << X86_FLAGS_RF_BIT)
#define X86_FLAGS_VM_BIT      17 // Virtual Mode
#define X86_FLAGS_VM          (1 << X86_FLAGS_VM_BIT)
#define X86_FLAGS_AC_BIT      18 // Alignment Check/Access Control
#define X86_FLAGS_AC          (1 << X86_FLAGS_AC_BIT)
#define X86_FLAGS_VIF_BIT     19 // Virtual Interrupt Flag
#define X86_FLAGS_VIF         (1 << X86_FLAGS_VIF_BIT)
#define X86_FLAGS_VIP_BIT     20 // Virtual Interrupt Pending
#define X86_FLAGS_VIP         (1 << X86_FLAGS_VIP_BIT)
#define X86_FLAGS_ID_BIT      21 // CPUID detection
#define X86_FLAGS_ID          (1 << X86_FLAGS_ID_BIT)


/**
 * Control register indexes.
 */
#define X86_CTRL_CR0 0
#define X86_CTRL_CR2 2
#define X86_CTRL_CR3 3
#define X86_CTRL_CR4 4
#define X86_CTRL_CR8 8

/**
 * Byte packed structure for Control Register 0 (CR0).
 *
 * Contains system control flags that control operating mode and states of the processor.
 *
 * 32-bits on i386.
 * 64-bits on amd64 (upper 32-bits are reserved).
 *
 * Intel SDM Vol. 3A - 2.5 CONTROL REGISTERS (Figure 2-7. Control Registers)
 * AMD APM Vol. 2 r3.30 - 3.1.1 CR0 Register (Figure 3-1. Control Register 0 (CR0))
 */
typedef union _X86_CR0 {
    struct _X86_CR0_BITS {
        UINT32 PE:1;            //!<   0   Protection Enable. Enables protected mode when set; enables real-address
                                //!<       mode when clear.
        UINT32 MP:1;            //!<   1   Monitor Coprocessor. Controls the interaction of the WAIT (or FWAIT)
                                //!<       instruction with the TS flag (bit 3 of CR0).
        UINT32 EM:1;            //!<   2   Emulation. Indicates that the processor does not have an internal or
                                //!<       external x87 FPU when set; indicates an x87 FPU is present when clear.
        UINT32 TS:1;            //!<   3   Task Switched.
        UINT32 ET:1;            //!<   4   Extension Type. Reserved in the Pentium 4, Intel Xeon, P6 family, and
                                //!<       Pentium processors. In the Pentium 4, Intel Xeon, and P6 family processors,
                                //!<       this flag is hardcoded to 1.
        UINT32 NE:1;            //!<   5   Numeric Error. Enables the native (internal) mechanism for reporting x87
                                //!<       FPU errors when set; enables the PC-style x87 FPU error reporting mechanism
                                //!<       when clear.
        UINT32 Reserved0:10;    //!< 15:6  Reserved.
        UINT32 WP:1;            //!<  16   Write Protect. When set, inhibits supervisor-level procedures from writing
                                //!<       into readonly pages; when clear, allows supervisor-level procedures to write
                                //!<       into read-only pages (regardless of the U/S bit setting).
        UINT32 Reserved1:1;     //!<  17   Reserved.
        UINT32 AM:1;            //!<  18   Alignment Mask. Enables automatic alignment checking when set; disables
                                //!<       alignment checking when clear.
        UINT32 Reserved2:10;    //!< 28:19 Reserved.
        UINT32 NW:1;            //!<  29   Not Write-through. When the NW and CD flags are clear, write-back
                                //!<       (Pentium 4, Intel Xeon, P6 family, and Pentium processors) or write-through
                                //!<       (for Intel486 processors) is enabled for writes that hit the cache and
                                //!<       invalidation cycles are enabled.
        UINT32 CD:1;            //!<  30   Cache Disable. When the CD and NW flags are clear, caching of memory
                                //!<       locations for the whole of physical memory in the processor's internal and
                                //!<       external caches is enabled.
        UINT32 PG:1;            //!<  31   Paging. Enables paging when set; disables paging when clear. When paging is
                                //!<       disabled, all linear addresses are treated as physical addresses.
    } Bits;
    UINT32 Uint32;
    UINT64 Uint64;
} PACKED X86_CR0;
C_ASSERT(FIELD_OFFSET(X86_CR0, Uint32) == 0 && FIELD_SIZEOF(X86_CR0, Uint32) == 4);
C_ASSERT(FIELD_OFFSET(X86_CR0, Uint64) == 0 && FIELD_SIZEOF(X86_CR0, Uint64) == 8);

#define X86_CR0_PE_BIT         0   // Protection Enable
#define X86_CR0_PE             (1 << X86_CR0_PE_BIT)
#define X86_CR0_MP_BIT         1   // Monitor Coprocessor
#define X86_CR0_MP             (1 << X86_CR0_MP_BIT)
#define X86_CR0_EM_BIT         2   // Emulation
#define X86_CR0_EM             (1 << X86_CR0_EM_BIT)
#define X86_CR0_TS_BIT         3   // Task Switched
#define X86_CR0_TS             (1 << X86_CR0_TS_BIT)
#define X86_CR0_ET_BIT         4   // Extension Type
#define X86_CR0_ET             (1 << X86_CR0_ET_BIT)
#define X86_CR0_NE_BIT         5   // Numeric Error
#define X86_CR0_NE             (1 << X86_CR0_NE_BIT)
#define X86_CR0_WP_BIT         16  // Write Protect
#define X86_CR0_WP             (1 << X86_CR0_WP_BIT)
#define X86_CR0_AM_BIT         18  // Alignment Mask
#define X86_CR0_AM             (1 << X86_CR0_AM_BIT)
#define X86_CR0_NW_BIT         29  // Not Write-through
#define X86_CR0_NW             (1 << X86_CR0_NW_BIT)
#define X86_CR0_CD_BIT         30  // Cache Disable
#define X86_CR0_CD             (1 << X86_CR0_CD_BIT)
#define X86_CR0_PG_BIT         31  // Paging
#define X86_CR0_PG             (1 << X86_CR0_PG_BIT)

/**
 * Byte packed structure for Control Register 3 (CR3).
 *
 * Contains the physical address of the base of the paging-structure hierarchy and two flags (PCD and
 * PWT). Only the most-significant bits (less the lower 12 bits) of the base address are specified; the lower
 * 12 bits of the address are assumed to be 0. The first paging structure must thus be aligned to a page (4-KByte)
 * boundary. The PCD and PWT flags control caching of that paging structure in the processor’s internal data
 * caches (they do not control TLB caching of page-directory information). The CR3 register is also commonly
 * referred to the page-directory base register (or PDBR for short).
 *
 * When using the physical address extension, the CR3 register contains the base address of the page-directory
 * pointer table. In IA-32e mode, the CR3 register contains the base address of the PML4 table.
 *
 * See also: Intel SDM Chapter 4, "Paging".
 *
 * 32-bits on i386.
 * 64-bits on amd64.
 *
 * Intel SDM Vol 3A - 2.5 CONTROL REGISTERS
 * Intel SDM Vol 3A - 4.5 4-LEVEL PAGING
 * AMD APM Vol. 2 r3.30 - 3.1.2 CR2 and CR3 Registers
 */
typedef union _X86_CR3 {
    struct _X86_CR3_BITS {
        UINT32 Reserved0:3;     //!<  2:0  Reserved.
        UINT32 PWT:1;           //!<   3   Page-level Write-Through (bit 3 of CR3). Controls the memory type used to
                                //!<       access the first paging structure of the current paging-structure hierarchy.
                                //!<       See Section 4.9, "Paging and Memory Typing". This bit is not used if paging
                                //!<       is disabled, with PAE paging, or with 4-level paging if CR4.PCIDE=1.
        UINT32 PCD:1;           //!<   4   Page-level Cache Disable (bit 4 of CR3). Controls the memory type used to
                                //!<       access the first paging structure of the current paging-structure hierarchy.
                                //!<       See Section 4.9, "Paging and Memory Typing". This bit is not used if paging
                                //!<       is disabled, with PAE paging, or with 4-level paging2 if CR4.PCIDE=1.
        UINT32 Reserved1:7;     //!< 11:5  Reserved.
        UINT32 BaseLow:20;      //!< 31:12 Page Table Base Address Lower 32 bits.
        UINT32 BaseHigh:20;     //!< 51:32 Page Table Base Address High 32 bits. (these bits exist only on processors
                                //!<       supporting the Intel-64 arch).
        UINT32 Reserved2:12;    //!< 63:52 Ignored (these bits exist only on processors supporting the Intel-64 arch).
    } Bits;
    UINT32 Uint32;
    UINT64 Uint64;
} PACKED X86_CR3;
C_ASSERT(FIELD_OFFSET(X86_CR3, Uint32) == 0 && FIELD_SIZEOF(X86_CR3, Uint32) == 4);
C_ASSERT(FIELD_OFFSET(X86_CR3, Uint64) == 0 && FIELD_SIZEOF(X86_CR3, Uint64) == 8);

typedef union _X86_CR3_PAE {
    struct _X86_CR3_PAE_BITS {
        UINT32 Reserved0:5;     //!<  4:0  Reserved.
        UINT32 Base:27;         //!< 31:5  Page Table Base Address.
        UINT32 Reserved1;       //!< 63:32 Ignored (these bits exist only on processors supporting the Intel-64 arch).
    } Bits;
    UINT32 Uint32;
    UINT64 Uint64;
} PACKED X86_CR3_PAE;
C_ASSERT(FIELD_OFFSET(X86_CR3_PAE, Uint32) == 0 && FIELD_SIZEOF(X86_CR3_PAE, Uint32) == 4);
C_ASSERT(FIELD_OFFSET(X86_CR3_PAE, Uint64) == 0 && FIELD_SIZEOF(X86_CR3_PAE, Uint64) == 8);

typedef union _X86_CR3_PCID {
    struct _X86_CR3_PCID_BITS {
        UINT32 PCID:12;         //!< 11:0  PCID (see Section 4.10.1 Process-Context Identifiers (PCIDs)).
        UINT32 BaseLow:20;      //!< 31:5  Page Table Base Address Lower 32 bits.
        UINT32 BaseHigh:20;     //!< 31:5  Page Table Base Address High 32 bits.
        UINT32 Reserved0:11;    //!< 62:52 Ignored.
        UINT32 NoInvalidate:1;  //!<  63   If this set to 1 on a MOV CR3 instruction, the instruction is not required
                                //!<       to invalidate any TLB entries or entries in paging-structure caches. If 0,
                                //!<       the instruction invalidates all TLB entries associated with the PCID
                                //!<       specified in bits 11:0 except those for global pages.
    } Bits;
    UINT32 Uint32;
    UINT64 Uint64;
} PACKED X86_CR3_PCID;
C_ASSERT(FIELD_OFFSET(X86_CR3_PCID, Uint32) == 0 && FIELD_SIZEOF(X86_CR3_PCID, Uint32) == 4);
C_ASSERT(FIELD_OFFSET(X86_CR3_PCID, Uint64) == 0 && FIELD_SIZEOF(X86_CR3_PCID, Uint64) == 8);

#define X86_CR3_PWT_BIT                 3   // Page Write Through
#define X86_CR3_PWT                     (1 << X86_CR3_PWT_BIT)
#define X86_CR3_PCD_BIT                 4   // Page Cache Disable
#define X86_CR3_PCD                     (1 << X86_CR3_PCD_BIT)
#define X64_CR3_PCID_NO_INVALIDATE_BIT  63 // Preserve old PCID
#define X64_CR3_PCID_NO_INVALIDATE      ((UINT64)1 << X64_CR3_PCID_NO_FLUSH_BIT)
#define X64_CR3_PCID_MASK               0x00000FFF // PCID Mask
#define X86_CR3_BASE_LOW_MASK           0xFFFFF000 // Base Low Mask
#define X86_CR3_BASE_HIGH_MASK          ((UINT64)0xFFFFF00000000) // Base High Mask
#define X86_CR3_BASE_MASK               ((UINT64)0xFFFFFFFFFF000) // Base Mask


/**
 * Byte packed structure for Control Register 4 (CR4).
 *
 * Contains a group of flags that enable several architectural extensions, and indicate operating
 * system or executive support for specific processor capabilities.
 *
 * 32-bits on i386.
 * 64-bits on amd64 (upper 32-bits are reserved).
 *
 * Intel SDM Vol 3A - 2.5 CONTROL REGISTERS (Figure 2-7. Control Registers)
 * AMD APM Vol. 2 r3.30 - 3.1.3 CR4 Register
 */
typedef union _X86_CR4 {
    struct _X86_CR4_BITS {
        UINT32 VME:1;           //!<   0   Virtual-8086 Mode Extensions.
        UINT32 PVI:1;           //!<   1   Protected-Mode Virtual Interrupts.
        UINT32 TSD:1;           //!<   2   Time Stamp Disable.
        UINT32 DE:1;            //!<   3   Debugging Extensions.
        UINT32 PSE:1;           //!<   4   Page Size Extensions.
        UINT32 PAE:1;           //!<   5   Physical Address Extension.
        UINT32 MCE:1;           //!<   6   Machine Check Enable.
        UINT32 PGE:1;           //!<   7   Page Global Enable.
        UINT32 PCE:1;           //!<   8   Performance Monitoring Counter Enable.
        UINT32 OSFXSR:1;        //!<   9   Operating System Support for FXSAVE and FXRSTOR instructions
        UINT32 OSXMMEXCPT:1;    //!<  10   Operating System Support for Unmasked SIMD Floating Point Exceptions.
        UINT32 UMIP:1;          //!<  11   User-Mode Instruction Prevention.
        UINT32 Reserved0:1;     //!<  12   Reserved.
        UINT32 VMXE:1;          //!<  13   VMX Enable
        UINT32 SMXE:1;          //!<  14   SMX Enable
        UINT32 Reserved1:1;     //!<  15   Reserved.
        UINT32 FSGSBASE:1;      //!<  16   Enables the instructions RDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE.
        UINT32 PCIDE:1;         //!<  17   PCID Enable. Enables process-context identifiers (PCIDs) when set.
        UINT32 OSXSAVE:1;       //!<  18   XSAVE and Processor Extended States-Enable.
        UINT32 Reserved2:1;     //!<  19   Reserved.
        UINT32 SMEP:1;          //!<  20   Supervisor Mode Execution Protection Enable.
        UINT32 SMAP:1;          //!<  21   Supervisor Mode Access Protection Enable
        UINT32 PKE:1;           //!<  22   Enables 4-level paging to associate each linear address with a protection key.
        UINT32 Reserved3:9;     //!< 31:23 Reserved.
        UINT32 Reserved4;       //!< 63:32 Reserved upper 32-bits.
    } Bits;
    UINT32 Uint32;
    UINT64 Uint64;
} PACKED X86_CR4, *PX86_CR4;
C_ASSERT(FIELD_OFFSET(X86_CR4, Uint32) == 0 && FIELD_SIZEOF(X86_CR4, Uint32) == 4);
C_ASSERT(FIELD_OFFSET(X86_CR4, Uint64) == 0 && FIELD_SIZEOF(X86_CR4, Uint64) == 8);

#define X86_CR4_VME_BIT        0   // Enable Virtual-8086 Mode Extensions
#define X86_CR4_VME            (1 << X86_CR4_VME_BIT)
#define X86_CR4_PVI_BIT        1   // Virtual Interrupts Flag Enable
#define X86_CR4_PVI            (1 << X86_CR4_PVI_BIT)
#define X86_CR4_TSD_BIT        2   // Time Stamp Disable at CPL3
#define X86_CR4_TSD            (1 << X86_CR4_TSD_BIT)
#define X86_CR4_DE_BIT         3   // Enable Debugging Extensions
#define X86_CR4_DE             (1 << X86_CR4_DE_BIT)
#define X86_CR4_PSE_BIT        4   // Enable Page Size Extensions
#define X86_CR4_PSE            (1 << X86_CR4_PSE_BIT)
#define X86_CR4_PAE_BIT        5   // Enable Physical Address Extensions
#define X86_CR4_PAE            (1 << X86_CR4_PAE_BIT)
#define X86_CR4_MCE_BIT        6   // Machine Check Enable
#define X86_CR4_MCE            (1 << X86_CR4_MCE_BIT)
#define X86_CR4_PGE_BIT        7   // Enable Global Pages
#define X86_CR4_PGE            (1 << X86_CR4_PGE_BIT)
#define X86_CR4_PCE_BIT        8   // Enable Performance Counters at CPL3
#define X86_CR4_PCE            (1 << X86_CR4_PCE_BIT)
#define X86_CR4_OSFXSR_BIT     9   // Enable Fast FPU Save and Restore
#define X86_CR4_OSFXSR         (1 << X86_CR4_OSFXSR_BIT)
#define X86_CR4_OSXMMEXCPT_BIT 10  // Enable Unmasked SSE Exceptions
#define X86_CR4_OSXMMEXCPT     (1 << X86_CR4_OSXMMEXCPT_BIT)
#define X86_CR4_VMXE_BIT       13  // Enable VMX Virtualization
#define X86_CR4_VMXE           (1 << X86_CR4_VMXE_BIT)
#define X86_CR4_SMXE_BIT       14  // Enable Safer Mode (TXT)
#define X86_CR4_SMXE           (1 << X86_CR4_SMXE_BIT)
#define X86_CR4_FSGSBASE_BIT   16  // Enable RDWRFSGS
#define X86_CR4_FSGSBASE       (1 << X86_CR4_FSGSBASE_BIT)
#define X86_CR4_PCIDE_BIT      17  // Enable PCID
#define X86_CR4_PCIDE          (1 << X86_CR4_PCIDE_BIT)
#define X86_CR4_OSXSAVE_BIT    18  // Enable XSAVE and XRESTORE
#define X86_CR4_OSXSAVE        (1 << X86_CR4_OSXSAVE_BIT)
#define X86_CR4_SMEP_BIT       20  // Enable SMEP
#define X86_CR4_SMEP           (1 << X86_CR4_SMEP_BIT)
#define X86_CR4_SMAP_BIT       21  // Enable SMAP
#define X86_CR4_SMAP           (1 << X86_CR4_SMAP_BIT)
#define X86_CR4_PKE_BIT        22  // Enable Protection Keys
#define X86_CR4_PKE            (1 << X86_CR4_PKE_BIT)


/**
 * Byte packed structure for an FP/SSE/SSE2 context.
 */
typedef struct _X86_FX_BUFFER {
    UINT8 Buffer[512];
} PACKED X86_FX_BUFFER;
C_ASSERT(sizeof(X86_FX_BUFFER) == 512);

#if defined(_MSC_VER)
#pragma pack(pop)
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif // _ARCH_X86_H_
