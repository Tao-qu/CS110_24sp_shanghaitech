.section .text
.globl draw
.type draw, @function
draw:
	li a0,112
	li a1,9
	li a2,128
	li a3,21
	li a4,65535
	addi sp,sp,-4
	sw ra,0(sp)
	jal LCD_DrawRectangle
	lw ra,0(sp)
	addi sp,sp,4
