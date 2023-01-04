;# SPDX-License-Identifier: GPL-3.0-only

.intel_syntax noprefix

.data
console_print_addr: .long 0

.text
console_print_sig: .string "console_out"

;# Make sure anything with a % sign doesn't get wrecked
console_print_format: .string "%s"

.globl _console_print_asm
_console_print_asm:
    pushfd
    pushad

    ;# Check if we know the address
    cmp dword ptr console_print_addr, 0
    jnz do_console_print

    ;# Get the address if not set
    lea eax, byte ptr console_print_sig
    push eax
    call _get_address_for_signature
    mov console_print_addr, eax
    pop eax

    do_console_print:
    ;# Set the variables
    ;# eax = pointer to color
    mov eax, [esp+0x28]
    lea edx, console_print_format
    push [esp+0x2C] ;# message
    push edx
    call dword ptr console_print_addr
    add esp, 8

    ;# Clean up
    popad
    popfd
    ret
