;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _widget_open_event_before_asm
_widget_open_event_before_asm:
    push ebp
    mov ebp, esp
    add ebp, 4
    
    ;# Widgets tree upstream node
    push eax
    mov eax, [ebp + 0xC]

    ;# Save everything
    pushad
    pushfd

    ;# Check if we are on the root widget
    test eax, eax
    jz widget_open_event_before_is_root_widget
    push 0
    mov eax, 0
    jmp widget_open_event_before_call_events
    widget_open_event_before_is_root_widget:
    push eax
    mov eax, 1

    widget_open_event_before_call_events:
    push eax
    push dword ptr [ebp + 0x4] ;# definition tag path
    push dword ptr [ebp + 0x8] ;# definition tag handle
    call _dispatch_widget_open_before_event
    add esp, 0x10

    popfd
    test eax, eax ;# Check if event was cancelled
    popad
    pop eax
    pop ebp
    jnz widget_open_event_before_exit

    jmp dword ptr _widget_create_function_override_return

    widget_open_event_before_exit:
    mov eax, 0
    ret

.globl _widget_open_event_after_asm
_widget_open_event_after_asm:
    pushad
    pushfd

    ;# Check if the widget was created
    test eax, eax
    jz widget_open_event_after_exit
    push eax ;# save this for later
    
    push ebp
    mov ebp, esp
    add ebp, 4

    ;# Widgets tree upstream node
    push edx
    mov edx, [ebp + 0x24 + 0x28 + 0xC]
    
    ;# Check if we are on the root widget
    test edx, edx
    jz widget_open_event_after_is_root_widget
    push 0
    mov edx, 0
    jmp widget_open_event_after_call_events
    widget_open_event_after_is_root_widget:
    push edx
    mov edx, 1

    widget_open_event_after_call_events:
    push edx
    push eax ;# created widget
    call _dispatch_widget_open_after_event
    add esp, 0xC

    pop edx
    pop ebp 
    pop eax

    widget_open_event_after_exit:
    popfd
    popad
    ret

.globl _widget_back_event_asm
_widget_back_event_asm:
    pushfd
    pushad

    push eax ;# widget
    call _dispatch_widget_back_before_event

    test eax, eax
    jz widget_back_event_exit
    add esp, 4

    popad
    popfd
    ret

    widget_back_event_exit:
    call _dispatch_widget_back_after_event
    add esp, 4

    popad
    popfd

    jmp dword ptr _widget_back_function_override_return

.globl _widget_focus_event_asm
_widget_focus_event_asm:
    pushfd
    pushad

    push ecx
    call _dispatch_widget_focus_before_event
    pop ecx

    test eax, eax
    jnz widget_focus_event_exit

    push ecx
    call _dispatch_widget_focus_after_event
    pop ecx

    popad
    popfd

    jmp dword ptr _widget_focus_function_override_return

    widget_focus_event_exit:
    popad
    popfd
    ret

.globl _widget_mouse_focus_update_before_asm
_widget_mouse_focus_update_before_asm:
    push esi 
    call _dispatch_widget_focus_before_event
    add esp, 4
    ret

.globl _widget_mouse_focus_update_after_asm
_widget_mouse_focus_update_after_asm:
    push esi 
    call _dispatch_widget_focus_after_event
    add esp, 4
    ret

.globl _widget_accept_event_asm
_widget_accept_event_asm:
    pushad
    pushfd

    ;# Check enter key
    cmp al, 0
    jne widget_accept_event_exit

    ;# Check enter event
    cmp byte ptr [esi + 4], 0
    jne widget_accept_event_exit

    ;# Run events
    push dword ptr [esp + 0x48] ;# widget
    call _dispatch_widget_accept_before_event
    add esp, 4

    test eax, eax
    jz widget_accept_event_allow

    popfd
    ;# Overwrite cmp result
    cmp al, 0
    popad
    ret
    
    widget_accept_event_allow:
    push dword ptr [esp + 0x48] ;# widget
    call _dispatch_widget_accept_after_event
    add esp, 4

    ;# Get out, don't touch anything
    widget_accept_event_exit:
    popfd
    popad
    ret

