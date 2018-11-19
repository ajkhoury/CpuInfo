/**
 * CpuInfo
 * Copyright (c) 2006-2018, Intel Corporation. All rights reserved.
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
 * @authors EDK2 Team (little things)
 *          Aidan Khoury (ajkhoury)
 * @date 12/22/2017
 *
 * @brief Anything specific to the AMD64/EM64T/IA-32e architectures.
 */

#ifndef _ARCH_X64_H_
#define _ARCH_X64_H_
#if defined(_MSC_VER)
#pragma once
#endif

#include "types.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4214) // nonstandard extension used: bit field types other than int
#endif

//
// XCR0 register features.
//
#define X64_XCR_XFEATURE_ENABLED_MASK 0


//
// The following structures must by 1-byte packed/aligned.
//
#if defined(_MSC_VER)
#pragma pack(push, 1)
#endif

/** GDT Segment Descriptor
 *
 * 31          24         19    16                7           0
 * ------------------------------------------------------------
 * |             | |B| |A|       | |   |1|0|E|W|A|            |
 * | BASE 31..24 |G|/|L|V| LIMIT |P|DPL|  TYPE   | BASE 23:16 |
 * |             | |D| |L| 19:16 | |   |1|1|C|R|A|            |
 * ------------------------------------------------------------
 * |                             |                            |
 * |        BASE 15:00           |        LIMIT 15:00         |
 * |                             |                            |
 * ------------------------------------------------------------
 */

/**
 * Byte packed structure for a 64-bit Code or Data Segment Sescriptor.
 *
 * Intel SDM Vol. 3A - 3.4.5 Segment Descriptors (Figure 3-8. Segment Descriptor)
 * AMD APM Vol. 2 r3.30 - 4.8 Long-Mode Segment Descriptors (Figure 4-20. Code-Segment Descriptor-Long Mode)
 */
typedef union _X64_SEGMENT_DESCRIPTOR {
    struct _X64_SEGMENT_DESCRIPTOR_BYTES {
        UINT16 LimitLow;        //!< 0x00 Limit low bits 15:00
        UINT16 BaseLow;         //!< 0x02 Base low bits 15:00
        UINT8 BaseMiddle;       //!< 0x04 Base middle bits 23:16
        UINT8 Flags1;           //!< 0x05 Access flags
        UINT8 Flags2;           //!< 0x06 Limit, AVL, Granularity, etc.
        UINT8 BaseHigh;         //!< 0x07 Base high bits 23:16
    } Bytes;
    struct _X64_SEGMENT_DESCRIPTOR_BITS {
        UINT32 LimitLow:16;     //!< Limit low bits 15:00

        UINT32 BaseLow:16;      //!< Base low bits 15:00
        UINT32 BaseMiddle:8;    //!< Base middle bits 23:16

        UINT32 Type:4;          //!< Segment type. Indicates the segment or gate type and specifies the kinds of access
                                //!< that can be made to the segment and the direction of growth. The interpretation of
                                //!< this field depends on whether the descriptor type flag specifies an application
                                //!< (code or data) descriptor or a system descriptor. The encoding of the type field
                                //!< is different for code, data, and system descriptors (see Figure 5-1).
                                //!< See Section 3.4.5.1, 'Code- and Data-Segment Descriptor Types', for a description
                                //!< of how this field is used to specify code and data-segment types.

        UINT32 S:1;             //!< S descriptor type flag. Specifies whether the segment descriptor is for a system
                                //!< segment (S flag is clear), or a code or data segment (S flag is set).

        UINT32 DPL:2;           //!< Descriptor privilege level. Specifies the privilege level of the segment.
                                //!< The privilege level can range from 0 to 3, with 0 being the most privileged level.
                                //!< The DPL is used to control access to the segment.
                                //!< See Section 5.5, 'Privilege Levels', for a description of the relationship of the
                                //!< DPL to the CPL of the executing code segment and the RPL of a segment selector.

        UINT32 P:1;             //!< Segment present. Indicates whether the segment is present in memory (set) or
                                //!< not present (clear). Figure 3-9 in section 3.4.5.1 shows the format of a segment
                                //!< descriptor when the segment-present flag is clear.

        UINT32 LimitHigh:4;     //!< Limit high bits 19:16

        UINT32 AVL:1;           //!< Available for use by system software.

        UINT32 L:1;             //!< 64-bit code segment flag (IA-32e mode only). In IA-32e mode, bit 21 of the second
                                //!< doubleword of the segment descriptor indicates whether a code segment contains
                                //!< native 64-bit code. A value of 1 indicates instructions in this code segment are
                                //!< executed in 64-bit mode. A value of 0 indicates the instructions in this code
                                //!< segment are executed in compatibility mode. If L-bit is set, then D-bit must be
                                //!< cleared. When not in IA-32e mode or for non-code segments, bit 21 is reserved and
                                //!< should always be set to 0.

        UINT32 DB:1;            //!< Default operation size. Performs different functions depending on whether the
                                //!< segment descriptor is an executable code segment, an expand-down data segment, or
                                //!< a stack segment. (This flag should always be set to 1 for 32-bit code and data
                                //!< segments and to 0 for 16-bit code and data segments.)

        UINT32 G:1;             //!< Granularity flag. Determines the scaling of the segment limit field. When the
                                //!< granularity flag is clear, the segment limit is interpreted in byte units; when
                                //!< flag is set, the segment limit is interpreted in 4-KByte units. (This flag does
                                //!< not affect the granularity of the base address; it is always byte granular.)

        UINT32 BaseHigh:8;      //!< Base high bits 31:24
    } Bits;
    UINT64 Uint64;
} PACKED X64_SEGMENT_DESCRIPTOR, *PX64_SEGMENT_DESCRIPTOR;
C_ASSERT(sizeof(X64_SEGMENT_DESCRIPTOR) == 8);

