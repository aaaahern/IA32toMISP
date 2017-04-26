.data
	newline: .asciiz "\n"
.text
.globl fact
.ent fact
fact:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $fp, 0($sp)
	addi $fp, $sp, 0
	lw $t0, 8($fp)
	ble $t0, 1, end_fact

	add $t1, $zero, $t0
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	addi $t0, $t0, -1
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal fact
	addi $sp, $sp, 4
	lw $t1, 0($sp)
	addi $sp, $sp, 4
	mult $t1, $t0
	mflo $t0

end_fact:
	lw $fp, 0($sp)
	lw $ra, 4($sp)
	add $sp, $sp, 8
	jr $ra
.end fact

.globl main
.ent main
main:
	addi $sp, $sp, -8
	sw $ra, 4($sp)
	sw $fp, 0($sp)
	addi $fp, $sp, 0
	li $t0, 5
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	jal fact
	addi $sp, $sp, 4
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

