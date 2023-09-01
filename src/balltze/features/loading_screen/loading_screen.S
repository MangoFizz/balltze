;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _load_map_worker_asm
_load_map_worker_asm:
    ;# Get map name
    mov eax, [esp + 0x4]

    ;# Dispatch callbacks before loading map
    push eax
    call _dispatch_map_load_event_before
    pop eax

    ;# Load map
    call dword ptr [_load_map_function_address]

    ;# Dispatch callbacks after loading map
    push [esp + 0x4]
    call _dispatch_map_load_event_after
    add esp, 0x4

    ;# Set map load thread done flag
    mov _load_map_function_result, eax
    call _set_map_load_thread_done_flag
    ret

.globl _load_map_override_asm
_load_map_override_asm:
    push eax
    call _load_map_override
    add esp, 0x4
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
