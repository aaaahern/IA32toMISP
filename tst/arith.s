# test basic arithmetic operations
main:
    pushl %ebp
    movl %esp, %ebp
    movl $67, %eax
    movl $7, %ebx
    subl %ebx, %eax # 67-7 = 60
    prn %eax
    movl $2, %ebx
    imull %ebx, %eax # 60*2 = 120
    prn %eax
    shrl $3, %eax # 120>>3 = 15
    prn %eax
    leave
    ret
