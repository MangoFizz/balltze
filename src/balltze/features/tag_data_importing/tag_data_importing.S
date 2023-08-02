;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _map_loading_asm
_map_loading_asm:
    push dword ptr [esp+0x38]
    push dword ptr [esp+0x2C]
    call _dispatch_map_file_load_event_before
    add esp, 8
    ret

.globl _on_model_data_buffer_alloc_asm
_on_model_data_buffer_alloc_asm:
    lea eax, [esp + 0x2C]
    push eax
    call _on_model_data_buffer_alloc
    add esp, 4
    ret
