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
	li $s7, 3
	sw $s7, 0($sp)
	addi $sp, $sp, -4
	li $s7, 6
	sw $s7, 0($sp)
	addi $sp, $sp, -4
	li $s7, 7
	sw $s7, 0($sp)
	jal func
	addi $sp, $sp, 12
	add $s0, $zero, $t0
	add $a0, $zero, $s0
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

.globl func
.ent func
func:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $fp, 0($sp)
	addi $fp, $sp, 0
	lw $t0, 8($fp)
	lw $s7, 12($fp)
	add $t0, $t0, $s7
	lw $s7, 16($fp)
	add $t0, $t0, $s7
	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end func

