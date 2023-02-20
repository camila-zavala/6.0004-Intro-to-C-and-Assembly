.section .text     
.align 2 
.globl bubblesort

bubblesort:
  # a0: first memory address of array (DO NOT MODIFY)
  # a1: length of array

   li a2, 1    
while_loop:
    beqz a2, end
    li a2, 0
    addi a1, a1, -1
    li a3, 0
    blt a3, a1, for_loop
    j while_loop
for_loop:
    slli a4, a3, 2
    add a5, a0, a4
    lw a6, 0(a5)
    lw a7, 4(a5)
    bgt a6, a7, if_loop
    addi a3, a3, 1
    blt a3, a1, for_loop
    j while_loop
if_loop: 
    sw a6, 4(a5)
    sw a7, 0(a5)
    addi sp, sp, -36
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    sw a3, 12(sp)
    sw a4, 16(sp)
    sw a5, 20(sp)
    sw a6, 24(sp)
    sw a7, 28(sp)
    sw ra, 32(sp)
    jal arrayViz
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    lw a3, 12(sp)
    lw a4, 16(sp)
    lw a5, 20(sp)
    lw a6, 24(sp)
    lw a7, 28(sp)
    lw ra, 32(sp)
    addi sp, sp, 36
    li a2, 1
    addi a3, a3, 1
    blt a3, a1, for_loop
    j while_loop
end: 
    ret
  