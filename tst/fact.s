fact:
	pushl %ebp	
	movl %esp, %ebp
	movl 4(%ebp), %eax	
	cmpl $1, %eax	
	jle end_fact	
	movl %eax, %ecx	
	decl %eax
	pushl %eax
	call fact
	popl %ebx
	imull %ecx, %eax
end_fact:
	leave
	ret


main:
	movl $5, %eax
	pushl %eax
	call fact
	leave
	ret