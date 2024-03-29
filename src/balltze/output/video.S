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

.globl _bik_open
_bik_open:
    push [esp + 0x8]
    push [esp + 0x8]
    call dword ptr [_bik_open_addr]
    ret

.globl _bik_pause
_bik_pause:
    push [esp + 0x8]
    push [esp + 0x8]
    call dword ptr [_bik_pause_addr]
    ret

.globl _bik_next_frame
_bik_next_frame:
    push [esp + 0x4]
    call dword ptr [_bik_next_frame_addr]
    ret

.globl _bik_do_frame
_bik_do_frame:
    push [esp + 0x4]
    call dword ptr [_bik_do_frame_addr]
    ret

.globl _bik_copy_to_buffer
_bik_copy_to_buffer:
    push [esp + 0x1C]
    push [esp + 0x1C]
    push [esp + 0x1C]
    push [esp + 0x1C]
    push [esp + 0x1C]
    push [esp + 0x1C]
    push [esp + 0x1C]
    call dword ptr [_bik_copy_to_buffer_addr]
    ret

.globl _bik_wait
_bik_wait:
    push [esp + 0x4]
    call dword ptr [_bik_wait_addr]
    ret

.globl _bik_close
_bik_close:
    push [esp + 0x4]
    call dword ptr [_bik_close_addr]
    ret

.globl _bik_go_to
_bik_go_to:
    push [esp + 0xC]
    push [esp + 0xC]
    push [esp + 0xC]
    call dword ptr [_bik_go_to_addr]
    ret

.globl _bik_set_volume
_bik_set_volume:
    push [esp + 0xC]
    push [esp + 0xC]
    push [esp + 0xC]
    call dword ptr [_bik_set_volume_addr]
    ret
