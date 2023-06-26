;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _sound_playback_event
_sound_playback_event:
    pushad
    pushfd
    
    ;# Dispatch the listeners before the sound is played
    mov eax, [esp + 0x28]
    push eax
    call _dispatch_sound_playback_event_before
    add esp, 4
    test eax, eax
    jnz do_not_play_sound
    
    ;# This is the original function, but we need to call it with the same arguments
    mov eax, [esp + 0x40]
    push eax
    mov eax, [esp + 0x40]
    push eax
    mov eax, [esp + 0x40]
    push eax
    mov eax, [esp + 0x40]
    push eax
    mov eax, [esp + 0x40]
    push eax
    mov eax, [esp + 0x40]
    push eax
    mov eax, [esp + 0x40]
    push eax
    call dword ptr _original_play_sound_function
    add esp, 0x1C
    
    ;# Dispatch the listeners after the sound is played
    mov eax, [esp + 0x24]
    push eax
    call _dispatch_sound_playback_event_after
    add esp, 4

    do_not_play_sound:
    popfd
    popad
    ret
