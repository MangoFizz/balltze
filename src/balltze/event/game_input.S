;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _keyboard_input_event
_keyboard_input_event:
    push esi
    pushad
    pushfd
    lea ebx, [esp + 0x24]
    cmp ebp, 0x7FFF
    jne keyboard_input_event_mapped
    push 0
    jmp keyboard_input_event_call
    keyboard_input_event_mapped:
    push 1
    keyboard_input_event_call:
    push edx
    push 1
    call _dispatch_input_event_before
    cmp eax, 1
    jne keyboard_input_event_do_not_cancel
    mov dword ptr [ebx], 0x7FFF
    jmp keyboard_input_event_exit
    keyboard_input_event_do_not_cancel:
    call _dispatch_input_event_after
    keyboard_input_event_exit:
    add esp, 0xC
    popfd
    popad
    pop esi
    mov eax, 0
    ret

.globl _mouse_input_event
_mouse_input_event:
    push ebp
    pushad
    pushfd
    lea ebx, [esp + 0x24]
    cmp ebp, 0x7FFF
    jne mouse_input_event_mapped
    push 0
    jmp mouse_input_event_call
    mouse_input_event_mapped:
    push 1
    mouse_input_event_call:
    push edx
    push 2
    call _dispatch_input_event_before
    cmp eax, 1
    jne mouse_input_event_do_not_cancel
    mov dword ptr [ebx], 0x7FFF
    jmp mouse_input_event_exit
    mouse_input_event_do_not_cancel:
    call _dispatch_input_event_after
    mouse_input_event_exit:
    add esp, 0xC
    popfd
    popad
    pop ebp
    mov eax, 0
    ret

.globl _gamepad_input_event
_gamepad_input_event:
    push ebp
    pushad
    pushfd
    lea ebx, [esp + 0x24]
    cmp ebp, 0x7FFF
    jne gamepad_input_event_mapped
    push 0
    jmp gamepad_input_event_call
    gamepad_input_event_mapped:
    push 1
    gamepad_input_event_call:
    push edx
    push 3
    call _dispatch_input_event_before
    cmp eax, 1
    jne gamepad_input_event_do_not_cancel
    mov dword ptr [ebx], 0x7FFF
    jmp gamepad_input_event_exit
    gamepad_input_event_do_not_cancel:
    call _dispatch_input_event_after
    gamepad_input_event_exit:
    add esp, 0xC
    popfd
    popad
    pop ebp
    mov eax, 0
    ret