/**
 * Byte packed structure for a 64-bit System Segment Sescriptor.
 *
 * Intel SDM Vol. 3A - 7.2.3 TSS Descriptor in 64-bit mode (Figure 7-4. Format of TSS and LDT Descriptors in 64-bit Mode)
 * AMD APM Vol. 2 r3.30 - 4.8.3 System Descriptors (Figure 4-22. System-Segment Descriptor-64-Bit Mode)
 */
typedef union _X64_SYSTEM_SEGMENT_DESCRIPTOR {
    struct _X64_SYSTEM_SEGMENT_DESCRIPTOR_BYTES {
        UINT16 LimitLow;        //!< Limit low bits [15:0]
        UINT16 BaseLow;         //!< Base low bits [15:0]
        UINT8 BaseMiddle;       //!< Base middle bits [23:16]
        UINT8 Flags1;           //!< Access flags
        UINT8 Flags2;           //!< Granularity, etc. flags
        UINT8 BaseHigh;         //!< Base high bits [23:16]
        UINT32 BaseUpper;       //!< Base upper bits [63:32]
        UINT32 Reserved;        //!< Must always be zero
    } Bytes;
    struct _X64_SYSTEM_SEGMENT_DESCRIPTOR_BITS {
        UINT32 LimitLow:16;     //!< Limit low bits [15:0]

        UINT32 BaseLow:16;      //!< Base low bits [15:0]
        UINT32 BaseMid:8;       //!< Base middle bits [23:16]

        UINT32 Type:4;          //!< Segment type. Indicates the segment or gate type and specifies the kinds of access
                                //!< that can be made to the segment and the direction of growth. The interpretation of
                                //!< this field depends on whether the descriptor type flag specifies an application
                                //!< (code or data) descriptor or a system descriptor. The encoding of the type field
                                //!< is different for code, data, and system descriptors (see Figure 5-1).
                                //!< See Section 3.4.5.1, 'Code- and Data-Segment Descriptor Types', for a description
                                //!< of how this field is used to specify code and data-segment types.

        UINT32 S:1;             //!< S descriptor type flag. Specifies whether the segment descriptor is for a system
                                //!< segment (S flag is clear), or a code or data segment (S flag is set).

        UINT32 DPL:2;           //!< Descriptor privilege level. Specifies the privilege level of the segment.
                                //!< The privilege level can range from 0 to 3, with 0 being the most privileged level.
                                //!< The DPL is used to control access to the segment.
                                //!< See Section 5.5, 'Privilege Levels', for a description of the relationship of the
                                //!< DPL to the CPL of the executing code segment and the RPL of a segment selector.

        UINT32 P:1;             //!< Segment present. Indicates whether the segment is present in memory (set) or
                                //!< not present (clear). Figure 3-9 in section 3.4.5.1 shows the format of a segment
                                //!< descriptor when the segment-present flag is clear.

        UINT32 LimitHigh:4;     //!< Limit high bits [19:16]

        UINT32 AVL:1;           //!< Available for use by system software.

        UINT32 L:1;             //!< 64-bit code segment flag (IA-32e mode only). In IA-32e mode, bit 21 of the second
                                //!< doubleword of the segment descriptor indicates whether a code segment contains
                                //!< native 64-bit code. A value of 1 indicates instructions in this code segment are
                                //!< executed in 64-bit mode. A value of 0 indicates the instructions in this code
                                //!< segment are executed in compatibility mode. If L-bit is set, then D-bit must be
                                //!< cleared. When not in IA-32e mode or for non-code segments, bit 21 is reserved and
                                //!< should always be set to 0.

        UINT32 DB:1;            //!< Default operation size. Performs different functions depending on whether the
                                //!< segment descriptor is an executable code segment, an expand-down data segment, or
                                //!< a stack segment. (This flag should always be set to 1 for 32-bit code and data
                                //!< segments and to 0 for 16-bit code and data segments.)

        UINT32 G:1;             //!< Granularity flag. Determines the scaling of the segment limit field. When the
                                //!< granularity flag is clear, the segment limit is interpreted in byte units; when
                                //!< flag is set, the segment limit is interpreted in 4-KByte units. (This flag does
                                //!< not affect the granularity of the base address; it is always byte granular.)

        UINT32 BaseHigh:8;      //!< Base high bits [31:24]
        UINT64 BaseUpper:32;    //!< Base upper bits [63:32]
        UINT64 Reserved:32;     //!< Must always be zero
    } Bits;
    UINT128 Uint128;
} PACKED X64_SYSTEM_SEGMENT_DESCRIPTOR, *PX64_SYSTEM_SEGMENT_DESCRIPTOR;
C_ASSERT(sizeof(X64_SYSTEM_SEGMENT_DESCRIPTOR) == 16);


