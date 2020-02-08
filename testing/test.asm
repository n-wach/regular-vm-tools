init:
SET r1, 10 ; we gonna sum the numbers from 1 to 10 into r2

loop:
ADD r2, r2, r1 ; add cur
DEC r1
JZ r1, $end
JMPI $loop

end:
HALT

