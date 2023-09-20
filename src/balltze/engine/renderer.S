;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _load_bitmap_asm
_load_bitmap_asm:
    push eax
    mov eax, [esp + 0x8]
    push [esp + 0xC]
    push [esp + 0xC]
    call dword ptr _load_bitmap_function_address
    add esp, 8
    pop eax
    ret

.globl _render_ui_asm
_render_ui_asm:
    mov eax, [esp + 0x4]
    call dword ptr _render_ui_function_address
    ret
