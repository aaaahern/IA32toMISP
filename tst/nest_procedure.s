# test nested procedure
main:
	pushl %ebp
	movl %esp, %ebp
	pushl $6
	call add1
	movl %eax, %ecx # %ecx should be 9
	# print %ecx
    movl $4, %eax
    movl $1, %ebx
    movl $5, %edx
    int 80h
	leave
	ret

add1:
	pushl %ebp
	movl %esp, %ebp
	movl 8(%ebp), %eax
	addl $1, %eax
	pushl %eax
	call add2
	leave
	ret

add2:
	pushl %ebp
	movl %esp, %ebp
	movl 8(%ebp), %eax
	addl $2, %eax
	leave
	ret
