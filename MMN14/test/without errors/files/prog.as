
; 1. this line is ok (label X is ignored, warning may be issued in 1st pass)
X:     .extern DATA1

; 2. this line is ok (label Y is ignored, warning may be issued in 1st pass)
Y:     .entry START

; 3. this line is ok
       .entry END

; 4. this line is ok (DATA1 is an external label)
START:  add  DATA1,r1


; 9. this line is ok (it is ok to designate the same entry more than once)
       .entry END

; 10. this line is ok
END:   stop

; 11. undefined label 
       .entry Q

; 12. undefined label (declaration of label X was ignored) 
       .entry X

; 13. undefined label (declaration of label Y was ignored)  
       .entry Y

