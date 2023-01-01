;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _hold_for_action_hud_message_before_event_button_name
_hold_for_action_hud_message_before_event_button_name:
    mov ebx, [esp + 0x28]
    mov ecx, dword ptr [esp + 0xB0]
    shr ecx, 0x10
    push ecx
    push 3 ;# 3 = button name
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_before_event
    add esp, 0x10
    ret

.globl _hold_for_action_hud_message_after_event_button_name
_hold_for_action_hud_message_after_event_button_name:
    lea eax, [esp + 0x4C]
    mov ebx, [esp + 0x28]
    movsx ecx, word ptr [esp + 0xB0]
    push ecx
    push 3 ;# 3 = button name
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_after_event
    add esp, 0x10
    ret

.globl _hold_for_action_hud_message_before_event_button_name_left_quote
_hold_for_action_hud_message_before_event_button_name_left_quote:
    mov ebx, [esp + 0x28]
    push -1
    push 1 ;# 1 = left quote
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_before_event
    add esp, 0x10
    ret

.globl _hold_for_action_hud_message_after_event_button_name_left_quote
_hold_for_action_hud_message_after_event_button_name_left_quote:
    lea eax, [esp + 0x4C]
    mov ebx, [esp + 0x28]
    push -1
    push 1 ;# 1 = left quote
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_after_event
    add esp, 0x10
    ret

.globl _hold_for_action_hud_message_before_event_button_name_right_quote
_hold_for_action_hud_message_before_event_button_name_right_quote:
    mov ebx, [esp + 0x28]
    push -1
    push 2 ;# 2 = right quote
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_before_event
    add esp, 0x10
    ret

.globl _hold_for_action_hud_message_after_event_button_name_right_quote
_hold_for_action_hud_message_after_event_button_name_right_quote:
    lea eax, [esp + 0x4C]
    mov ebx, [esp + 0x28]
    push -1
    push 2 ;# 2 = right quote
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_after_event
    add esp, 0x10
    ret
