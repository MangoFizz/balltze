;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data

load_bitmap_address: .long 0

.text

load_bitmap_function_sig: .string "load_bitmap_function"

.globl _load_bitmap_asm
_load_bitmap_asm:
    pushad
    pushfd
    
    cmp dword ptr load_bitmap_address, 0
    jnz load_bitmap_function_sig_found
    lea eax, load_bitmap_function_sig
    push eax
    call _get_address_for_signature
    mov load_bitmap_address, eax
    add esp, 4
    load_bitmap_function_sig_found:

    mov eax, [esp + 0x28]
    push [esp + 0x30]
    push [esp + 0x30]
    call dword ptr load_bitmap_address
    add esp, 8

    popfd
    popad
    ret
