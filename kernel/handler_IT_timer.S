.text
    # regarder dans handler_IT.S pour commentaires
    .globl handler_IT_timer
handler_IT_timer:
    push %eax
    push %edx
    push %ecx
    call handler_timer
    popl %ecx
    popl %edx
    popl %eax
    iret