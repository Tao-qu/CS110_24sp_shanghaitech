.data
str: .byte 'L', 'O', 'A', 'D', 'I', 'N', 'G', 0
title: .byte 'F', 'l', 'a', 'p', 'p', 'y', ' ', 'B', 'i', 'r', 'd', 0
.section .text
.globl draw
.type draw, @function

draw:
	li a0, 1
	li a1, 1
	li a2, 159
	li a3, 79
	li a4, 65535
	addi sp, sp, -4
	sw ra, 0(sp)
	jal LCD_DrawRectangle
	lw ra, 0(sp)
	addi sp, sp, 4

	li a0, 43
	li a1, 32
	la a2, str
	li a3, 65535
	addi sp, sp, -4
	sw ra, 0(sp)
	jal LCD_ShowString
	lw ra, 0(sp)
	addi sp, sp, 4

	li s0, 3       
    li s1, 105     

loop:
    li a0, 1000
    addi sp, sp, -4
    sw ra, 0(sp)
    jal delay_1ms
    lw ra, 0(sp)
    addi sp, sp, 4

    addi a0, s1, 0 
    li a1, 40
    li a2, 65535
    addi sp, sp, -4
    sw ra, 0(sp)
    jal LCD_DrawPoint_big
    lw ra, 0(sp)
    addi sp, sp, 4

    addi s1, s1, 5  
    addi s0, s0, -1 
    bnez s0, loop

draw_title:
	li a0, 3000
    addi sp, sp, -4
    sw ra, 0(sp)
    jal delay_1ms
    lw ra, 0(sp)
    addi sp, sp, 4

	li a0, 2
	li a1, 2
	li a2, 158
	li a3, 78
	li a4, 0
	addi sp, sp, -4
	sw ra, 0(sp)
	jal LCD_Fill
	lw ra, 0(sp)
	addi sp, sp, 4

	li a0, 30
	li a1, 32
	la a2, title
	li a3, 65535
	addi sp, sp, -4
	sw ra, 0(sp)
	jal LCD_ShowString
	lw ra, 0(sp)
	addi sp, sp, 4