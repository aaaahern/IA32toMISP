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
    prn     %eax   # should be 343
    movl    $0, %eax
    leave
    ret
