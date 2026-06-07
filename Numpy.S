.global _start// starting piint

.section .bss
    Scalar: .skip 8  
.section .data
length:
    msg: .ascii "hello pakoda \n" 
    msg_end:
    .equ my_length, (msg_end - msg)
    Array: .byte 10,20,30,40,50
    Index: .byte 1,2,3,4,5    
.section .text //code space

.macro cross_product vec1,v

.endm

_start:
     mov x2,#5
     bl Simd_dot_product
     bl run_loop
     bl exit
exit:    
     mov x0,#0
     mov x8,#93
     svc #0
halwai:
     adrp x0,msg
     add x0, x0, :lo12:msg
     mov x1,#my_length
     ret
print_string:
    mov x2,x1// length
    mov x1 ,x0//text ad.
    mov x0, #1
    mov x8, #64
    svc #0
    ret
run_loop:
     sub sp,sp,#16
     str x19,[sp,#0]
     str x30,[sp,#8]
     mov x19, #5
loop:
    bl halwai
    bl print_string
    subs x19,x19,#1
    b.ne loop
    ldr x19,[sp,#0]
    ldr x30,[sp,#8]
    add sp ,sp,#16 
    ret
dot_product:
    // Formual \sum (A[i] \times B[i])
    mov x3,#0 // reasult store
    mov x4,#0 // index of array
    adrp x5,Array
    add x5,x5,:lo12:Array
    adrp x7,Index
    add x7,x7,:lo12:Index
dp_loop:
    cmp x4,x2
    b.eq dp_done
    ldrb w6,[x5,x4] // Memory Address = Base Address (x5) + Offset (x4)
    ldrb w8,[x7,x4] // Memory Address = Base Address (x5) + Offset (x4)
    madd x3, x6, x8, x3
    add x4, x4, #1   
    b dp_loop

dp_done:
    ret

Simd_dot_product:
    mov x3,#0 // reasult store
    mov x4,#0 // index of array
    adrp x5,Array
    add x5,x5,:lo12:Array
    adrp x7,Index
    add x7,x7,:lo12:Index
    // Pointers setup ho chuke hain (x5, x7)
    ld1 {v0.8b}, [x5]       // Array ke 8 bytes load karo
    ld1 {v1.8b}, [x7]       // Index ke 8 bytes load karo    
    umull v2.8h, v0.8b, v1.8b // Multiply (8-bit -> 16-bit)
    addv h3, v2.8h          // Sabka sum nikalo
    umov w3, v3.h[0]        // Result ko x3 mein lao (taaki print kar sako)
    ret

Simd_Cross_Product: 
    
