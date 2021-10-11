
; 1. this line is ok
DATA1: .data 100

; 2. this line is ok (highest positive immediate that fits in 12 bits)
       cmp #2047,DATA1



; 4. this line is ok (lowest negative immediate that fits in 12 bits)
       cmp r2,#-2048



; 6. this line is ok
       stop

