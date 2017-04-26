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
	li $t0, 1
	li $s0, 0

loop:
	add $t0, $t0, $s0
	add $s0, $s0, $t0
	add $a0, $zero, $t0
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	add $a0, $zero, $s0
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall
	bgt $t0, 433494437, end

	bgt $s0, 0, loop


end:
	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end main