/** Get GDT descriptor base address */
#define X64_GDT_BASE(desc) \
    (((desc)->Bytes.BaseHigh << 24 | (desc)->Bytes.BaseMiddle << 16 | (desc)->Bytes.BaseLow) |  \
        (((desc)->Bytes.Flags1 & 0x10) ? ((((X64_SYSTEM_SEGMENT_DESCRIPTOR*)(desc))->Bytes.BaseUpper) << 32) : 0))

/**
 * Byte packed structure for an IDTR, GDTR, LDTR descriptor.
 *
 * Intel SDM Vol. 3A - 3.5.1 Segment Descriptor Tables (Figure 3-11. Pseudo-Descriptor Formats)
 * AMD APM Vol. 2 r3.30 - 4.6.2 Global Descriptor-Table Register (Figure 4-8. GDTR and IDTR Format-Long Mode)
 */
typedef struct _X64_DESCRIPTOR {
    UINT16 Limit;
    UINT64 Base;
} PACKED X64_DESCRIPTOR;
C_ASSERT(sizeof(X64_DESCRIPTOR) == 10);

/**
 * Byte packed structure for an X64 Interrupt Gate Descriptor.
 *
 * Intel SDM Vol. 3A - 6.14.1 64-Bit Mode IDT (Figure 6-7. 64-Bit IDT Gate Descriptors)
 * AMD APM Vol. 2 - 4.8.4 Gate Descriptors (Figure 4-24. Interrupt-Gate and Trap-Gate Descriptors)
 */
