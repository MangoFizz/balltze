;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _on_model_data_buffer_alloc_asm
_on_model_data_buffer_alloc_asm:
    lea eax, [esp + 0x2C]
    push eax
    call _on_model_data_buffer_alloc
    add esp, 4
    ret
