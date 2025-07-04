;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data
rasterizer_get_vertex_shader_permutation_index_function_addr: .long 0
rasterizer_get_transparent_geometry_group_vertex_type_function_addr: .long 0
rasterizer_render_transparent_geometry_group_function_addr: .long 0
rasterizer_render_transparent_geometry_group_vertices_function_addr: .long 0
rasterizer_set_framebuffer_blend_function_function_addr: .long 0
rasterizer_set_bitmap_data_texture_function_addr: .long 0
rasterizer_apply_shader_texture_animation_function_addr: .long 0
rasterizer_prepare_shader_transparent_chicago_function_addr: .long 0

.text
rasterizer_get_vertex_shader_permutation_index_function_sig: .string "rasterizer_get_vertex_shader_permutation_index_function"
rasterizer_get_transparent_geometry_group_vertex_type_function_sig: .string "rasterizer_get_transparent_geometry_group_vertex_type_function"
rasterizer_render_transparent_geometry_group_function_sig: .string "rasterizer_render_transparent_geometry_group_function"
rasterizer_render_transparent_geometry_group_vertices_function_sig: .string "rasterizer_render_transparent_geometry_group_vertices_function"
rasterizer_set_framebuffer_blend_function_function_sig: .string "rasterizer_set_framebuffer_blend_function_function"
rasterizer_set_bitmap_data_texture_function_sig: .string "rasterizer_set_bitmap_data_texture_function"
rasterizer_apply_shader_texture_animation_function_sig: .string "rasterizer_apply_shader_texture_animation_function"
rasterizer_prepare_shader_transparent_chicago_function_sig: .string "rasterizer_prepare_shader_transparent_chicago_function"

.globl _rasterizer_get_vertex_shader_permutation_index
_rasterizer_get_vertex_shader_permutation_index:
    cmp dword ptr rasterizer_get_vertex_shader_permutation_index_function_addr, 0
    jnz rasterizer_get_vertex_shader_permutation_index_function_found
    pushad
    pushfd
    lea eax, byte ptr rasterizer_get_vertex_shader_permutation_index_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov rasterizer_get_vertex_shader_permutation_index_function_addr, eax
    popfd
    popad
    rasterizer_get_vertex_shader_permutation_index_function_found:
    mov ecx, [esp + 4]
    jmp dword ptr rasterizer_get_vertex_shader_permutation_index_function_addr

.globl _rasterizer_get_transparent_geometry_group_vertex_type
_rasterizer_get_transparent_geometry_group_vertex_type:
    cmp dword ptr rasterizer_get_transparent_geometry_group_vertex_type_function_addr, 0
    jnz rasterizer_get_transparent_geometry_group_vertex_type_function_found
    pushad
    pushfd
    lea eax, byte ptr rasterizer_get_transparent_geometry_group_vertex_type_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov rasterizer_get_transparent_geometry_group_vertex_type_function_addr, eax
    popfd
    popad
    rasterizer_get_transparent_geometry_group_vertex_type_function_found:
    mov ecx, [esp + 4]
    jmp dword ptr rasterizer_get_transparent_geometry_group_vertex_type_function_addr

.globl _rasterizer_render_transparent_geometry_group
_rasterizer_render_transparent_geometry_group:
    cmp dword ptr rasterizer_render_transparent_geometry_group_function_addr, 0
    jnz rasterizer_render_transparent_geometry_group_function_found
    pushad
    pushfd
    lea eax, byte ptr rasterizer_render_transparent_geometry_group_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov rasterizer_render_transparent_geometry_group_function_addr, eax
    popfd
    popad
    rasterizer_render_transparent_geometry_group_function_found:
    jmp dword ptr rasterizer_render_transparent_geometry_group_function_addr

