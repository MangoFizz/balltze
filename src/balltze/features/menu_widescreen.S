;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _menu_render_text_asm
_menu_render_text_asm:
    pushfd
    pushad

    push ecx
    call _widescreen_displace_menu_text
    add esp, 4

    popad
    popfd
    ret

.globl _widescreen_input_text
_widescreen_input_text:
    pushfd
    pushad
    mov dword ptr _widescreen_text_input_element, ecx
    call _reposition_menu_text_input
    popad
    popfd
    ret

.globl _widescreen_input_text_undo
_widescreen_input_text_undo:
    pushfd
    pushad
    call _unreposition_menu_text_input
    popad
    popfd
    ret

.globl _widescreen_mouse
_widescreen_mouse:
    ;# edx = x, ecx = y
    push edx

    ;# Y bounds is always 0-480
    mov edx, dword ptr [_widescreen_mouse_y]
    cmp ecx, 480
    jg y_too_high
    cmp ecx, 0
    jl y_too_low
    mov dword ptr [edx], ecx
    jmp y_done
    y_too_low:
    mov dword ptr [edx], 0
    jmp y_done
    y_too_high:
    mov dword ptr [edx], 480

    y_done:

    mov edx, dword ptr [_widescreen_mouse_x]
    push ebx
    push ecx
    mov ebx, dword ptr [_widescreen_mouse_left_bound]
    mov ecx, dword ptr [_widescreen_mouse_right_bound]

    cmp eax, ecx
    jg x_too_high
    cmp eax, ebx
    jl x_too_low
    mov dword ptr [edx], eax
    jmp x_done
    x_too_low:
    mov dword ptr [edx], ebx
    jmp x_done
    x_too_high:
    mov dword ptr [edx], ecx

    x_done:
    pop ecx
    pop ebx
    pop edx
    mov eax, 1 ;# do not execute original code
    ret
