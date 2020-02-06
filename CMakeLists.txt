project(RegularVM)


add_executable(Assembler asm/asm.c asm/statement.c calc/token.c calc/error.c calc/calc.c calc/calc.h calc/token.h calc/error.h common/regs.c asm/compiler.c asm/compiler.h)
add_executable(Disassembler disasm/disasm.c asm/statement.c common/regs.c calc/calc.c calc/error.c calc/token.c disasm/disasm.h disasm/main.c asm/compiler.c asm/compiler.h)
add_executable(VM vm/vm.c asm/statement.c common/regs.c calc/calc.c calc/error.c calc/token.c disasm/disasm.c asm/compiler.c asm/compiler.h)
