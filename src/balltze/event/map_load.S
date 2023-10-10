;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _map_load_before_event
_map_load_before_event:
    push eax
    call _dispatch_map_load_event_before
    add esp, 0x4
    ret

.globl _map_load_after_event
_map_load_after_event:
    call _dispatch_map_load_event_after
    ret

.globl _map_file_load_client_hook_asm
_map_file_load_client_hook_asm:
    push dword ptr [esp + 0x38]
    push dword ptr [esp + 0x2C]
    call _dispatch_map_file_load_event_before
    add esp, 8
    ret


.globl _map_file_load_server_hook_asm
_map_file_load_server_hook_asm:
    push dword ptr [esp + 0x28]
    push dword ptr [esp + 0x30]
    call _dispatch_map_file_load_event_before
    add esp, 8
    ret

.globl _map_file_data_read_event_before
_map_file_data_read_event_before:
    mov eax, [esp + 0x2C]
    mov ebp, [esp + 0x30]
    mov esi, [esp + 0x08]
    lea ebx, [esp + 0x14]
    push esi
    push ebx
    push ebp
    push eax
    call _dispatch_map_file_data_read_event_before
    add esp, 0x10
    ret

.globl _map_file_data_read_event_after
_map_file_data_read_event_after:
    mov eax, [esp + 0x2C]
    mov ebp, [esp + 0x30]
    mov esi, [esp + 0x08]
    push esi
    push ebx
    push ebp
    push eax
    call _dispatch_map_file_data_read_event_after
    add esp, 0x10
    ret
