 
.code16
.globl _start

boot:

_start:
    mov $5, %al  # set
    push %ax     # stor
    call boot
