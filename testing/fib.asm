; calculate the nth fibonacci number (zero-indexed)
; n is stored in r1
; the result is stored in r2
SET r1, 7

; int fibonacci(n) {
;   if(n == 0) return 0;
;   if(n == 1) return 1;
;   return fibonacci(n - 1) + fibonacci(n - 2)
; }

main:
PUSH r1 ; pass r1 to the function
CALL $fibonacci
POP at0 ; caller responsible for poping args
MOV r2, rr ; save result to r2
HALT

fibonacci:
; sav regs
PUSH r3
PUSH r4

; get n from args
RARG r3, 3

; if(r3 == 0) return 0
JZ r3, $returnZero

; if(r3 == 1) return 1
DEC r3
JZ r3, $returnOne

; r4 = fibonacci(n - 1)
PUSH r3
CALL $fibonacci
POP at0
MOV r4, rr

; r3 = fibonacci(n - 2)
DEC r3
PUSH r3
CALL $fibonacci
POP at0
MOV r3, rr

; return r4 + r3
ADD rr, r4, r3
POP r4 ; r4
POP r3 ; r3
RET

returnOne:
POP r4 ; r4
POP r3 ; r3
SET rr, 1
RET

returnZero:
POP r4 ; r4
POP r3 ; r3
SET rr, 0
RET

