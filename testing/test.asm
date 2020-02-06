main: NOP
NOP
NOP ;dumb comment
ADD r3, r3, r3
ADD r4, r3, r3
ADD r5, r3, r3
JMPI $main + 4 ; jump to main plus 1 instruction
PUSH pc
ADD r6, r3, r3
ADD r7, r3, r3
ADD r8, r3, r3

