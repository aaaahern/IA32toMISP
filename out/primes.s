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
	li $t1, 2

checkprime:
	li $s0, 2

checkfactor:
	beq $t1, $s0, primefound

	add $t0, $zero, $t1
	div $t0, $s0
	mflo $t0
	mfhi $t2
	beq $t2, 0, nextprime

	addi $s0, $s0, 1
	b checkfactor

primefound:
	add $a0, $zero, $t1
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, newline
	syscall

nextprime:
	addi $t1, $t1, 1
	blt $t1, 50, checkprime

	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end main

