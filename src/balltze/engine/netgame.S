;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data
network_game_encode_message_function_addr: .long 0
network_game_decode_message_function_addr: .long 0
network_game_data_pointer_addr: .long 0
network_game_server_send_message_to_all_machines_function_addr: .long 0
network_game_server_send_message_to_machine_function_addr: .long 0
network_game_client_send_chat_message_function_addr: .long 0
network_game_client_send_message_function_addr: .long 0
network_game_client_unknown_function_1_addr: .long 0

.text
network_game_encode_message_function_sig: .string "network_game_encode_message_function"
network_game_decode_message_function_sig: .string "network_game_decode_message_function"
network_game_data_pointer_sig: .string "network_game_data_pointer"
network_game_server_send_message_to_all_machines_function_sig: .string "network_game_server_send_message_to_all_machines_function"
network_game_server_send_message_to_machine_function_sig: .string "network_game_server_send_message_to_machine_function"
network_game_client_send_chat_message_function_sig: .string "network_game_client_send_chat_message_function"
network_game_client_send_message_function_sig: .string "network_game_client_send_message_function"
network_game_client_unknown_function_1_sig: .string "network_game_client_unknown_function_1"

find_server_info_sig:
    cmp dword ptr network_game_data_pointer_addr, 0
    jnz server_info_sig_found
    lea eax, byte ptr network_game_data_pointer_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov network_game_data_pointer_addr, eax
    server_info_sig_found:
    ret

.globl _network_game_encode_message_asm
_network_game_encode_message_asm:
    cmp dword ptr network_game_encode_message_function_addr, 0
    jnz do_encode_packet
    lea eax, byte ptr network_game_encode_message_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov network_game_encode_message_function_addr, eax

    do_encode_packet:
    push ebp
    mov ebp, esp
    add ebp, 4

    mov eax, [ebp + 0x20] ;# arg7
    push eax

    mov eax, [ebp + 0x1C] ;# arg6
    push eax

    mov eax, [ebp + 0x18] ;# arg5
    push eax

    mov eax, [ebp + 0x14] ;# ptr_to_ptr
    push eax

    mov eax, [ebp + 0x10] ;# arg3
    push eax

    mov eax, [ebp + 0xC] ;# type_int
    push eax 

    mov eax, [ebp + 0x8] ;# arg1
    push eax

    mov eax, [ebp + 0x4] ;# output_buffer
    
    mov edx, 0x7FF8
    call dword ptr network_game_encode_message_function_addr
    add esp, 0x1C
    
    pop ebp
    ret

.globl _network_game_decode_message_asm
_network_game_decode_message_asm:
    cmp dword ptr network_game_decode_message_function_addr, 0
    jnz do_decode_packet
    lea eax, byte ptr network_game_decode_message_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov network_game_decode_message_function_addr, eax

    do_decode_packet:
    push ebp
    mov ebp, esp
    add ebp, 4

    mov ecx, [ebp + 0x4] ;# destination
    mov eax, [ebp + 0x8] ;# message_header
    call dword ptr network_game_decode_message_function_addr

    pop ebp
    ret

.globl _network_game_server_send_message_to_all_machines_asm
_network_game_server_send_message_to_all_machines_asm:
    call find_server_info_sig

    cmp dword ptr network_game_server_send_message_to_all_machines_function_addr, 0
    jnz do_send_packet_to_all
    lea eax, byte ptr network_game_server_send_message_to_all_machines_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov network_game_server_send_message_to_all_machines_function_addr, eax

    do_send_packet_to_all:
    push ebp
    mov ebp, esp
    add ebp, 4

    push ecx
    push eax

    mov eax, [ebp + 0x1C] ;# buffer_priority
    push eax

    mov eax, [ebp + 0x18] ;# unbuffered
    push eax

    mov eax, [ebp + 0x14] ;# flush_queue
    push eax

    mov eax, [ebp + 0x10] ;# write_to_local_connection
    push eax

    mov eax, [ebp + 0x4] ;# message
    push eax

    mov eax, [ebp + 0xC] ;# ingame_only
    push eax

    mov eax, [ebp + 0x8] ;# message_size

    mov ecx, dword ptr [network_game_data_pointer_addr] ;# network_game_data_pointer
    mov ecx, [ecx]
    mov ecx, [ecx]

    cmp ecx, 0
    jz skip_send_message_to_all

    call dword ptr network_game_server_send_message_to_all_machines_function_addr

    skip_send_message_to_all:
    add esp, 0x18

    pop eax
    pop ecx
    pop ebp
    ret

