;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _ui_widget_event_handler_dispatch_hook
_ui_widget_event_handler_dispatch_hook:
    push dword ptr [esp + 0x14]
    push dword ptr [esp + 0x14]
    push dword ptr [esp + 0x14]
    push dword ptr [esp + 0x14]
    push dword ptr [esp + 0x14]
    call _dispatch_widget_event
    add esp, 0x14
    cmp eax, 0
    jne cancel_event
    jmp dword ptr [_ui_widget_event_handler_dispatch_hook_return]
    cancel_event:
    ret
