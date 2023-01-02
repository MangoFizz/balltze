;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _keyboard_input_before_event
_keyboard_input_before_event:
    push eax
    pushad
    pushfd
    lea ebx, [esp + 0x24]
    push edi
    push 1
    push ebx
    call _dispatch_input_event_before
    add esp, 0xC
    popfd
    popad
    pop eax
    ret

.globl _mouse_input_before_event
_mouse_input_before_event:
    push eax
    pushad
    pushfd
    lea ebx, [esp + 0x24]
    push ecx
    push 2
    push ebx
    call _dispatch_input_event_before
    add esp, 0xC
    popfd
    popad
    pop eax
    ret

.globl _gamepad_input_before_event
_gamepad_input_before_event:
    push ebp
    pushad
    pushfd
    lea ebx, [esp + 0x24]
    cmp ebp, 0x7FFF
    jne gamepad_input_before_event_mapped
    push 0
    jmp gamepad_input_before_event_call
    gamepad_input_before_event_mapped:
    push 1
    gamepad_input_before_event_call:
    push edx
    push 3
    call _dispatch_input_event_before
    add esp, 0xC
    cmp eax, 1
    jne gamepad_input_before_event_exit
    mov dword ptr [ebx], 0x7FFF
    gamepad_input_before_event_exit:
    popfd
    popad
    pop ebp
    mov eax, 0
    ret
