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
    call _dispatch_map_load_event_after

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

.globl _set_d3d9_device_multithreaded_flag
_set_d3d9_device_multithreaded_flag:
    or edi, 0x4 ;# enable multithreading 
    ret
