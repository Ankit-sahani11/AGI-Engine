.global _start// starting piint

.section .data// ram space
/* ​Address of msg: Assembler dekhta hai ki msg kahan shuru hua (Maano 0x1000).
​Address of msg_end: Assembler dek
hta hai ki msg_end kahan hai (kyunki string 12 by
tes ki hai, toh msg_end hoga 0x100C).
Decimal (Decimal) Hexadecimal (Hex)
10 = A
11 = B
12 = C
13 = D
14 = E
15 = F(Ox100F)
​Subtraction: Woh calculation karta hai: 0x100C - 0x1000.
​Result: 0x0C (Jo decimal mein 12 hai).
​Storage: Woh is 12 value ko my_length_
am wale address par jaakar likh deta hai.*/
length:
    msg: .ascii "hello pakoda \n"
    msg_end:
    my_length: .byte (msg_end - msg)
    msg_two: .ascii "2th pakoda \n"
    msg_tow_end:
    tow_my_length: .byte (msg_tow_end - msg_two)
    int: .byte 54
.section .text //code space
_start:
     bl halwai
     mov x0,#0
     mov x8,#93
     svc #0
halwai:
     mov x0,#1
     adrp x0,msg
     add x0, x0, :lo12:msg
     adrp x22,my_length
     add x22,x22,:lo12:my_length
     ldrb w1,[x22]
     bl print_string
     adrp x0,msg_two
     add x0,x0,:lo12:msg_two
     adrp x5,tow_my_length
     add x5,x5,:lo12:tow_my_length
     ldrb w1,[x5]
     bl print_string
     adrp x0,int
     add x0,x0,:lo12:int
     mov x1,#2
     bl print_int
//     bl experiment
     bl print_int	
     ret 
      // Arm 64 assembly ki ek problem hai 
     // address hota hai 64 bit ka 
     // intruction hota hai 32 bit ks 
     // abb 32 bit ke ander kese address copy ho sakata hai 64 bit ka
     // chote kuye me badi balti thodi na aat sakati hai
     // ram me data 4kb ke page me store kiya jata hai 
     /* ​:lo12:my_length: Yeh assembler ko kehta 
     hai ki "mujhe my_length ka last 12 bits ka 
     offset (position) chahiye".
​     add x5, x5, :lo12:my_length: Yeh adrp se mile hue 
     "Page Base Address" mein us "offset" ko 
     jod deta hai.*/
print_string:
    mov x2,x1// text address 
    mov x1 ,x0// length of text
    mov x0, #1  
    mov x8, #64
    svc #0
    ret
   
print_int:
    mov x2,x1
    mov x1 ,x0
    mov x0, #1  // STDOUT
    mov x8, #64
    svc #0
    ret
experiment:
    mov x0, #1
    adrp x6, int
    add x6,x6,:lo12:int
    ldrb w1,[x6]  
    mov x2, #1  
    mov x8, #64
    svc #0
