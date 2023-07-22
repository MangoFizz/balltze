;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _console_command_event
_console_command_event:
    pushfd
    pushad

    push edi
    call _dispatch_console_command_event_before
    pop edi
    
    test eax, eax
    jne console_command_event_exit
    
    mov eax, [esp + 0x28]
    push edi
    push eax
    call dword ptr [_console_command_function]
    add esp, 0x4
    pop edi
    
    push edi
    call _dispatch_console_command_event_after
    add esp, 4
    
    console_command_event_exit:    
    popad
    popfd
    ret
