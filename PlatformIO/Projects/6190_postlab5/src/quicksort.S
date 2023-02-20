.section .text     
.align 2 
.globl quicksort

quicksort:
  
  addi sp, sp, -20
  sw ra, 0(sp)
  sw s0, 4(sp)
  sw s1, 8(sp)
  sw s2, 12(sp)
  sw s3, 16(sp)
    
  mv s1, a0
  mv s2, a2
  mv s3, a1
  bge a1, a2, end
    
  call partition
  mv s0, a0

  mv a0, s1
  call arrayViz

  mv a0, s0
  addi a2, a0, -1
  mv a0, s1 
  mv a1, s3
  call quicksort
   
   
  mv a0, s1
  mv a2, s2
  mv a1, s0
  addi a1, a1, 1
  call quicksort
  
end: 
  lw ra, 0(sp)
  lw s0, 4(sp)
  lw s1, 8(sp)
  lw s2, 12(sp)
  lw s3, 16(sp)
  addi sp, sp, 20
  ret



partition:
  addi sp, sp, -20
  sw s0, 0(sp)
  sw s1, 4(sp)
  sw s2, 8(sp)
  sw s3, 12(sp)
  sw ra, 16(sp)

  addi a3, a1, -1  # start - 1 = i
  mv t2, a1  # j 

  slli a6, a2, 2  
  add a6, a6, a0
  lw a4, 0(a6) # p[end]
    
  slli a5, a1, 2 
  add a5, a5, a0  # p[start] mem address
    
for_loop:
  bge t2, a2, real_end  # j >= end, end
  lw t3, 0(a5)  # t3= p[j]
  ble t3, a4, check # p[j] <= x = p[end]
  addi t2, t2, 1  # j = j +1
  addi a5, a5, 4 # move adress for the p[j]
  j for_loop
check:
  addi a3, a3, 1   # i = i+1
  slli t1, a3, 2   # 4*i
  add t1, t1, a0  # t1 = p[i] adress
  lw t0, 0(t1)    # t0 =p[i]
  sw t3, 0(t1)    # t3 -> p[i] = p[j]

  sw t0, 0(a5)    # t0 = p[j] = temp
  addi t2, t2, 1  # j ++
  addi a5, a5, 4  # move adress for p[j] 
  j for_loop
real_end: 
  addi a3, a3, 1   //a3 = i = i+1
  slli t1, a3, 2   // t1 = i*4
  add t1, t1, a0   // t1 = array + i*4
  lw t0, 0(t1)     // t0 = p[i+1] = temp
  lw t3,0(a6)     // t3 = p[end]
  sw t3, 0(t1)   // p[i+1] = p[end]
  sw t0, 0(a6)   // temp = p[end]
  mv a0, a3    // a0 = i +1
  ret
  