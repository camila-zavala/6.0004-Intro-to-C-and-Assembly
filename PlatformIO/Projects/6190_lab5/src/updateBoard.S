.section .text     
.align 2   
.globl updateBoard

# updateBoard
# ARGUMENTS a0: screen buffer (current board), a1: temporary output buffer (for new board)
# RETURNS: Nothing
updateBoard:
    
    # TO-DO: Translate the C implementation of updateBoard to assembly. 
    # Make sure to follow RISC-V calling convention!
    
    addi sp, sp, -32
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)
    sw s5, 24(sp)
    sw s6, 28(sp)
   
    mv s0, a0
    mv a0, a1
    call eraseBuffer
    li s1, 0
    li s2, 0
    mv s3, a0   # s3 = new board
    li s4, 32
   
compare1:
    li t0, 8
    blt s2, t0, compare2
    j end
   
compare2:
    blt s1, s4, loop2
    addi s2, s2, 1
    li s1, 0
    j compare1
   
loop2:
    mv a0, s0
    mv a1, s1
    mv a2, s2
    call checkNeighbors
    # int neighbor_tally = checkNeighbors(current_board,x,y); // Tally its living neighbors.
    mv s5, a0
    mv a0, s0
    mv a1, s1
    mv a2, s2
    call getPixel
    # int current_cell = getPixel(current_board,x,y); // Get the cell's current value.
    mv s6, a0
    li t1, 3
    bne t1, s5, elseif
    mv a0, s3
    mv a1, s1
    mv a2, s2
    li a3, 1
    call setPixel
    j else
    # setPixel(new_board,x,y,1); // ...the cell should become occupied.
elseif:
    beqz s6, else
    li t2, 2
    bne t2, s5, else
    # current_cell && neighbor_tally==2 then stay here if
    mv a0, s3
    mv a1, s1
    mv a2, s2
    li a3, 1
    call setPixel
else:
    addi s1, s1, 1
    j compare2
end:
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp)
    addi sp, sp, 32
    ret

# getPixel
# ARGUMENTS a0: screen_buffer, a1: x, a2: y
# RETURNS: 1 if cell occupied, 0 otherwise
getPixel:
    # your code here
    slli a2, a2, 2  # a2: 4 * y for address 
    add a2, a2, a0  # a2: screen_buffer + 4*y 
    lw a3, 0(a2)    # a3: screen_buffer[y]
    srl a3, a3, a1  # a3: screen_buffer[y] >>_l x
    andi a0, a3, 1  # a0: a3 & 1
    ret

# checkNeighbors
# ARGUMENTS a0: game_board, a1: x index, a2: y index
# RETURNS: total occupied cells in the eight surrounding cells of (x,y) (game board wraps in x and y)
checkNeighbors: 
    addi sp, sp, -4
    sw ra, 0(sp)
    addi a3, a1, 1 
    andi a3, a3, 31
    addi a4, a1, -1
    andi a4, a4, 31
    addi a5, a2, -1
    andi a5, a5, 7
    addi a6, a2, 1
    andi a6, a6, 7
tally: 
    call tallyNeighbors # tallyNeighbors(game_board, x, y, left, right, up, down)
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

# tallyNeighbors
# ARGUMENTS a0: game_board, a1: current x index, a2: current y index, a3: left neighbor index, 
# a4: right neighbor index, a5: up neighbor index, a6: down neighbor index 
# RETURNS: total occupied cells in the eight surrounding cells of current (x,y)
tallyNeighbors:
    # TODO: This procedure is functionally correct, but doesn't follow RISC-V calling convention.
    # Make this procedure follow calling convention. You may only add instructions that: 
    # 1. Increment/decrement the stack pointer
    # 2. Put elements on the stack
    # 3. Take elements off the stack.
    addi sp, sp, -36
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw a3, 20(sp)
    sw a4, 24(sp)
    sw a5, 28(sp)
    sw a6, 32(sp)
    
    mv s0, a0 # s0: game_board
    mv s1, a1 # a1: x
    mv s2, a2 # a2: y
    li s3, 0  # s3: tally
    

    mv a1, a4
    mv a2, a5
    call getPixel # getPixel(game_board, x-1, y-1)
    add s3, s3, a0
    
    lw a4, 24(sp)
    mv a0, s0
    mv a1, a4
    mv a2, s2
    call getPixel # getPixel(game_board, x-1, y)
    add s3, s3, a0

    lw a4, 24(sp)
    lw a6, 32(sp)
    mv a0, s0
    mv a1, a4
    mv a2, a6
    call getPixel # getPixel(game_board, x-1, y+1)
    add s3, s3, a0

    lw a5, 28(sp)
    mv a0, s0
    mv a1, s1
    mv a2, a5
    call getPixel # getPixel(game_board, x, y-1)
    add s3, s3, a0

    lw a6, 32(sp)
    mv a0, s0
    mv a1, s1
    mv a2, a6
    call getPixel # getPixel(game_board, x, y+1)
    add s3, s3, a0

    lw a3, 20(sp)
    lw a5, 28(sp)
    mv a0, s0
    mv a1, a3
    mv a2, a5
    call getPixel # getPixel(game_board, x+1, y-1)
    add s3, s3, a0

    lw a3, 20(sp)
    mv a0, s0
    mv a1, a3
    mv a2, s2
    call getPixel # getPixel(game_board, x+1, y)
    add s3, s3, a0

    
    lw a3, 20(sp)
    lw a6, 32(sp)
    mv a0, s0
    mv a1, a3
    mv a2, a6
    call getPixel # getPixel(game_board, x+1, y+1)
    add s3, s3, a0

    mv a0, s3
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    addi sp, sp, 36
    ret
   