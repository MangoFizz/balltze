;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data
draw_hud_bitmap_function: .long 0

.text

draw_hud_bitmap_function_sig: .string "draw_hud_bitmap_function"

.globl _hud_draw_bitmap_sprite
_hud_draw_bitmap_sprite:
    pushad
    pushfd

    cmp dword ptr draw_hud_bitmap_function, 0
    jnz function_found
    lea eax, draw_hud_bitmap_function_sig
    push eax
    call _get_address_for_signature
    mov draw_hud_bitmap_function, eax
    add esp, 4
    function_found:

    ;# Get arguments
    lea eax, [esp + 0x28]
    mov ebx, [esp + 0x2C]
    mov ecx, [esp + 0x30]
    mov edx, [esp + 0x34]

    push ebx
    push 0
    push 0x3F800000
    push eax
    push 2

    lea eax, [edx + 0x8]
    mov edi, eax
    mov edx, ecx
    mov ebp, ecx
    mov ecx, 0x7FFFFF00

    call dword ptr draw_hud_bitmap_function
    add esp, 0x14

    popfd
    popad

    ret
