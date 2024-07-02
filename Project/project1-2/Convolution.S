.import input.S

.text
main:
    la t0 image_length
    lw s0 0(t0)
    la t0 image_width
    lw s1 0(t0)
    la t0 kernel_length
    lw s2 0(t0)
    la t0 kernel_width
    lw s3 0(t0)
    la s4 image
    la s5 kernel

    sub s6 s0 s2
    addi s6 s6 1
    sub s7 s1 s3
    addi s7 s7 1

    li a1 0
    li s8 0
    li s9 0
    li s10 0
    li s11 0

kernel_loop:
    mv t5 s4
    add t0 s8 s10
    mul t0 t0 s0
    add t0 t0 s9
    add t0 t0 s11
    slli t0 t0 2
    add t5 t5 t0

    mv t6 s5
    mul t0 s10 s2
    add t0 t0 s11
    slli t0 t0 2
    add t6 t6 t0

    lw t1 0(t5)
    lw t2 0(t6)
    mul t1 t1 t2
    add a1 a1 t1

image_loop:
    addi s11 s11 1
    blt s11 s2 kernel_loop
    li s11 0
    addi s10 s10 1
    blt s10 s3 kernel_loop
    li s10 0
    li a0 1
    ecall
    li a1 32
    li a0 11
    ecall
    li a1 0
    addi s9 s9 1
    blt s9 s6 kernel_loop
    li s9 0
    li a1 10
    li a0 11
    ecall
    li a1 0
    addi s8 s8 1
    blt s8 s7 kernel_loop
    li s8 0

end:
    li a0 10
    ecall