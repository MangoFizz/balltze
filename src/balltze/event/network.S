;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _packet_received_before_event
_packet_received_before_event:
    lea eax, [esp + 0x28] ;# socket
    lea ebx, [esp + 0x2C] ;# buffer
    lea edx, [esp + 0x38] ;# from
    lea edi, [esp + 0x3C] ;# fromlen
    push edi
    push edx
    push ebx
    push eax
    call _packet_received_event_before_dispatcher
    add esp, 0x10
    ret

.globl _packet_received_after_event
_packet_received_after_event:
    mov ecx, eax ;# bytes read
    lea eax, [esp + 0x28] ;# socket
    lea ebx, [esp + 0x2C] ;# buffer
    lea edx, [esp + 0x38] ;# from
    lea edi, [esp + 0x3C] ;# fromlen
    push edi
    push edx
    push ecx
    push ebx
    push eax
    call _packet_received_event_after_dispatcher
    add esp, 0x14
    ret
