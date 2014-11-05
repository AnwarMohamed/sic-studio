#pragma once
#include "cSourceFile.h"

#define zero(o, s) memset(o, 0, s);

cSourceFile::cSourceFile(CHAR* Filename): cFile(Filename) {
	is_ready = true;

    SICOpCode* opcode;
    for (int i = 0; i < 59; ++i)
    {
        opcode = new SICOpCode;
        zero(opcode, sizeof(SICOpCode));

        opcode->mnemonic = opcodes_mnemonic[i];
        opcode->opcode = opcodes_format[i][0];
        opcode->operands = opcodes_format[i][1];
        opcode->register_based = opcodes_format[i][2] ? true : false;

        _opcodes_table[opcodes_mnemonic[i]] = opcode;
    }
}

cSourceFile::~cSourceFile(void) {
}
