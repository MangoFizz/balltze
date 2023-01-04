;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _hold_for_action_hud_message_before_event_button_name
_hold_for_action_hud_message_before_event_button_name:
    lea ebx, [esp + 0x28]
    lea ecx, [esp + 0xAC]
    mov esi, dword ptr [esp + 0x6C]
    push ecx
    push esi
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_before_event_button_slice
    add esp, 0x10
    ret

.globl _hold_for_action_hud_message_after_event_button_name
_hold_for_action_hud_message_after_event_button_name:
    lea eax, [esp + 0x4C]
    lea ebx, [esp + 0x28]
    lea ecx, [esp + 0xAC]
    mov esi, dword ptr [esp + 0x6C]
    push ecx
    push esi
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_after_event_button_slice
    add esp, 0x10
    ret

.globl _hold_for_action_hud_message_before_event_button_name_left_quote
_hold_for_action_hud_message_before_event_button_name_left_quote:
    lea ebx, [esp + 0x28]
    mov esi, dword ptr [esp + 0x64]
    push 1 ;# 1 = left quote
    push esi
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_before_event
    add esp, 0x10
    ret

.globl _hold_for_action_hud_message_after_event_button_name_left_quote
_hold_for_action_hud_message_after_event_button_name_left_quote:
    lea eax, [esp + 0x4C]
    lea ebx, [esp + 0x28]
    mov esi, dword ptr [esp + 0x64]
    push 1 ;# 1 = left quote
    push esi
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_after_event
    add esp, 0x10
    ret

.globl _hold_for_action_hud_message_before_event_button_name_right_quote
_hold_for_action_hud_message_before_event_button_name_right_quote:
    lea ebx, [esp + 0x28]
    mov esi, dword ptr [esp + 0x74]
    push 2 ;# 2 = right quote
    push esi
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_before_event
    add esp, 0x10
    ret

.globl _hold_for_action_hud_message_after_event_button_name_right_quote
_hold_for_action_hud_message_after_event_button_name_right_quote:
    lea eax, [esp + 0x4C]
    lea ebx, [esp + 0x28]
    mov esi, dword ptr [esp + 0x74]
    push 2 ;# 2 = right quote
    push esi
    push eax
    push ebx
    call _dispatch_hold_for_action_hud_message_after_event
    add esp, 0x10
    ret
