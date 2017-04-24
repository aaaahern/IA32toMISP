main:
	pushl %ebp
	movl %esp, %ebp		
	movl $2, %ecx	

checkPrime:	
	movl $2, %ebx	

checkFactor:	
	cmpl %ebx, %ecx
	je primeFound
	
	movl %ecx, %eax
	cltd
	idivl %ebx
	cmpl $0, %edx
	je nextPrime
	
	incl %ebx
	jmp checkFactor

primeFound:	
	prn %ecx

nextPrime:	
	incl %ecx
	cmpl $50, %ecx
	jl checkPrime
	leave
	ret

