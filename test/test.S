 
.code16
.globl _start

_start:
    mov $16, %al
    cli
    hlt
