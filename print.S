.section .data
msg: .ascii "hello pakoda"
// Yahan hum length ko ek memory variable bana rahe hain
len_var: .byte 12   

.section .text
.global _start
_start:
    mov x0, #1           // FD
    
    // Address of msg
    adrp x1, msg
    add x1, x1, :lo12:msg 

    // ADRP ka use karke len_var ka address nikala
    adrp x5, len_var
    add x5, x5, :lo12:len_var
    
    // Ab us address se value load ki
    ldrb w2, [x5]        // w2 mein 12 aa gaya

    mov x8, #64          // Syscall write
    svc #0

    // Exit
    mov x0, #0
    mov x8, #93
    svc #0
