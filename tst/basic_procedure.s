# test basic procedure call with arguments
main:
	pushl %ebp
	movl %esp, %ebp
	pushl $3
	pushl $6
	pushl $7
	call func
	movl %eax, %ebx 
	prn %ebx  # %ebx should be 16
	leave
	ret

func:
	pushl %ebp
	movl %esp, %ebp
	movl 8(%ebp), %eax
	addl 12(%ebp), %eax
	addl 16(%ebp), %eax
	leave
	ret