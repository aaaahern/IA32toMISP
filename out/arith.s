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
	li $t0, 67
	li $s0, 7
	sub $t0, $t0, $s0
	add $a0, $zero, $t0
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	li $s0, 2
	mult $s0, $t0
	mflo $t0
	add $a0, $zero, $t0
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	srl $t0, $t0, 3
	add $a0, $zero, $t0
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end main

