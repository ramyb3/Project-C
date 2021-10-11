
; 1. this line is ok
X:   cmp r1,r2

; 2. this line is ok (highest positive value that fits in 12 bits)
Z1:  .data  2047



; 4. this line is ok (lowest negative value that fits in 12 bits)
Z3:  .data  -2048



; 6. this line is ok
     stop

