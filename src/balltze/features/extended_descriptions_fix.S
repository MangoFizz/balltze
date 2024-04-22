;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _multiplayer_screen_extended_screen_pic_index_1_fix
_multiplayer_screen_extended_screen_pic_index_1_fix:
    mov word ptr [eax + 0x58], 1
    ret
