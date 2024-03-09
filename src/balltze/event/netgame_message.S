;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _network_game_client_decode_hud_message_call_override
_network_game_client_decode_hud_message_call_override:
    pushad
    pushfd

    lea ebx, [esp + 0x30]

    push eax
    push ebx
    call _network_game_decode_message_asm
    pop ebx
    pop eax

    push ebx
    call _dispatch_network_game_chat_message_event_before
    add esp, 4
    
    cmp eax, 0
    jnz do_not_process_client_chat_message
    
    lea ebx, [esp + 0x30]
    push ebx
    call _dispatch_network_game_chat_message_event_after
    add esp, 4

    popfd
    popad
    jmp dword ptr _network_game_decode_hud_message_call_override_return
    
    do_not_process_client_chat_message:
    popfd
    popad
    ret

.globl _network_game_server_decode_hud_message_call_override
_network_game_server_decode_hud_message_call_override:
    pushad
    pushfd

    lea ebx, [esp + 0x40]

    push eax
    push ebx
    call _network_game_decode_message_asm
    pop ebx
    pop eax

    push ebx
    call _dispatch_network_game_chat_message_event_before
    add esp, 4
    
    cmp eax, 0
    jnz do_not_process_server_chat_message
    
    lea ebx, [esp + 0x40]
    push ebx
    call _dispatch_network_game_chat_message_event_after
    add esp, 4

    popfd
    popad
    jmp dword ptr _network_game_decode_hud_message_call_override_return
    
    do_not_process_server_chat_message:
    popfd
    popad
    ret
