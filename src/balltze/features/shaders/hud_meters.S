;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data

hud_interface_meter_beign_drawn: .long 0

.text

.globl _draw_hud_interface_meter_asm
_draw_hud_interface_meter_asm:
    push esi
    mov esi, [esp + 0x160]
    push esi
    push edi
    mov esi, dword ptr [hud_interface_meter_beign_drawn]
    push esi
    call _draw_hud_interface_meter
    add esp, 0xC
    pop esi
    jmp dword ptr _rasterizer_draw_screen_quad_bitmap_meter_return

.globl _save_hud_interface_meter_being_drawn
_save_hud_interface_meter_being_drawn:
    mov dword ptr [hud_interface_meter_beign_drawn], esi 
    ret
