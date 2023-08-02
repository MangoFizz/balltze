;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

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
