;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _sound_playback_event
_sound_playback_event:
    pushfd
    pushad

    push edi
    call _dispatch_sound_playback_event_before
    add esp, 4
    
    test eax, eax
    jne sound_playback_event_exit
    
    mov eax, [esp + 0x30]
    push eax
    mov eax, [esp + 0x30]
    push eax
    mov eax, [esp + 0x30]
    push eax
    call dword ptr [_sound_playback_function]
    add esp, 0xC
    
    push edi
    call _dispatch_sound_playback_event_after
    add esp, 4
    
    sound_playback_event_exit:    
    popad
    popfd
    ret
