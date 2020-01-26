project(RegularVM)

add_executable(Assembler asm/asm.c common/ops.c common/regs.c)
add_executable(Disassembler disasm/disasm.c common/ops.c common/regs.c)
add_executable(VM vm/vm.c common/ops.c common/regs.c)
