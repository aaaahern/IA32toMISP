main:
	pushl %ebp
	movl %esp, %ebp
	pushl $3
	pushl $3
	pushl $3
	call func
	movl %eax, %ebx
	leave
	ret

func:
	pushl %ebp
	movl %esp, %ebp
	movl 4(%ebp), %eax
	addl 8(%ebp), %eax
	addl 12(%ebp), %eax
	leave
	ret
