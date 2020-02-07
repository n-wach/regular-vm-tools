project(RegularVM)


add_executable(Assembler
        asm/asm.c
        asm/statement.c
        asm/compiler.c
        asm/main.c
        calc/token.c
        calc/error.c
        calc/calc.c
        common/regs.c )
add_executable(Disassembler
        disasm/disasm.c
        disasm/main.c
        common/regs.c)
add_executable(VM
        vm/vm.c
        vm/main.c
        disasm/disasm.c
        common/regs.c)
