;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data
last_rcon_message: .long 0
cancel_rcon_message: .long 0

.text

.globl _rcon_message_event_before_hook
_rcon_message_event_before_hook:
    mov [last_rcon_message], ecx
    push ecx
    call _dispatch_rcon_message_event_before
    add esp, 4
    mov [cancel_rcon_message], eax
    ret

.globl _rcon_message_event_cancellation_hook
_rcon_message_event_cancellation_hook:
    mov eax, [cancel_rcon_message]
    ret

.globl _rcon_message_event_after_hook
_rcon_message_event_after_hook:
    push [last_rcon_message]
    call _dispatch_rcon_message_event_after
    add esp, 4
    ret
