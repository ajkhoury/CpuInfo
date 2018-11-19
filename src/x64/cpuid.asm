;++
; CpuInfo
; Copyright (c) 2017-2018, Aidan Khoury. All rights reserved.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
;
; @file cpuid.asm
; @author Aidan Khoury (ajkhoury)
; @date 11/15/2018
;
; @brief Cpuid functions for the x86-64 (64-bit) architecture.
;--

[BITS 64]
DEFAULT REL

SECTION .text

;
; void __cpuid( int _Info[4], int _Level );
;
global ASM_PFX(__cpuid)
ASM_PFX(__cpuid):
    push    rbx
    push    rbp
    mov     rbp, rcx
    mov     rax, rdx
    xor     rcx, rcx
    cpuid
    mov     dword [rbp + 0], eax
    mov     dword [rbp + 4], ebx
    mov     dword [rbp + 8], ecx
    mov     dword [rbp + 0Ch], edx
    pop     rbp
    pop     rbx
    ret

;
; void __cpuidex( int _Info[4], int _Level, int _Ecx );
;
global ASM_PFX(__cpuidex)
ASM_PFX(__cpuidex):
    push    rbx
    push    rbp
    mov     rbp, rcx
    mov     rax, rdx
    mov     rcx, r8
    cpuid
    mov     dword [rbp + 0], eax
    mov     dword [rbp + 4], ebx
    mov     dword [rbp + 8], ecx
    mov     dword [rbp + 0Ch], edx
    pop     rbp
    pop     rbx
    ret
