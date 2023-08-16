;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _load_map_override_asm
_load_map_override_asm:
    mov eax, [esp + 0x4]
    call dword ptr [_load_map_function_address]
    mov _load_map_function_result, eax
    call _set_map_load_thread_done_flag
    ret
