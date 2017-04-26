# print the result of from (1!) to (10!)
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
	popl %ecx
	imull %ecx, %eax
end_fact:
	leave
	ret


main:
	pushl %ebp	
	movl %esp, %ebp
	movl $0, %ebx
loop:
	incl %ebx
	pushl %ebx
	movl %ebx, %eax
	pushl %eax
	call fact
	prn %eax
	popl %ebx
	cmpl $10, %ebx
	jl loop
	leave
	ret
