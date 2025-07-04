;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _console_command_event_before
_console_command_event_before:
    push edi
    call _dispatch_console_command_event_before
    add esp, 4
    ret

.globl _console_command_event_after
_console_command_event_after:
    push edi
    call _dispatch_console_command_event_after
    add esp, 4
    ret
