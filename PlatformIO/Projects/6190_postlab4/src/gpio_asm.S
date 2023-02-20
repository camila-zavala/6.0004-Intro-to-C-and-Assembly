.section .text      # indicates a code segment
.align 2            # specifies how data should be arranged in memory
.globl pinSetup   # makes pinSetup able to be used outside of this file
.globl pinWrite     # makes pinWrite able to be used outside of this file
.globl pinRead      # makes pinRead able to be used outside of this file
.globl setPixel     # makes setPixel able to be used outside of this file
.globl eraseBuffer  # makes eraseBuffer able to be used outside of this file

# C declaration: int pinRead(int pin_num)
# ARGUMENTS a0: pin_num
# RETURNS bit read from GPIO pin 
pinRead:

    li a2, 0x6000403C   # GPIO_IN_ADDR
    lw a3, 0(a2)        # load the value stored at a2
    srl a4, a3, a0      # shift right logically by pin number
    andi a1, a4, 1      # And the result from the right shift with 1
    mv a0, a1
    ret 


# C declaration: void pinWrite(int pin_num, int value)
# ARGUMENTS a0: pin_num, a1: value
# RETURNS: Nothing
pinWrite:
    li a2, 0x60004004   # GPIO_OUT_ADDR
    lw a3, 0(a2)
    li a4, 1
    beq a1, a4, if_loop
    sll a5, a4, a0 
    not a6, a5
    and a3, a3, a6
    j end
if_loop:
    sll a5, a1, a0
    or a3, a3, a5
    j end
end:
    sw a3, 0(a2)
    ret




# C declaration: void pinSetup(int pin_num, int mode)
# ARGUMENTS a0: pin_num, a1: mode
# RETURNS: Nothing
pinSetup:
    li a2, 0x60009004   # IO_MUX_GPIOn_ADDR
    li a3, 0x60004020   # GPIO_ENABLE_ADDR
    beqz a1, if_loop1
    lw a4, 0(a3)
    sll a7, a1, a0
    or a4, a4, a7
    sw a4, 0(a3)
    ret
if_loop1: 
    li a5, 1
    slli a0, a0, 2
    add a2, a2, a0
    lw a4, 0(a2)
    slli a7, a5, 8
    or a4, a4, a7
    slli a7, a5, 9
    or a4, a4, a7
    sw a4, 0(a2)
    ret

# C declaration: void setPixel(uint32_t* screen_buffer_addr, uint8_t x, uint8_t y, uint8_t val)
# ARGUMENTS a0: screen buffer starting address, a1: x, a2: y, a3: val
# RETURNS: Nothing
setPixel:

    li a4, 1
    beq a3, a4, if_loop2
    slli a5, a2, 2
    add a6, a0, a5
    lw a7, 0(a6)
    sll a3, a4, a1
    not a4, a3
    and a7, a7, a4
    sw a7, 0(a6)
    ret
if_loop2:
    slli a5, a2, 2
    add a6, a0, a5
    lw a7, 0(a6)
    sll a3, a3, a1
    or a7, a7, a3
    sw a7, 0(a6) 
    ret 


# C declaration: void eraseBuffer(uint32_t* screen_buffer_addr)
# ARGUMENTS a0: screen_buffer starting address
# RETURNS: Nothing
eraseBuffer:
    li a1, 8            # upper bound on for loop
    li a2, 0            # "i" for for loop
looping: 
    slli a3, a2, 2      # calculate 4*i
    add a4, a0, a3      # get address of array element by adding base address + 4*i 
    sw zero, 0(a4)      # write 0 to memory address
    addi a2, a2, 1      # increment i 
    bne a2, a1, looping # continue looping if i < 8
    ret                 # return from eraseBuffer