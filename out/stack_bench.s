.data
	newline: .asciiz "\n"
.text
.globl main
.ent main
main:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $fp, 0($sp)
	addi $fp, $sp, 0
	li $t0, 0xf
	li $s0, 0

loop0:
	addi $sp, $sp, -4
	li $s7, 0
	sw $s7, 0($sp)
	addi $s0, $s0, 1
	add $a0, $zero, $s0
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	bne $s0, $t0, loop0


loop1:
	lw $t1, 0($sp)
	addi $sp, $sp, 4
	addi $s0, $s0, -1
	add $a0, $zero, $s0
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	bne $s0, 0, loop1

	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end main

