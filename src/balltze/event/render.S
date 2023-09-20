;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _ui_render_event_before_dispatcher_asm
_ui_render_event_before_dispatcher_asm:
    push ebp
    call _ui_render_event_before_dispatcher
    add esp, 4
    ret
