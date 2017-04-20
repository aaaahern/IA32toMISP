fact:
	pushl %ebp	
	movl %esp, %ebp
	popl %eax	
	cmpl $1, %eax	
	jle end_fact	
	movl %eax, %ecx	
	decl %eax
	pushl %eax
	call fact
	imull %ecx, %ebx
end_fact:
	leave
	ret


main:
	movl $0, %eax
loop:	incl %eax
	pushl %eax
	call fact
	cmpl $10, %eax
	jl loop
	leave
	ret