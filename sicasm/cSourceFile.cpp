#pragma once
#include "cSourceFile.h"

#define zero(o, s) memset(o, 0, s);

char* sourcelines[54][3] = {
	{ "PRIME", "START", "1000" },
	{ "", "LDA", "FPRIME" },
	{ "", "COMP", "TWO" },
	{ "", "JLT", "SETNP" },
	{ "", "LDX", "TWO" },
	{ "CHKLOOP", "STX", "CHKIDX" },
	{ "", "JSUB", "CHKMOD" },
	{ "", "LDA", "CHKIDX" },
	{ "", "MUL", "CHKIDX" },
	{ "", "COMP", "FPRIME" },
	{ "", "LDA", "CHKIDX" },
	{ "", "ADD", "ONE" },
	{ "", "STA", "CHKIDX" },
	{ "", "LDX", "CHKIDX" },
	{ "", "JLT", "CHKLOOP" },
	{ "", "J", "SETP" },
	{ "CHKMOD", "LDA", "FPRIME" },
	{ "", "DIV", "CHKIDX" },
	{ "", "MUL", "CHKIDX" },
	{ "", "STA", "CHKMODR" },
	{ "", "LDA", "FPRIME" },
	{ "", "SUB", "CHKMODR" },
	{ "", "COMP", "ZERO" },
	{ "", "JEQ", "SETNP" },
	{ "", "RSUB", "" },
	{ "SETNP", "LDX", "ZERO" },
	{ "SETNPL", "TD", "OUTDEV" },
	{ "", "JEQ", "SETNPL" },
	{ "", "LDCH", "SETNPS,X" },
	{ "", "WD", "OUTDEV" },
	{ "", "TIX", "SETNPSS" },
	{ "", "JLT", "SETNPL" },
	{ "", "J", "EXIT" },
	{ "SETP", "LDX", "ZERO" },
	{ "SETPL", "TD", "OUTDEV" },
	{ "", "JEQ", "SETPL" },
	{ "", "LDCH", "SETPS,X" },
	{ "", "WD", "OUTDEV" },
	{ "", "TIX", "SETPSS" },
	{ "", "JLT", "SETPL" },
	{ "", "J", "EXIT" },
	{ "CHKIDX", "RESW", "1" },
	{ "CHKMODR", "RESW", "1" },
	{ "FPRIME", "WORD", "3" },
	{ "ZERO", "WORD", "0" },
	{ "ONE", "WORD", "1" },
	{ "TWO", "WORD", "2" },
	{ "SETNPSS", "WORD", "9" },
	{ "SETPSS", "WORD", "5" },
	{ "INDEV", "BYTE", "X'F0'" },
	{ "OUTDEV", "BYTE", "X'05'" },
	{ "SETNPS", "BYTE", "C'NOT PRIME'" },
	{ "SETPS", "BYTE", "C'PRIME'" },
	{ "EXIT", "END", "PRIME" },
};

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
        opcode->format = opcodes_format[i][3];

        _opcodes_table[opcodes_mnemonic[i]] = opcode;
    }

	SICCodeLine* line;
	for (int i = 0; i < 54; ++i) {
        line = new SICCodeLine;
        line->address = 0;
        line->operand_indexed = false;
        line->opcode_ref = nullptr;
		line->label = sourcelines[i][0];
		line->mnemonic = sourcelines[i][1];
		line->operands.push_back(sourcelines[i][2]);
		_siccode_lines.push_back(line);
	}
}

cSourceFile::~cSourceFile(void) {
}
