.text
.global main
.align 2

main:
    // write(1, msg, 14)
    mov x0, #1          // 1 = stdout                                         
    mov x2, #14         // Message length
    mov x8, #64         // Syscall 64 (write)
    svc #0              // Call kernel

    // exit(0)
    mov x0, #0          // Return status 0
    ret                 // Return to caller

.data
msg:
    .ascii "Hello, pakoda!\n"
