.global _start
_start:
     mov x0,#0
     mov x8,#93
     svc #0

reflect_light:
    // 1. Calculate Dot Product: (N . I)
    fmul    v2.4s, v1.4s, v0.4s     // v2 = [Nx*Ix, Ny*Iy, Nz*Iz, 0]
    
    // Summing x, y, z components correctly
    faddp   v4.4s, v2.4s, v2.4s     // v4 = [x+y, z+0, x+y, z+0]
    faddp   v4.4s, v4.4s, v4.4s     // v4 = [(x+y)+(z+0), ...] -> s4 has the total sum
    
    // 2. Calculate 2 * (N . I)
    fadd    s4, s4, s4              // s4 = sum + sum (Faster than multiplying by 2.0)
    
    // 3. Broadcast 2*(N.I) to all lanes
    dup     v3.4s, v4.s[0]          // v3 = [scalar, scalar, scalar, scalar]
    
    // 4. Calculate vector: [2 * (N . I)] * N
    fmul    v3.4s, v3.4s, v1.4s     // v3 = 2(N.I) * N
    
    // 5. Final Reflection: R = I - v3
    fsub    v2.4s, v0.4s, v3.4s     // v2 = I - 2(N.I)N
    
    ret

