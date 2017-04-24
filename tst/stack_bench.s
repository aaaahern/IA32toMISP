main:
	pushl %ebp
	movl %esp, %ebp
	movl $0xF, %eax
	movl $0, %ebx

loop0:	
	pushl $0
	incl %ebx
	prn %ebx
	cmpl %eax, %ebx
	jne loop0

loop1:	
	popl %ecx
	decl %ebx
	prn %ebx
	cmpl 0, %ebx
	jne loop1

	leave
	ret