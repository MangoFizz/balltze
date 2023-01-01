;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data
create_widget_function_address: .long 0
open_widget_function_address: .long 0
close_widget_function_address: .long 0
find_widget_function_address: .long 0
focus_widget_function_address: .long 0
get_widget_list_item_index_function_address: .long 0
free_widget_memory_function_address: .long 0

.text
create_widget_function_sig: .string "widget_create_function"
open_widget_function_sig: .string "widget_open_function"
close_widget_function_sig: .string "widget_close_function"
find_widget_function_sig: .string "widget_find_function"
focus_widget_function_sig: .string "widget_focus_function"
get_widget_list_item_index_function_sig: .string "widget_list_item_index_function"
free_widget_memory_function_sig: .string "widget_memory_release_function"

.globl _create_widget_asm
_create_widget_asm:
    ;# Get open widget function address
    cmp dword ptr create_widget_function_address, 0
    jnz do_create_widget
    lea eax, byte ptr create_widget_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov create_widget_function_address, eax

    do_create_widget:
    mov eax, [esp + 4] 
    mov ecx, [esp + 8] 

    ;# Null values
    push -1
    push -1
    push -1
    push 0

    ;# Widget instance to replace
    push ecx

    ;# Widget tag ID
    push eax

    ;# Widget tag path (null because we are using the tag ID parameter)
    push 0

    call dword ptr create_widget_function_address
    add esp, 28

    ret

.globl _open_widget_asm
_open_widget_asm:
    cmp dword ptr open_widget_function_address, 0
    jnz do_open_widget
    lea eax, byte ptr open_widget_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov open_widget_function_address, eax

    do_open_widget:
    mov eax, [esp + 4] ;# Current widget root instance
    mov edx, [esp + 8] ;# New widget tag ID
    push edx
    push eax
    call dword ptr open_widget_function_address
    add esp, 8

    ret

.globl _close_widget_asm
_close_widget_asm:
    cmp dword ptr close_widget_function_address, 0
    jnz do_back_to_previous_widget
    lea eax, byte ptr close_widget_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov close_widget_function_address, eax

    do_back_to_previous_widget:
    mov eax, [esp + 4] ;# Current root widget

    call dword ptr close_widget_function_address

    ret

.globl _find_widget_asm
_find_widget_asm:
    cmp dword ptr find_widget_function_address, 0
    jnz do_find_widget
    lea eax, byte ptr find_widget_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov find_widget_function_address, eax

    do_find_widget:
    mov eax, [esp + 4] ;# Widget where to search
    mov ecx, [esp + 8] ;# Tag ID of the widget definition

    push ecx
    push eax
    call dword ptr find_widget_function_address
    add esp, 8

    ret

.globl _focus_widget_asm
_focus_widget_asm:
    cmp dword ptr focus_widget_function_address, 0
    jnz do_focus_widget
    lea eax, byte ptr focus_widget_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov focus_widget_function_address, eax

    do_focus_widget:
    ;# Idk why, but the function reads it's arguments from EAX and ECX registers.
    mov eax, [esp + 4] ;# Root widget
    mov ecx, [esp + 8] ;# Widget to focus

    call dword ptr focus_widget_function_address

    ret

.globl _get_widget_list_item_index_asm
_get_widget_list_item_index_asm:
    cmp dword ptr get_widget_list_item_index_function_address, 0
    jnz do_get_widget_list_item_index
    lea eax, byte ptr get_widget_list_item_index_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov get_widget_list_item_index_function_address, eax

    do_get_widget_list_item_index:
    push esi
    mov esi, [esp + 8] ;# Widget 
    call dword ptr get_widget_list_item_index_function_address
    pop esi

    ret

.globl _free_widget_asm
_free_widget_asm:
    cmp dword ptr free_widget_memory_function_address, 0
    jnz do_free_widget
    lea eax, byte ptr free_widget_memory_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov free_widget_memory_function_address, eax

    do_free_widget:
    push [esp + 4] ;# Widget 
    call dword ptr free_widget_memory_function_address
    add esp, 4

    ret
