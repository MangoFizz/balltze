;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data

get_next_sound_permutation_index_addr: .long 0
do_not_override_next_permutation_getter: .long 0

.text

get_next_sound_permutation_index_sig: .string "get_next_sound_permutation_function"

.globl _get_next_sound_permutation_index_hook
_get_next_sound_permutation_index_hook:
    cmp dword ptr get_next_sound_permutation_index_addr, 0
    jnz do_get_next_sound_permutation_index
    lea eax, byte ptr get_next_sound_permutation_index_sig
    push eax
    call _get_address_for_signature
    mov get_next_sound_permutation_index_addr, eax
    add esp, 4

    do_get_next_sound_permutation_index:
    cmp byte ptr do_not_override_next_permutation_getter, 1
    je do_not_override

    mov ebx, [esp + 0x4]
    push ebx
    call _pop_sound_permutation
    add esp, 4
    cmp eax, -1
    jne done

    do_not_override:
    mov ebx, [esp + 0x4]
    push ebx
    xor eax, eax
    call dword ptr get_next_sound_permutation_index_addr
    add esp, 4

    mov ebx, [esp + 0x4]
    push eax
    push ebx
    call _push_sound_permutation
    add esp, 4
    pop eax

    done:
    ret

.globl _sound_playback_event
_sound_playback_event:
    pushad
    pushfd

    ;# Get the sound tag address 
    mov eax, [esp + 0x28]
    push eax
    call _get_tag_address_from_id
    add esp, 4

    ;# Get the sound permutation index
    mov eax, [eax + 0x14]
    push eax
    mov byte ptr do_not_override_next_permutation_getter, 1
    call _get_next_sound_permutation_index_hook
    mov byte ptr do_not_override_next_permutation_getter, 0
    pop ecx
    cmp eax, -1
    jz do_not_play_sound

    ;# Dispatch the listeners before the sound is played
    mov ebx, [esp + 0x28]
    push eax
    push ecx
    push ebx
    call _dispatch_sound_playback_event_before
    test eax, eax
    jnz do_not_play_sound

    ;# This is the original function, but we need to call it with the same arguments
    mov eax, [esp + 0x4C]
    push eax
    mov eax, [esp + 0x4C]
    push eax
    mov eax, [esp + 0x4C]
    push eax
    mov eax, [esp + 0x4C]
    push eax
    mov eax, [esp + 0x4C]
    push eax
    mov eax, [esp + 0x4C]
    push eax
    mov eax, [esp + 0x4C]
    push eax
    call dword ptr _original_play_sound_function
    add esp, 0x1C

    ;# Dispatch the listeners after the sound is played
    call _dispatch_sound_playback_event_after

    do_not_play_sound:
    add esp, 0xC
    popfd
    popad
    ret
