 
.code16
.globl _start

test1:
    cli
    hlt 

_start:
    mov $0x10, %al
    jmp test1
