;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

apply_damage_function_override: ;# workaround for Chimera's hook
    sub esp, 0x94
    jmp dword ptr _object_damage_function_address

.globl _object_damage_event
_object_damage_event:
    add esp, 0x94
    pushfd
    pushad

    push dword ptr [esp + 0x28]
    lea eax, dword ptr [esp + 0x30]
    push eax
    call _dispatch_object_damage_event_before
    add esp, 8

    test eax, eax
    jne object_damage_event_exit

    push dword ptr [esp + 0x3C]
    push dword ptr [esp + 0x3C]
    push dword ptr [esp + 0x3C]
    push dword ptr [esp + 0x3C]
    push dword ptr [esp + 0x3C]
    push dword ptr [esp + 0x3C]
    call apply_damage_function_override
    add esp, 0x18

    push dword ptr [esp + 0x28]
    lea eax, dword ptr [esp + 0x30]
    push eax
    call _dispatch_object_damage_event_after
    add esp, 8

    object_damage_event_exit:
    popad
    popfd
    ret
