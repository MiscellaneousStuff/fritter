; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
VIDINFO  equ  1 << 2            ; VESA graphics mode
FLAGS    equ  MBALIGN | MEMINFO | VIDINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

; Graphics Definitions
WIDTH equ 800 ; 640, 800, 1024
HEIGHT equ 600 ; 480, 600, 768
BPP equ 32

global mboot
;extern code
;extern bss
;extern end

section .multiboot
align 4
mboot:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
  dd mboot
    dd 0;code
    dd 0;bss
    dd 0;end
    dd _start
  dd 0 ; Set graphics mode
  dd WIDTH
  dd HEIGHT
  dd BPP
 
section .text
global _start:function (_start.end - _start)
_start:
  cli
	mov esp, stack_top
  push ebx ; Push pointer to multiboot info structure here
  xor ebp, ebp ; Null base pointer so stack traces can terminate here

	extern kmain
	call kmain

.hang:	hlt
	jmp .hang
.end:

section .bss
align 16
stack_bottom:
resb 32 * 1024 ; 32 KiB
stack_top: