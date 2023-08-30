;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data

load_bitmap_address: .long 0

.text

load_bitmap_function_sig: .string "load_bitmap_function"

.globl _load_bitmap_asm
_load_bitmap_asm:
    cmp dword ptr load_bitmap_address, 0
    jnz load_bitmap_function_sig_found
    lea eax, load_bitmap_function_sig
    push eax
    call _get_address_for_signature
    mov load_bitmap_address, eax
    add esp, 4
    load_bitmap_function_sig_found:

    push esi

    ;# Get params
    mov eax, [esp + 0x4]
    mov ebx, [eax + 0x8]
    mov ecx, [eax + 0xC]

    push ecx
    push ebx
    mov esi, eax
    call dword ptr load_bitmap_address
    add esp, 8

    pop esi
    ret
