; This file is intended to check the bad-path of the assembler.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; All errors should be detectable in the first pass of the assembler.

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; Disclaimer: this list of errors is not necessarily exhaustive; 
;             you are encouraged to identify additional errors.

; 1. this line is ok (label DATA4 is declared later)
Start:  dec DATA4




; 5. this line is ok (label declared as external)
       .extern DATA4

; 7. this line is ok (immediate target operand allowed for cmp)
Next:   cmp  #5,#6


   			

; 17. label was already declared earlier
   clr  r2

; 18. label does not start in first column (optional error!)
     label1:   sub  r1,r7


; 22. this line is ok (.data directive can be used without a label)
        .data 400

; 23. this line is ok (.string directive can be used without a label)
        .string "maman14"


; 25. this line is ok (arbitrary spaces/tabs between fields or at end of line)
  .data   100,    	200 ,  -500,300, 500   				   

; 26. this line is ok (arbitrary spaces/tabs between fields or at end of line)
        cmp     r1  ,      DATA1   				   

; 27. this line is ok (no spaces/tabs between operands)
        add     #1,r2

; 35. label was already declared earlier
DATA1:  .data   300

; 36. this line is ok (new label, as labels are case sensitive)
Data1:  .data   100, +200, -300

; 38. this line is ok (label X declaration ignored - warning may be issued)
X:      .entry  DATA1

; 39. this line is ok (it is ok to declare the same external more than once)
        .extern DATA4


; 41. this line is ok (label Y declaration ignored - warning may be issued)
Y:      .extern DATA8

; 42. this line is ok (label STOP is declared later)
        .entry  STOP


; 46. This line is ok (it is ok to designate the same entry more than once)
        .entry  DATA1

; 49. this line is ok (comma within string is not a separator)
STR1:   .string "abc, ,defg"



; 54. this line is ok
        rts

; 56. this line is ok (case sensitivity)
STOP:   stop

