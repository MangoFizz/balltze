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

;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _netgame_sound_override
_netgame_sound_override:
    pushad
    pushfd

    push esi
    call _dispatch_network_game_sound_event_before
    add esp, 4

    test eax, eax
    jz do_netgame_sound

    popfd
    popad
    ret

    do_netgame_sound:
    push esi
    call _dispatch_network_game_sound_event_after
    add esp, 4

    popfd
    popad
    jmp dword ptr _netgame_sound_fn_return

.globl _network_game_multiplayer_hud_message_event_before_asm
_network_game_multiplayer_hud_message_event_before_asm:
    pushfd
    pushad
    push edi ;# local player
    push ecx ;# victim
    push eax ;# killer
    push edx ;# event type
    call _dispatch_network_game_multiplayer_hud_message_before
    add esp, 16
    popad
    popfd
    ret

.globl _network_game_multiplayer_hud_message_event_after_asm
_network_game_multiplayer_hud_message_event_after_asm:
    pushfd
    pushad
    push edi ;# local player
    push ecx ;# victim
    push eax ;# killer
    push edx ;# event type
    call _dispatch_network_game_multiplayer_hud_message_after
    add esp, 16
    popad
    popfd
    ret
