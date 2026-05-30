.global _start// starting piint

.section .data
length:
    msg: .ascii "hello pakoda \n"
    msg_end:
    my_length: .byte (msg_end - msg)
.section .text //code space
.macro Dot_product vec1,vec2

.endmacro
.macro cross_product vec1,vec2
.endmacro
_start:
     bl halwai
     bl exit
exit:    
     mov x0,#0
     mov x8,#93
     svc #0
halwai:
     adrp x0,msg
     add x0, x0, :lo12:msg
     adrp x22,my_length
     add x22,x22,:lo12:my_length
     ldrb w1,[x22]
     bl print_string
print_string:
    mov x2,x1// length
    mov x1 ,x0//text ad.
    mov x0, #1
    mov x8, #64
    svc #0
    ret

