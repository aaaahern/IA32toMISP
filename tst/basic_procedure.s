# test basic procedure call with arguments
main:
	pushl %ebp
	movl %esp, %ebp
	pushl $3
	pushl $6
	pushl $7
	call func
	movl %eax, %ecx 
	# %ecx should be 16
	# print %ebx
	movl $4, %eax
	movl $1, %ebx
	movl $5, %edx
	int 80h
	leave
	ret

func:
	pushl %ebp
	movl %esp, %ebp
	movl 8(%ebp), %eax
	addl 12(%ebp), %eax
	addl 16(%ebp), %eax
	leave
	ret
