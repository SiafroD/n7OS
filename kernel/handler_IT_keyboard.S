.text
    # regarder dans handler_IT.S pour commentaires
    .globl handler_IT_keyboard
handler_IT_keyboard:
    push %eax
    push %edx
    push %ecx
    call handler_keyboard
    popl %ecx
    popl %edx
    popl %eax
    iret