.globl _network_game_server_send_message_to_machine_asm
_network_game_server_send_message_to_machine_asm:
    call find_server_info_sig

    cmp dword ptr network_game_server_send_message_to_machine_function_addr, 0
    jnz do_send_packet_to_player
    lea eax, byte ptr network_game_server_send_message_to_machine_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov network_game_server_send_message_to_machine_function_addr, eax

    do_send_packet_to_player:
    push ebp
    mov ebp, esp
    add ebp, 4

    push esi
    push eax

    mov eax, [ebp + 0x1C] ;# buffer_priority
    push eax

    mov eax, [ebp + 0x18] ;# unbuffered
    push eax

    mov eax, [ebp + 0x14] ;# flush_queue
    push eax

    mov eax, [ebp + 0x10] ;# write_to_local_connection
    push eax

    mov eax, [ebp + 0x8] ;# message_size
    push eax

    mov eax, [ebp + 0x4] ;# message
    push eax

    mov eax, [ebp + 0xC] ;# ingame_only
    push eax

    mov eax, [ebp + 0x20] ;# machine_id

    mov esi, dword ptr [network_game_data_pointer_addr] ;# network_game_data_pointer
    mov esi, [esi]
    mov esi, [esi]

    cmp ecx, 0
    jz skip_send_message_to_player

    call dword ptr network_game_server_send_message_to_machine_function_addr

    skip_send_message_to_player:
    add esp, 0x1C

    pop eax
    pop esi

    pop ebp
    ret

.globl _network_game_client_send_chat_message_asm
_network_game_client_send_chat_message_asm:
    pushfd
    pushad

    ;# If we don't have the address, get it
    cmp dword ptr network_game_client_send_chat_message_function_addr, 0
    jnz do_send_chat
    lea eax, byte ptr network_game_client_send_chat_message_function_sig
    push eax
    call _get_address_for_signature
    mov network_game_client_send_chat_message_function_addr, eax
    pop eax

    ;# Prepare our message
    do_send_chat:
    mov eax, [esp + 0x28]
    mov edx, [esp + 0x2C]
    mov esi, [esp + 0x2C]
    push [esp + 0x2C]
    sub esp, 4

    pushfd
    pushad
    call _chat_get_local_rcon_id
    mov [esp + 0x24], eax
    popad
    popfd

    ;# Send the message
    call dword ptr network_game_client_send_chat_message_function_addr
    add esp, 8

    popad
    popfd
    ret

.globl _network_game_client_send_message_asm
_network_game_client_send_message_asm:
    call find_server_info_sig

    cmp dword ptr network_game_client_send_message_function_addr, 0
    jnz find_network_game_client_unknown_function_1_sig
    lea eax, byte ptr network_game_client_send_message_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov network_game_client_send_message_function_addr, eax

    find_network_game_client_unknown_function_1_sig:
    cmp dword ptr network_game_client_unknown_function_1_addr, 0
    jnz do_send_client_packet
    lea eax, byte ptr network_game_client_unknown_function_1_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov network_game_client_unknown_function_1_addr, eax

    do_send_client_packet:
    push ebp
    mov ebp, esp
    add ebp, 4
    
    push esi
    push edx
    push edi

    ;# Get client network data address
    mov edx, dword ptr [network_game_data_pointer_addr] 
    mov edx, [edx]
    add edx, 4
    mov edx, [edx]

    ;# not sure what is this
    mov edi, dword ptr [edx + 0xADC]

    ;# despite the wrong name of the function, it seems to be a validation
    ;# this function is called too when sending messages from the server
    lea esi, [edi + 0x10]
    push 1
    push edi
    call dword ptr network_game_client_send_message_function_addr
    add esp, 8

    ;# if the thing returned 0, we should not send the message
    cmp eax, 0
    jz skip_send_client_packet

    ;# bump up this thing
    mov edx, [esp + 0x4]
    lea edx, [edx + 1]
    add dword ptr [edi + 0xA80], edx

    ;# Get local rcon id
    sub esp, 4
    pushfd
    pushad
    call _chat_get_local_rcon_id
    mov [esp + 0x24], eax
    popad
    popfd

    ;# flush the message queue (?) 
    push 1
    lea ecx, [esp] ;# load rcon id
    mov eax, esi
    call dword ptr network_game_client_unknown_function_1_addr
    
    ;# this seems to push the message to the queue
    mov edx, [ebp + 0x8] ;# buffer size
    push edx
    mov ecx, [ebp + 0x4] ;# buffer pointer
    mov eax, esi
    mov byte ptr [esi + 0x1C], 0
    call dword ptr network_game_client_unknown_function_1_addr

    add esp, 8
    mov byte ptr [esi + 0x1C], 0

    add esp, 4 ;# pop rcon id

    skip_send_client_packet:
    pop edi
    pop edx
    pop esi
    pop ebp

    ret
