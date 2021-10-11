
; 1. this line is ok
int:     .data 0

; 2. this line is ok 
start:   jsr  %end

; 3. this line is ok
         bne  %start

; 4. this line is ok 
         jmp  %start         




; 12. invalid operand (or invalid characters (%start))
end:     stop 

