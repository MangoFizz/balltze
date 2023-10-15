;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _unknown_command_sapp_hook_override
_unknown_command_sapp_hook_override:
    push eax
    xor eax, eax
    mov al, byte ptr [_restore_unknown_command_message_print]
    cmp al, 1
    pop eax
    je skip_sapp_hook
    jmp dword ptr [_unknown_command_sapp_hook_override_return]
    skip_sapp_hook:
    lea ecx, dword ptr [esp + 0x8]
    push ecx
    mov ecx, dword ptr [_unknown_command_sapp_hook_override_return]
    add ecx, 0x5
    jmp ecx
