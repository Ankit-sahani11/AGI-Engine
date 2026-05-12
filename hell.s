 .text
.global main
.align 2

main:
    # write(1, msg, 14)
    mov x0, #1          # stdout
    adrp x1, msg@PAGE   # msg ka page address
    add x1, x1, msg@PAGEOFF # page offset (specific address)
    mov x2, #14         # length
    mov x8, #64         # write syscall
    svc #0

    # exit(0)
    mov x0, #0
    ret                 # 'main' function se return karne ke liye

.data
msg:
    .ascii "Hello, World!\n"

