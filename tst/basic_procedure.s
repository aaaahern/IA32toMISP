main:
	pushl %ebp
	movl %esp, %ebp
	movl $1, %eax
	pushl $1
	call func
	leave
	ret

func:
	pushl %ebp
	movl %esp, %ebp
	movl $123, %ebx
	leave
	ret