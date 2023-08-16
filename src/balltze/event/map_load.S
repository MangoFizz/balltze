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
