;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _load_map_override_asm
_load_map_override_asm:
    mov eax, [esp + 0x4]
    call dword ptr [_load_map_function_address]
    mov _load_map_function_result, eax
    call _set_map_load_thread_done_flag
    ret

.globl _set_video_mode_asm
_set_video_mode_asm:
    push eax
    mov eax, [esp + 0x14]
    or eax, 0x4 ;# enable multithreading 
    mov [esp + 0x14], eax
    pop eax
    call dword ptr [ecx + 0x40]
    jmp dword ptr [_set_video_mode_return]
