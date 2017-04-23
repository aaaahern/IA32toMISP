main:
	pushl %ebp
	movl %esp, %ebp
    movl $1, %eax
    movl $0, %ebx

loop:   addl %ebx, %eax
    addl %eax, %ebx

    cmpl $1134903170, %eax
    jl end

    cmpl $0, %ebx
    jg loop

end:
    leave
    ret 
