# print out fib array
main:
	pushl %ebp
	movl %esp, %ebp
    movl $1, %eax
    movl $0, %ebx

loop:   addl %ebx, %eax
    addl %eax, %ebx

    prn %eax
    prn %ebx

    cmpl $433494437, %eax
    jg end

    cmpl $0, %ebx
    jg loop

end:
    leave
    ret 