typedef union _X64_IDT_GATE_DESCRIPTOR {
    struct _X64_IDT_GATE_DESCRIPTOR_BITS {
        UINT16 OffsetLow;       //!< 0x00 Offset bits 15..0.
        UINT16 Selector;        //!< 0x02 Selector.
        UINT8 Ist:3;            //!< 0x04 IST Interrupt Stack Table.
        UINT8 Reserved0:5;      //!< 0x04 Reserved (must be zero).
        UINT8 GateType:4;       //!< 0x05 bits 03:00 Gate Type. See #defines above.
        UINT8 S:1;              //!< 0x05 bits 04:04 S bit which should always be zero.
        UINT8 Dpl:2;            //!< 0x05 bits 06:05 Descriptor privilege level.
        UINT8 Present:1;        //!< 0x05 bits 07:07 Present bit.
        UINT16 OffsetMiddle;    //!< 0x06 Offset bits 31..16.
        UINT64 OffsetHigh:32;   //!< 0x08 Offset bits 63..32.
        UINT64 Reserved1:32;    //!< 0x0C Reserved.
    } Bits;
    UINT128 Uint128;            //!< 0x00
} PACKED X64_IDT_GATE_DESCRIPTOR;
C_ASSERT(sizeof(union _X64_IDT_GATE_DESCRIPTOR) == 16);



/**
 * 64-bit Task State Segment.
 *
 * Intel SDM Vol. 3A - 7.2.1 Task-State Segment (TSS) (Figure 7-11. 64-Bit TSS Format)
 * AMD APM Vol. 2 r3.30 - 12.2.5 64-Bit Task State Segment (Figure 12-8. Long Mode TSS Format)
 */
typedef struct _X64_TASK_STATE_SEGMENT {
    UINT32 Reserved0;               //!< 0x00
    UINT64 Rsp0;                    //!< 0x04
    UINT64 Rsp1;                    //!< 0x0C
    UINT64 Rsp2;                    //!< 0x14
    UINT64 ReservedIst0;            //!< 0x1C
    UINT64 Ist1;                    //!< 0x24
    UINT64 Ist2;                    //!< 0x2C
    UINT64 Ist3;                    //!< 0x34
    UINT64 Ist4;                    //!< 0x3C
    UINT64 Ist5;                    //!< 0x44
    UINT64 Ist6;                    //!< 0x4C
    UINT64 Ist7;                    //!< 0x54
    UINT64 Reserved1;               //!< 0x5C
    UINT16 Reserved2;               //!< 0x64
    UINT16 IoMapBase;               //!< 0x66
//  UINT8 IoMap[8192];              //!< 0x68
} PACKED X64_TASK_STATE_SEGMENT;

/**
 * 64-bit Task State Segment Descriptor.
 *
 * Intel SDM Vol. 3A - 7.2.3 TSS Descriptor in 64-bit mode (Figure 7-4. Format of TSS and LDT Descriptors in 64-bit Mode)
 * AMD APM Vol. 2 r3.30 - 12.2.5 64-Bit Task State Segment (Figure 12-8. Long Mode TSS Format)
 */
typedef union _X64_TSS_DESCRIPTOR {
    struct _X64_TSS_DESCRIPTOR_BITS {
        UINT32 LimitLow:16;     //!< 15:0  Segment Limit [15:00]
        UINT32 BaseLow:16;      //!< 31:16 Base Address  [15:00]

        UINT32 BaseMid:8;       //!< 39:32 Base Address  [23..16]
        UINT32 Type:4;          //!< 43:40 Type (1 0 B 1)
        UINT32 Reserved0:1;     //!<  44   Reserved.
        UINT32 DPL:2;           //!< 46:45 Descriptor Privilege Level
        UINT32 P:1;             //!<  47   Segment Present
        UINT32 LimitHigh:4;     //!< 51:48 Segment Limit 19..16
        UINT32 AVL:1;           //!<  52   Available for use by system software
        UINT32 Reserved1:1;     //!<  53   Reserved.
        UINT32 Reserved2:1;     //!<  54   Reserved.
        UINT32 G:1;             //!<  55   Granularity
        UINT32 BaseHigh:8;      //!< 63:56 Base Address [31..24]
        UINT64 BaseUpper:32;    //!< 95:64 Base Address [63..32]
        UINT64 Reserved3:32;    //!<127:96 Reserved.
    } Bits;
    UINT128 Uint128;
} PACKED X64_TSS_DESCRIPTOR, *PX64_TSS_DESCRIPTOR;
C_ASSERT(sizeof(X64_TSS_DESCRIPTOR) == 16);