.globl _rasterizer_render_transparent_geometry_group_vertices
_rasterizer_render_transparent_geometry_group_vertices:
    cmp dword ptr rasterizer_render_transparent_geometry_group_vertices_function_addr, 0
    jnz rasterizer_render_transparent_geometry_group_vertices_function_found
    pushad
    pushfd
    lea eax, byte ptr rasterizer_render_transparent_geometry_group_vertices_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov rasterizer_render_transparent_geometry_group_vertices_function_addr, eax
    popfd
    popad
    rasterizer_render_transparent_geometry_group_vertices_function_found:
    mov ecx, [esp + 8]
    jmp dword ptr rasterizer_render_transparent_geometry_group_vertices_function_addr

.globl _rasterizer_set_framebuffer_blend_function
_rasterizer_set_framebuffer_blend_function:
    cmp dword ptr rasterizer_set_framebuffer_blend_function_function_addr, 0
    jnz rasterizer_set_framebuffer_blend_function_function_found
    pushad
    pushfd
    lea eax, byte ptr rasterizer_set_framebuffer_blend_function_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov rasterizer_set_framebuffer_blend_function_function_addr, eax
    popfd
    popad
    rasterizer_set_framebuffer_blend_function_function_found:
    mov ecx, [esp + 4]
    jmp dword ptr rasterizer_set_framebuffer_blend_function_function_addr

.globl _rasterizer_set_bitmap_data_texture
_rasterizer_set_bitmap_data_texture:
    cmp dword ptr rasterizer_set_bitmap_data_texture_function_addr, 0
    jnz rasterizer_set_bitmap_data_texture_function_found
    pushad
    pushfd
    lea eax, byte ptr rasterizer_set_bitmap_data_texture_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov rasterizer_set_bitmap_data_texture_function_addr, eax
    popfd
    popad
    rasterizer_set_bitmap_data_texture_function_found:
    mov eax, [esp + 0x14]
    jmp dword ptr rasterizer_set_bitmap_data_texture_function_addr

.globl _rasterizer_apply_shader_texture_animation
_rasterizer_apply_shader_texture_animation:
    cmp dword ptr rasterizer_apply_shader_texture_animation_function_addr, 0
    jnz rasterizer_apply_shader_texture_animation_function_found
    pushad
    pushfd
    lea eax, byte ptr rasterizer_apply_shader_texture_animation_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov rasterizer_apply_shader_texture_animation_function_addr, eax
    popfd
    popad
    rasterizer_apply_shader_texture_animation_function_found:
    push ebp
    mov ebp, esp
    add ebp, 4
    pushad
    mov edi, [ebp + 0x28]
    mov ebx, [ebp + 0x24]
    mov ecx, [ebp + 0x20]
    push [ebp + 0x1C]
    push [ebp + 0x18]
    push [ebp + 0x14]
    push [ebp + 0x10]
    push [ebp + 0xC]
    push [ebp + 0x8]
    mov esi, [ebp + 0x4]
    call dword ptr rasterizer_apply_shader_texture_animation_function_addr
    add esp, 0x18
    popad
    pop ebp
    ret

.globl _rasterizer_prepare_shader_transparent_chicago
_rasterizer_prepare_shader_transparent_chicago:
    cmp dword ptr rasterizer_prepare_shader_transparent_chicago_function_addr, 0
    jnz rasterizer_prepare_shader_transparent_chicago_function_found
    pushad
    pushfd
    lea eax, byte ptr rasterizer_prepare_shader_transparent_chicago_function_sig
    push eax
    call _get_address_for_signature
    add esp, 4
    mov rasterizer_prepare_shader_transparent_chicago_function_addr, eax
    popfd
    popad
    rasterizer_prepare_shader_transparent_chicago_function_found:
    mov ebx, [esp + 4]
    jmp dword ptr rasterizer_prepare_shader_transparent_chicago_function_addr

.globl _load_bitmap_asm
_load_bitmap_asm:
    push eax
    mov eax, [esp + 0x8]
    push [esp + 0xC]
    push [esp + 0xC]
    call dword ptr _load_bitmap_function_address
    add esp, 8
    pop eax
    ret

.globl _render_user_interface_asm
_render_user_interface_asm:
    mov eax, [esp + 0x4]
    call dword ptr _render_user_interface_function_address
    ret
