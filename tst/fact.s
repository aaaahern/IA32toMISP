fact:
	pushl %ebp	
	movl %esp, %ebp
	movl 8(%ebp), %eax	
	cmpl $1, %eax	
	jle end_fact	
	movl %eax, %ecx
	pushl %ecx
	decl %eax
	pushl %eax
	call fact
	imull %ecx, %eax
	popl %ecx
end_fact:
	leave
	ret


main:
	pushl %ebp	
	movl %esp, %ebp
	movl $5, %eax
	pushl %eax
	call fact
	leave
	ret