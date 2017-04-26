.data
	newline: .asciiz "\n"
.text
.globl power
.ent power
power:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $fp, 0($sp)
	addi $fp, $sp, 0
	lw $t0, 8($fp)
	lw $s0, 12($fp)
	lw $t1, 8($fp)

loop:
	mult $t1, $t0
	mflo $t0
	addi $s0, $s0, -1
	bgt $s0, 1, loop

	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end power

.globl main
.ent main
main:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $fp, 0($sp)
	addi $fp, $sp, 0
	addi $sp, $sp, -4
	li $s7, 3
	sw $s7, 0($sp)
	addi $sp, $sp, -4
	li $s7, 7
	sw $s7, 0($sp)
	jal power
	addi $sp, $sp, 8
	add $t1, $zero, $t0
	li $t0, 4
	li $s0, 1
	li $t2, 5
	add $a0, $zero, $t1
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $t0, 0
	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end main

