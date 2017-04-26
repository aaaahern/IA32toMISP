# accessing stack data via scaled indexed memory addressing
main:
	pushl %ebp
	movl %esp, %ebp
	pushl $1
	pushl $2
	pushl $3
	pushl $4
	pushl $5
	call access
	
	# print %ecx
	movl $4, %eax
	movl $1, %ebx
	movl %ecx, %ecx
	movl $5, %edx
	int 80h

	leave
	ret

access:
	pushl %ebp
	movl %esp, %ebp
	movl $4, %ebx
	movl (%ebp, %ebx, 4), %ecx  # %ecx should be 3
	leave
	ret