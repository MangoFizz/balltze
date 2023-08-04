;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data
create_object_query_fn: .long 0
create_object_fn: .long 0
delete_object_fn: .long 0
apply_damage_addr: .long 0
unit_enter_vehicle_function_addr: .long 0

.text
create_object_query_fn_sig: .string "create_object_query_function"
create_object_fn_sig: .string "create_object_function"
delete_object_fn_sig: .string "delete_object_function"
apply_damage_sig: .string "apply_damage_function"
unit_enter_vehicle_function_sig: .string "unit_enter_vehicle_function"

.globl _create_object_query_asm
_create_object_query_asm:
    cmp dword ptr create_object_query_fn, 0
    jnz do_create_query

    lea eax, byte ptr create_object_query_fn_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    sub eax, 6
    mov create_object_query_fn, eax

    do_create_query:

    ;# Push parent object handle
    mov eax, dword ptr [esp + 0x8]
    push eax

    ;# Push object tag id
    mov eax, dword ptr [esp + 0x8]
    push eax

    ;# Move query buffer to eax
    mov eax, dword ptr [esp + 0x14]

    call dword ptr create_object_query_fn
    add esp, 8

    ret

.globl _create_object_asm
_create_object_asm:
    cmp dword ptr create_object_fn, 0
    jnz do_create_object

    lea eax, byte ptr create_object_fn_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    sub eax, 24
    mov create_object_fn, eax

    do_create_object:

    ;# Push object type
    mov eax, dword ptr [esp + 0x8]
    push eax

    ;# Push query
    mov eax, dword ptr [esp + 0x8]
    push eax

    call dword ptr create_object_fn
    add esp, 8

    ret

.globl _delete_object_asm
_delete_object_asm:
    cmp dword ptr delete_object_fn, 0
    jnz do_delete_object
    lea eax, byte ptr delete_object_fn_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    sub eax, 10
    mov delete_object_fn, eax

    do_delete_object:
    mov eax, dword ptr [esp + 0x4]
    push eax
    call dword ptr delete_object_fn
    add esp, 4

    ret

.globl _apply_damage_asm
_apply_damage_asm:
    pushfd
    pushad

    cmp dword ptr apply_damage_addr, 0
    jnz do_apply_damage
    lea eax, byte ptr apply_damage_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov apply_damage_addr, eax

    do_apply_damage:

    ;# Pass some stuff
    push 0
    push 0xFFFFFFFF
    push 0xFFFFFFFF
    push 0xFFFFFFFF

    ;# Object handle
    push dword ptr [esp+0x3C]

    ;# Struct pointer
    push dword ptr [esp+0x3C]

    ;# Call it and hope we don't crash
    call dword ptr apply_damage_addr

    ;# Cleanup
    add esp, 0x18

    popad
    popfd

    ret

.globl _unit_enter_vehicle_asm
_unit_enter_vehicle_asm:
    cmp dword ptr unit_enter_vehicle_function_addr, 0
    jnz do_unit_enter_vehicle
    lea eax, byte ptr unit_enter_vehicle_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov unit_enter_vehicle_function_addr, eax

    do_unit_enter_vehicle:

    ;# Push unit handle
    push dword ptr [esp+0xC]
    ;# Push vehicle handle
    push dword ptr [esp+0xC]
    ;# Push seat name
    push dword ptr [esp+0xC]

    call dword ptr unit_enter_vehicle_function_addr

    add esp, 0xC

    ret
