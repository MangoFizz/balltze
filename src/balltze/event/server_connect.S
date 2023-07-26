;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _server_connect_event_before
_server_connect_event_before:
    lea ecx, dword ptr [ecx + 0x2]
    push ecx
    lea eax, dword ptr [ebx + 0x12]
    push eax
    push ebx
    call _dispatch_server_connect_event_before
    add esp, 0xC
    ret ;# bool on eax

.globl _server_connect_event_after
_server_connect_event_after:
    lea ecx, dword ptr [ecx + 0x2]
    push ecx
    lea eax, dword ptr [ebx + 0x12]
    push eax
    push ebx
    call _dispatch_server_connect_event_after
    add esp, 0xC
    ret
