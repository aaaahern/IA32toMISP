test1:
	pushl %ebp
	movl %esp, %ebp
	movl %eax, %ebx #register to register
	leave
	ret

test2:
	pushl %ebp
	movl %esp, %ebp
	movl %eax, 3(%ebx) #register to indirect memory
	leave
	ret

test3:
	pushl %ebp
	movl %esp, %ebp
	movl %eax, 3 #register to absolute memory
	leave
	ret

test4:
	pushl %ebp
	movl %esp, %ebp
	movl %eax, 3(%ebx, %ecx) #register to indexed memory
	leave
	ret

test5:
	pushl %ebp
	movl %esp, %ebp
	movl %eax, 3(%ebx, %ecx, 10) #register to scaled indexed memory
	leave
	ret

test6:
	pushl %ebp
	movl %esp, %ebp
	movl $2, %eax #immediate to register
	leave
	ret

test7:
	pushl %ebp
	movl %esp, %ebp
	movl $2, 4(%eax) #immediate to indirect memory
	leave
	ret

test8:
	pushl %ebp
	movl %esp, %ebp
	movl $2, 4 #immediate to absolute memory
	leave
	ret

test9:
	pushl %ebp
	movl %esp, %ebp
	movl $2, (%eax, %ebx) #immediate to indexed memory
	leave
	ret

test10:
	pushl %ebp
	movl %esp, %ebp
	movl $2, (%eax, %ebx, 8) #immediate to scaled indexed memory
	leave
	ret

test11:
	pushl %ebp
	movl %esp, %ebp
	movl (%eax), %ebx #indirect memory to register
	leave
	ret

test12:
	pushl %ebp
	movl %esp, %ebp
	movl 6, %ebx #absolute memory to register
	leave
	ret

test13:
	pushl %ebp
	movl %esp, %ebp
	movl 2(%eax, %ecx), %ebx #indexed memory to register
	leave
	ret

test14:
	pushl %ebp
	movl %esp, %ebp
	movl 5(, %ecx, 8), %ebx #scaled indexed memory to register
	leave
	ret
