;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _reserve_read_region_permutations_local_vars_size_asm
_reserve_read_region_permutations_local_vars_size_asm:
    sub esp, 0x40 ;# add space for 32 region permutations
    jmp dword ptr _reserve_read_region_permutations_local_vars_size_return

.globl _free_read_region_permutations_local_vars_size_asm
_free_read_region_permutations_local_vars_size_asm:
    pop edi
    pop ebx
    add esp, 0x8C ;# free local variables total space; 0x8C = 0x4C + 0x40
    ret

.globl _read_region_permutations_param_1_read_1_asm
_read_region_permutations_param_1_read_1_asm:
    mov ecx, dword ptr [esp + 0x9C] ;# displace param 1 offset; 0x9C = 0x58 + 0x40 + 0x4 (hook return address)
    lea eax, dword ptr [eax + eax * 2]
    ret

.globl _read_region_permutations_param_1_read_2_asm
_read_region_permutations_param_1_read_2_asm:
    mov ecx, dword ptr [esp + 0xA4] ;# displace param 1 offset; 0xA4 = 0x60 + 0x40 + 0x4 (hook return address)
    inc eax
    ret
