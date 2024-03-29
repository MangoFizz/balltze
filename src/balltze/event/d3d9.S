;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _d3d9_begin_scene_before_event
_d3d9_begin_scene_before_event:
    pushad
    pushfd
    push dword ptr [esp + 0x28]
    call _dispatch_d3d9_begin_scene_before_event
    add esp, 4
    popfd
    popad
    ret

.globl _d3d9_begin_scene_after_event
_d3d9_begin_scene_after_event:
    pushad
    pushfd
    push dword ptr [esp + 0x28]
    call _dispatch_d3d9_begin_scene_after_event
    add esp, 4
    popfd
    popad
    ret

.globl _d3d9_end_scene_before_event
_d3d9_end_scene_before_event:
    pushad
    pushfd
    push dword ptr [esp + 0x28]
    call _dispatch_d3d9_end_scene_before_event
    add esp, 4
    popfd
    popad
    ret

.globl _d3d9_end_scene_after_event
_d3d9_end_scene_after_event:
    pushad
    pushfd
    push dword ptr [esp + 0x28]
    call _dispatch_d3d9_end_scene_after_event
    add esp, 4
    popfd
    popad
    ret

.globl _d3d9_device_reset_before_event
_d3d9_device_reset_before_event:
    pushfd
    pushad
    push dword ptr [esp + 0x28]
    push dword ptr [esp + 0x30]
    call _dispatch_d3d9_device_reset_before_event
    add esp, 8
    popad
    popfd
    ret

.globl _d3d9_device_reset_after_event
_d3d9_device_reset_after_event:
    pushfd
    pushad
    push dword ptr [esp + 0x28]
    push dword ptr [esp + 0x30]
    call _dispatch_d3d9_device_reset_after_event
    add esp, 8
    popad
    popfd
    ret
