# print the result of 7^3
power:
    pushl   %ebp
    movl    %esp, %ebp
    movl    8(%ebp), %eax
    movl    12(%ebp), %ebx
    movl    8(%ebp), %ecx
loop:
    imull   %ecx, %eax
    decl    %ebx
    cmpl    $1, %ebx
    jg      loop
    leave
    ret
    
main:
    pushl   %ebp
    movl    %esp, %ebp
    pushl   $3
    pushl   $7
    call    power
    # %eax should be 343
    # print %eax
    movl %eax, %ecx
    movl $4, %eax
    movl $1, %ebx
    movl $5, %edx
    int 80h

    movl    $0, %eax
    leave
    ret
