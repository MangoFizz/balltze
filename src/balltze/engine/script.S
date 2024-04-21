;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data
unit_scripting_enter_vehicle_function_addr: .long 0
unit_scripting_exit_vehicle_function_addr: .long 0
get_numeric_countdown_timer_function_addr: .long 0

.text
unit_scripting_enter_vehicle_function_sig: .string "unit_enter_vehicle_function"
unit_scripting_exit_vehicle_function_sig: .string "unit_exit_vehicle_function"
get_numeric_countdown_timer_function_sig: .string "get_numeric_countdown_timer_function"

.globl _unit_scripting_enter_vehicle_asm
_unit_scripting_enter_vehicle_asm:
    cmp dword ptr unit_scripting_enter_vehicle_function_addr, 0
    jnz do_unit_scripting_enter_vehicle
    lea eax, byte ptr unit_scripting_enter_vehicle_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov unit_scripting_enter_vehicle_function_addr, eax

    do_unit_scripting_enter_vehicle:

    ;# Push unit handle
    push dword ptr [esp+0xC]
    ;# Push vehicle handle
    push dword ptr [esp+0xC]
    ;# Push seat name
    push dword ptr [esp+0xC]

    call dword ptr unit_scripting_enter_vehicle_function_addr

    add esp, 0xC

    ret

.globl _unit_scripting_exit_vehicle_asm
_unit_scripting_exit_vehicle_asm:
    cmp dword ptr unit_scripting_exit_vehicle_function_addr, 0
    jnz do_unit_scripting_exit_vehicle
    lea eax, byte ptr unit_scripting_exit_vehicle_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov unit_scripting_exit_vehicle_function_addr, eax

    do_unit_scripting_exit_vehicle:

    ;# Mov unit handle
    mov eax, [esp+0x4]

    call dword ptr unit_scripting_exit_vehicle_function_addr

    ret

.globl _script_get_numeric_countdown_timer
_script_get_numeric_countdown_timer:
    cmp dword ptr get_numeric_countdown_timer_function_addr, 0
    jnz get_numeric_countdown_timer_function_found
    pushad
    pushfd
    lea eax, byte ptr get_numeric_countdown_timer_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov get_numeric_countdown_timer_function_addr, eax
    popfd
    popad
    get_numeric_countdown_timer_function_found:

    mov eax, [esp + 4]
    jmp dword ptr get_numeric_countdown_timer_function_addr
