;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data

mouse_button: .long 0
menu_button_event: .long 0

.text

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

.globl _handle_widget_mouse_button_press_asm
_handle_widget_mouse_button_press_asm:
    ;# Save stuff
    pushad
    pushfd

    ;# Get pressed widget
    mov ebx, dword ptr [esp + 0x48]

    ;# Get button event
    mov ecx, eax
    mov menu_button_event, ecx

    ;# Get mouse pressed button
    mov edx, 0
    mov dl, byte ptr [edi + 4]
    mov mouse_button, edx
    
    ;# Run events
    push edx
    push ebx
    call _call_widget_mouse_button_press_events
    add esp, 8

    ;# Check if the mouse button matches the menu button event
    mov ebx, [mouse_button]
    mov ecx, [menu_button_event]
    cmp bl, cl
    jne block_event

    ;# Set event allow flag
    popfd
    cmp al, 1 
    popad
    ret

    ;# Mouse pressed button doesn't equal to menu button event. Block event.
    block_event:
    popfd
    cmp bl, -1
    popad
    ret
