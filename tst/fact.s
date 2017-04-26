# print the result of (5!)
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
	movl $5, %eax
	pushl %eax
	call fact   
	prn %eax   # should be 120
	leave
	ret
