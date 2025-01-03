;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _draw_shader_transparent_generic_hook
_draw_shader_transparent_generic_hook:
    cmp eax, 4
    jne continue_to_shaders_switch

    pushad
    push eax
    push edx
    mov edx, dword ptr [ebp + 0xC]
    push edx
    push ebx
    call _rasterizer_shader_transparent_generic_draw
    add esp, 8
    pop edx
    pop eax
    popad

    continue_to_shaders_switch:
    cmp eax, 0xA
    ja skip_shaders_switch
    jmp dword ptr _switch_jmp
    skip_shaders_switch:
    jmp dword ptr _switch_default
