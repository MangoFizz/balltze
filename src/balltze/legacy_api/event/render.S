;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _ui_render_event_before_dispatcher_asm
_ui_render_event_before_dispatcher_asm:
    push ebp
    call _ui_render_event_before_dispatcher
    add esp, 4
    ret

.globl _widget_background_render_event_before_dispatcher_asm
_widget_background_render_event_before_dispatcher_asm:
    mov eax, [esp + 0x28]
    mov ecx, [esp + 0x30]
    push ecx
    push eax
    call _widget_background_render_event_before_dispatcher
    add esp, 8
    ret

.globl _widget_background_render_event_after_dispatcher_asm
_widget_background_render_event_after_dispatcher_asm:
    mov eax, [esp + 0x28]
    mov ecx, [esp + 0x30]
    push ecx
    push eax
    call _widget_background_render_event_after_dispatcher
    add esp, 8
    ret

.globl _hud_element_bitmap_render_event_before_dispatcher_asm
_hud_element_bitmap_render_event_before_dispatcher_asm:
    mov eax, [esp + 0x28]
    push ecx
    push eax
    call _hud_element_bitmap_render_event_before_dispatcher
    add esp, 8
    ret

.globl _hud_element_bitmap_render_event_after_dispatcher_asm
_hud_element_bitmap_render_event_after_dispatcher_asm:
    mov eax, [esp + 0x28]
    push eax
    call _hud_element_bitmap_render_event_after_dispatcher
    add esp, 4
    ret
