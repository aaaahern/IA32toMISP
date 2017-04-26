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
	li $s7, 6
	sw $s7, 0($sp)
	jal add1
	addi $sp, $sp, 4
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
	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end main

.globl add1
.ent add1
add1:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $fp, 0($sp)
	addi $fp, $sp, 0
	lw $t0, 8($fp)
	addi $t0, $t0, 1
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal add2
	addi $sp, $sp, 4
	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end add1

.globl add2
.ent add2
add2:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $fp, 0($sp)
	addi $fp, $sp, 0
	lw $t0, 8($fp)
	addi $t0, $t0, 2
	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end add2

