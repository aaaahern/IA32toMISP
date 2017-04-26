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
	addi $sp, $sp, -4
	li $s7, 1
	sw $s7, 0($sp)
	addi $sp, $sp, -4
	li $s7, 2
	sw $s7, 0($sp)
	addi $sp, $sp, -4
	li $s7, 3
	sw $s7, 0($sp)
	addi $sp, $sp, -4
	li $s7, 4
	sw $s7, 0($sp)
	addi $sp, $sp, -4
	li $s7, 5
	sw $s7, 0($sp)
	jal access
	addi $sp, $sp, 20
	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end main

.globl access
.ent access
access:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $fp, 0($sp)
	addi $fp, $sp, 0
	li $s0, 4
	addi $s6, $zero, 4
	mult $s6, $s0
	mflo $s6
	add $s6, $s6, $fp
	lw $t1, 0($s6)
	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end access

