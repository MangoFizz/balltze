;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data
mdp_encode_stateless_iterated_function_addr: .long 0
mdp_decode_stateless_iterated_function_addr: .long 0
mdp_discard_iteration_body_function_addr: .long 0
socket_ready_ptr_addr: .long 0
net_send_message_to_all_function_addr: .long 0
net_send_message_to_player_function_addr: .long 0

.text
mdp_encode_stateless_iterated_function_sig: .string "mdp_encode_stateless_iterated_function"
mdp_decode_stateless_iterated_function_sig: .string "mdp_decode_stateless_iterated_function"
mdp_discard_iteration_body_function_sig: .string "mdp_discard_iteration_body_function"
socket_ready_ptr_sig: .string "net_send_message_socket_ready_function"
net_send_message_to_all_function_sig: .string "net_send_message_to_all_function"
net_send_message_to_player_function_sig: .string "net_send_message_to_player_function"

.globl _mdp_encode_stateless_iterated_asm
_mdp_encode_stateless_iterated_asm:
    cmp dword ptr mdp_encode_stateless_iterated_function_addr, 0
    jnz do_encode_packet
    lea eax, byte ptr mdp_encode_stateless_iterated_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov mdp_encode_stateless_iterated_function_addr, eax

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
    call dword ptr mdp_encode_stateless_iterated_function_addr
    add esp, 0x1C
    
    pop ebp
    ret

.globl _mdp_decode_stateless_iterated_asm
_mdp_decode_stateless_iterated_asm:
    cmp dword ptr mdp_decode_stateless_iterated_function_addr, 0
    jnz do_decode_packet
    lea eax, byte ptr mdp_decode_stateless_iterated_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov mdp_decode_stateless_iterated_function_addr, eax

    do_decode_packet:
    push ebp
    mov ebp, esp
    add ebp, 4

    mov ecx, [ebp + 0x4] ;# destination
    mov eax, [ebp + 0x8] ;# message_header
    call dword ptr mdp_decode_stateless_iterated_function_addr

    pop ebp
    ret

.globl _mdp_discard_iteration_body_asm
_mdp_discard_iteration_body_asm:
    cmp dword ptr mdp_discard_iteration_body_function_addr, 0
    jnz do_discard_iteration_body
    lea eax, byte ptr mdp_discard_iteration_body_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov mdp_discard_iteration_body_function_addr, eax

    do_discard_iteration_body:
    push ebp
    mov ebp, esp
    add ebp, 4

    pushad
    pushfd
    mov eax, [ebp + 0x4] ;# message_header
    call dword ptr mdp_discard_iteration_body_function_addr
    popfd
    popad

    pop ebp
    ret

.globl _net_send_message_to_all_asm
_net_send_message_to_all_asm:
    cmp dword ptr net_send_message_to_all_function_addr, 0
    jnz net_send_message_to_all_find_socket_ready_ptr_signature
    lea eax, byte ptr net_send_message_to_all_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov net_send_message_to_all_function_addr, eax

    net_send_message_to_all_find_socket_ready_ptr_signature:
    cmp dword ptr socket_ready_ptr_addr, 0
    jnz do_send_packet_to_all
    lea eax, byte ptr socket_ready_ptr_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov socket_ready_ptr_addr, eax

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

    mov ecx, dword ptr [socket_ready_ptr_addr] ;# socket_ready_ptr
    mov ecx, [ecx]
    mov ecx, [ecx]

    cmp ecx, 0
    jz skip_send_message_to_all

    call dword ptr net_send_message_to_all_function_addr

    skip_send_message_to_all:
    add esp, 0x18

    pop eax
    pop ecx
    pop ebp
    ret

.globl _net_send_message_to_player_asm
_net_send_message_to_player_asm:
    cmp dword ptr net_send_message_to_player_function_addr, 0
    jnz net_send_message_to_player_find_socket_ready_ptr_signature
    lea eax, byte ptr net_send_message_to_player_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov net_send_message_to_player_function_addr, eax

    net_send_message_to_player_find_socket_ready_ptr_signature:
    cmp dword ptr socket_ready_ptr_addr, 0
    jnz do_send_packet_to_player
    lea eax, byte ptr socket_ready_ptr_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov socket_ready_ptr_addr, eax

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

    mov eax, [ebp + 0x20] ;# player_id

    mov esi, dword ptr [socket_ready_ptr_addr] ;# socket_ready_ptr
    mov esi, [esi]
    mov esi, [esi]

    cmp ecx, 0
    jz skip_send_message_to_player

    call dword ptr net_send_message_to_player_function_addr

    skip_send_message_to_player:
    add esp, 0x1C

    pop eax
    pop esi

    pop ebp
    ret
