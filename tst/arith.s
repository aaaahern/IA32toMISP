main:
    pushl %ebp
    movl %esp, %ebp
    movl $5, %eax
    movl $2, %ebx
    subl %ebx, %eax # 5-2 = 3
    imull %ebx, %eax # 3*2 = 6
    shrl $1, %eax # 6>>2 = 3
    leave
    ret
