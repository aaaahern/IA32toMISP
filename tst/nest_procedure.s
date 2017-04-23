main:
	pushl %ebp
	movl %esp, %ebp
	pushl $6
	call add1
	movl %eax, %ebx
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
