;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _draw_shader_transparent_chicago_hook
_draw_shader_transparent_chicago_hook:
    cmp byte ptr _use_shader_transparent_chicago_reimplementation, 0
    jz skip
    push eax
    push ebx
    call _draw_shader_transparent_chicago
    add esp, 8
    mov eax, 1
    ret
    skip:
    mov eax, 0
    ret
