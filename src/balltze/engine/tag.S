;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data

get_tag_address: .long 0
tag_handle: .long 0

.text

get_tag_handle_sig: .string "get_tag_handle"

.globl _get_tag_handle
_get_tag_handle:
    pushad
    pushfd

    cmp dword ptr get_tag_address, 0
    jnz found
    lea eax, get_tag_handle_sig
    push eax
    call _get_address_for_signature
    mov get_tag_address, eax
    add esp, 4
    found:

    ;# Get tag path
    mov eax, [esp + 0x28]

    ;# Get tag class
    mov ebx, [esp + 0x2C]

    ;# Find it!
    push eax
    mov edi, ebx
    call dword ptr get_tag_address
    add esp, 4
    mov tag_handle, eax
    
    popfd
    popad

    ;# Return value
    mov eax, tag_handle
    ret
