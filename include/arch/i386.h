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
 * @authors EDK2 Team (various little things)
 *          Aidan Khoury (ajkhoury)
 * @date 12/22/2017
 *
 * @brief Anything specific to the x86/i386 architecture.
 */

#ifndef _ARCH_I386_H_
#define _ARCH_I386_H_

//
// Specific to the I386 architecture
//

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
 * | BASE 31..24 |G|/|0|V| LIMIT |P|DPL|  TYPE   | BASE 23:16 |
 * |             | |D| |L| 19:16 | |   |1|1|C|R|A|            |
 * ------------------------------------------------------------
 * |                             |                            |
 * |        BASE 15:00           |        LIMIT 15:00         |
 * |                             |                            |
 * ------------------------------------------------------------
 */

/**
 * Byte packed structure for a 32-bit segment descriptor in a GDT/LDT.
 *
 * Intel SDM Volume 3A - 3.4.5 Segment Descriptors.
 * AMD APM Vol. 2 r3.30 - 4.7 Legacy Segment Descriptors.
 */
typedef union _I386_SEGMENT_DESCRIPTOR {
    struct {
        UINT16 LimitLow;        //!< 0x00 Limit low bits 15:00
        UINT16 BaseLow;         //!< 0x02 Base low bits 15:00
        UINT8 BaseMiddle;       //!< 0x04 Base middle bits 23:16
        UINT8 Flags1;           //!< 0x05 Access flags
        UINT8 Flags2;           //!< 0x06 Granularity, etc. flags
        UINT8 BaseHigh;         //!< 0x07 Base high bits 23:16
    } Bytes;
    struct {
        UINT32 LimitLow:16;     //!< Limit low bits 15:00

        UINT32 BaseLow:16;      //!< Base low bits 15:00
        UINT32 BaseMid:8;       //!< Base middle bits 23:16

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

        UINT32 L:1;             //!< Reserved in 32 bit (L bit in 64-bit)

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
} PACKED I386_SEGMENT_DESCRIPTOR, *PI386_SEGMENT_DESCRIPTOR;
C_ASSERT(sizeof(I386_SEGMENT_DESCRIPTOR) == 8);


/**
 * Byte packed structure for an i386 IDTR, GDTR, LDTR descriptor.
 *
 * Intel SDM Vol. 3A - 3.5.1 Segment Descriptor Tables (Figure 3-11. Pseudo-Descriptor Formats)
 * AMD APM Vol. 2 r3.30 - 4.6.2 Global Descriptor-Table Register
 */
typedef struct _I386_DESCRIPTOR {
    UINT16 Limit;
    UINT32 Base;
} PACKED I386_DESCRIPTOR, *PI386_DESCRIPTOR;
C_ASSERT(sizeof(I386_DESCRIPTOR) == 6);

/**
 * Byte packed structure for an i386 Interrupt Gate Descriptor.
 *
 * Intel SDM Volume 3A - 6.11 IDT Descriptors (Figure 6-2. IDT Gate Descriptors)
 * AMD64 SPM Rev. 3.28 - 4.7.5 Gate Descriptors (Figure 4-18. Interrupt-Gate and Trap-Gate Descriptors)
 */
typedef union _I386_IDT_GATE_DESCRIPTOR {
    struct {
        UINT16 OffsetLow;       //!< 0x00 Offset bits 15..0.
        UINT16 Selector;        //!< 0x02 Selector.
        UINT8 Reserved0;        //!< 0x04 Reserved.
        UINT8 GateType:4;       //!< 0x05 Gate Type.  See #defines above.
        UINT8 S:1;              //!< 0x05 S bit which should usually be zero.
        UINT8 Dpl:2;            //!< 0x05 Descriptor privilege level.
        UINT8 Present:1;        //!< 0x05 Present bit.
        UINT16 OffsetHigh;      //!< 0x06 Offset bits 31..16.
    } Bits;
    UINT64 Uint64;              //!< 0x00
} PACKED I386_IDT_GATE_DESCRIPTOR;
C_ASSERT(sizeof(union _I386_IDT_GATE_DESCRIPTOR) == 8);

/**
 * 32-bit Task State Segment structure.
 *
 * Intel SDM Volume 3A - Section 7.2.1 Task-State Segment (TSS)
 *
 */
typedef struct _I386_TASK_STATE_SEGMENT {
    UINT16 BackLink;
    UINT16 Reserved0;
    UINT32 Esp0;
    UINT16 Ss0;
    UINT16 Reserved1;
    UINT32 Esp1;
    UINT16 Ss1;
    UINT16 Reserved2;
    UINT32 Esp2;
    UINT16 Ss2;
    UINT16 Reserved3;
    UINT32 Cr3;
    UINT32 Eip;
    UINT32 Eflags;
    UINT32 Eax;
    UINT32 Ecx;
    UINT32 Edx;
    UINT32 Ebx;
    UINT32 Esp;
    UINT32 Ebp;
    UINT32 Esi;
    UINT32 Edi;
    UINT16 Es;
    UINT16 Reserved4;
    UINT16 Cs;
    UINT16 Reserved5;
    UINT16 Ss;
    UINT16 Reserved6;
    UINT16 Ds;
    UINT16 Reserved7;
    UINT16 Fs;
    UINT16 Reserved8;
    UINT16 Gs;
    UINT16 Reserved9;
    UINT16 Ldt;
    UINT16 Reserved10;
    UINT16 DebugTrapFlag;
    UINT16 IoMapBase;
//  UINT8 IoMap[8192];
} PACKED I386_TASK_STATE_SEGMENT;

typedef union _I386_TSS_DESCRIPTOR {
    struct {
        UINT32 LimitLow:16;         //!< Segment Limit 15..00
        UINT32 BaseLow:16;          //!< Base Address  15..00
        UINT32 BaseMid:8;           //!< Base Address  23..16
        UINT32 Type:4;              //!< Type (1 0 B 1)
        UINT32 Reserved0:1;         //!< 0
        UINT32 DPL:2;               //!< Descriptor Privilege Level
        UINT32 P:1;                 //!< Segment Present
        UINT32 LimitHigh:4;         //!< Segment Limit 19..16
        UINT32 AVL:1;               //!< Available for use by system software
        UINT32 Reserved1:2;         //!< 0 0
        UINT32 G:1;                 //!< Granularity
        UINT32 BaseHigh:8;          //!< Base Address 31..24
    } Bits;
    struct {
        UINT32 Low;
        UINT32 High;
    } Uint32;
    UINT64 Uint64;
} PACKED I386_TSS_DESCRIPTOR;

/**
 * i386 Segment Selector
 *
 * Intel SDM Vol 3A - 3.4.2 Segment Selectors (Figure 3-6. Segment Selector)
 */
typedef union _I386_SEGMENT_SELECTOR {
    struct {
        UINT16 Rpl:2;               //   1:0    Requested Privilege Level (RPL). Specifies the privilege level of the
                                    //          selector. The privilege level can range from 0 to 3, with 0 being the
                                    //          most privileged level. See Section 5.5, "Privilege Levels", for a
                                    //          description of the relationship of the RPL to the CPL of the executing
                                    //          program (or task) and the descriptor privilege level (DPL) of the
                                    //          descriptor the segment selector points to.
        UINT16 TableIndicator:1;    //    2     TI (table indicator) flag. Specifies the descriptor table to use:
                                    //          clearing this flag selects the GDT; setting this flag selects the
                                    //          current LDT.
        UINT16 Index:13;            //  15:3    Index. Selects one of 8192 descriptors in the GDT or LDT. The processor
                                    //          multiplies the index value by 8 (the number of bytes in a segment
                                    //          descriptor) and adds the result to the base address of the GDT or LDT
                                    //          (from the GDTR or LDTR register, respectively).
    } Bits;
    UINT16 Uint16;
} PACKED I386_SEGMENT_SELECTOR, *PI386_SEGMENT_SELECTOR;
C_ASSERT(sizeof(I386_SEGMENT_SELECTOR) == 2);


#if defined(_MSC_VER)
#pragma pack(pop)
#endif

#endif // _ARCH_I386_H_