/**
 * Definition of a hardware linear address.
 *
 * Intel SDM Vol. 3A - 4.5 4-LEVEL PAGING (Figure 4-8. Linear-Address Translation to a 4-KByte Page using 4-Level Paging)
 * AMD APM Vol. 2 r3.30 - 5.3.3 4-Kbyte Page Translation (Figure 5-17. 4-Kbyte Page Translation-Long Mode)
 */
typedef struct _X64_LINEAR_ADDRESS {
    union _X64_LINEAR_ADDRESS_UNION {
        UINT64 Address;
        struct _X64_LINEAR_ADDRESS_BITS {
            UINT64 PageOffset:12;   //!< 11:0  Byte offset into the physical page.
            UINT64 PTEIndex:9;      //!< 20:12 Page Table Entry index.
            UINT64 PDTEIndex:9;     //!< 29:21 Page Directory Entry index.
            UINT64 PDPTEIndex:9;    //!< 38:30 Page Directory Pointer Entry index.
            UINT64 PML4EIndex:9;    //!< 47:39 Page Map Level 4 Table Entry index.
                                    //!<       A.K.A. eXtended Page Directory Entry (PXE) index.
            UINT64 Reserved0:16;    //!< 63:48 Sign extension of bit 47, as required for canonical-address forms.
        } Bits;
    } u;
} PACKED X64_LINEAR_ADDRESS, *PX64_LINEAR_ADDRESS;
C_ASSERT(sizeof(X64_LINEAR_ADDRESS) == 8);

/**
 * A Page Table Entry on an X64 system has the following definition.
 *
 * Intel SDM Vol. 3A - 4.4.2 Linear-Address Translation with PAE Paging (Table 4-11. Format of a PAE Page-Table Entry
 *                                                                                   that Maps a 4-KByte Page)
 * AMD APM Vol. 2 r3.30 - 5.3.3 4-Kbyte Page Translation (Figure 5-21. 4-Kbyte PTE-Long Mode)
 */
#define _X64_HARDWARE_PTE_WORKING_SET_BITS  11

typedef struct _X64_PTE {
    UINT64 P                : 1;    //  0   Present (P) Bit.                        Mask 0x01
    UINT64 RW               : 1;    //  1   Read/Write (R/W) Bit.                   Mask 0x02
    UINT64 US               : 1;    //  2   User/Supervisor (U/S) Bit.              Mask 0x04
    UINT64 PWT              : 1;    //  3   Page-Level Writethrough (PWT) Bit.      Mask 0x08
    UINT64 PCD              : 1;    //  4   Page-Level Cache Disable (PCD) Bit.     Mask 0x10
    UINT64 A                : 1;    //  5   Accessed (A) Bit.                       Mask 0x20
    UINT64 D                : 1;    //  6   Dirty (D) Bit.                          Mask 0x40
    UINT64 PS               : 1;    //  7   Page Size (PS) Bit. If the lowest level is a PTE (PDE.PS=0),
                                    //      PAT occupies bit 7. If the lowest level is a PDE (PDE.PS=1) or
                                    //      PDPE (PDPE.PS=1), PAT occupies bit 12.  Mask 0x80
    UINT64 G                : 1;    //  8   Global Page (G) Bit. Mask 0x100
    UINT64 AVL              : 3;    // 11:9 Available to Software (AVL).
    UINT64 PFN              : 40;   // (M–1):12 Page Frame Number or Physical Page Base Address. This is has an
                                    //      architectural limit of 40 bits. A given implementation may support
                                    //      fewer bits.
    UINT64 Reserved         : _X64_HARDWARE_PTE_WORKING_SET_BITS; // 62:M Reserved (must be 0)
    UINT64 XD               : 1;    //  63  No Execute (NX) Bit.
} PACKED X64_PTE, *PX64_PTE;
C_ASSERT(sizeof(X64_PTE) == 8);


#if defined(_MSC_VER)
#pragma pack(pop)
#endif

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif // _ARCH_X64_H_
