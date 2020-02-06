project(RegularVM)

add_executable(Assembler asm/asm.c asm/statement.c asm/calc/token.c asm/calc/error.c asm/calc/calc.c asm/calc/calc.h asm/calc/token.h asm/calc/error.h common/regs.c)
add_executable(Disassembler disasm/disasm.c asm/statement.c common/regs.c asm/calc/calc.c asm/calc/error.c asm/calc/token.c)
add_executable(VM vm/vm.c)
