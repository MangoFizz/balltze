;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _play_bik_video_hook
_play_bik_video_hook:
    cmp byte ptr _override_bik_video_res, 0
    jz skip_override
    push eax
    mov eax, _bik_video_width
    mov [esp + 0xC], eax
    mov eax, _bik_video_height
    mov [esp + 0x10], eax
    pop eax
    mov byte ptr _override_bik_video_res, 0
    skip_override:
    ret
