;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.text

.globl _sound_playback_event_before
_sound_playback_event_before:
    push edi
    call _dispatch_sound_playback_event_before
    add esp, 4
    ret

.globl _sound_playback_event_after
_sound_playback_event_after:
    push edi
    call _dispatch_sound_playback_event_after
    add esp, 4
    ret