.globl _widget_sound_event_asm
_widget_sound_event_asm:
    pushad
    pushfd

    ;# Decrement the meme
    dec eax

    ;# sound index
    push eax 

    ;# Catch weird calls
    cmp eax, 3
    ja widget_sound_event_exit
    cmp eax, 0
    jl widget_sound_event_exit

    call _dispatch_widget_sound_before_event

    ;# Check cancel flag
    test eax, eax
    jz widget_sound_event_allow
    add esp, 4

    popfd
    popad
    ret

    widget_sound_event_allow:
    call _dispatch_widget_sound_after_event

    widget_sound_event_exit:
    add esp, 4
    popfd
    popad
    jmp dword ptr _widget_sound_function_override_return

.globl _widget_tab_list_items_next_vertical_event_before_asm
_widget_tab_list_items_next_vertical_event_before_asm:
    push 0
    jmp widget_tab_list_event_before_asm

.globl _widget_tab_list_items_next_vertical_event_after_asm
_widget_tab_list_items_next_vertical_event_after_asm:
    push 0
    jmp widget_tab_list_event_after_asm

.globl _widget_tab_list_items_next_horizontal_event_before_asm
_widget_tab_list_items_next_horizontal_event_before_asm:
    push 1
    jmp widget_tab_list_event_before_asm

.globl _widget_tab_list_items_next_horizontal_event_after_asm
_widget_tab_list_items_next_horizontal_event_after_asm:
    push 1
    jmp widget_tab_list_event_after_asm

.globl _widget_tab_list_items_previous_vertical_event_before_asm
_widget_tab_list_items_previous_vertical_event_before_asm:
    push 2
    jmp widget_tab_list_event_before_asm

.globl _widget_tab_list_items_previous_vertical_event_after_asm
_widget_tab_list_items_previous_vertical_event_after_asm:
    push 2
    jmp widget_tab_list_event_after_asm

.globl _widget_tab_list_items_previous_horizontal_event_before_asm
_widget_tab_list_items_previous_horizontal_event_before_asm:
    push 3
    jmp widget_tab_list_event_before_asm

.globl _widget_tab_list_items_previous_horizontal_event_after_asm
_widget_tab_list_items_previous_horizontal_event_after_asm:
    push 3
    jmp widget_tab_list_event_after_asm

.globl _widget_tab_children_next_vertical_event_before_asm
_widget_tab_children_next_vertical_event_before_asm:
    push 4
    jmp widget_tab_list_event_before_asm

.globl _widget_tab_children_next_vertical_event_after_asm
_widget_tab_children_next_vertical_event_after_asm:
    push 4
    jmp widget_tab_list_event_after_asm

.globl _widget_tab_children_next_horizontal_event_before_asm
_widget_tab_children_next_horizontal_event_before_asm:
    push 5
    jmp widget_tab_list_event_before_asm

.globl _widget_tab_children_next_horizontal_event_after_asm
_widget_tab_children_next_horizontal_event_after_asm:
    push 5
    jmp widget_tab_list_event_after_asm

.globl _widget_tab_children_previous_event_before_asm
_widget_tab_children_previous_event_before_asm:
    push 6
    jmp widget_tab_list_event_before_asm

.globl _widget_tab_children_previous_event_after_asm
_widget_tab_children_previous_event_after_asm:
    push 6
    jmp widget_tab_list_event_after_asm

widget_tab_list_event_before_asm:
    push ebx
    call _dispatch_widget_list_tab_before_event
    add esp, 8
    ret

widget_tab_list_event_after_asm:
    push ebx
    call _dispatch_widget_list_tab_after_event
    add esp, 8
    ret
