/*
*
*  Copyright (C) 2014  Anwar Mohamed <anwarelmakrahy[at]gmail.com>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to Anwar Mohamed
*  anwarelmakrahy[at]gmail.com
*
*/

#pragma once
#include <string>
#include <string>
#include <vector>

using namespace std;

#ifdef _WIN32
#pragma warning (disable : 4251)
#include <Windows.h>
#endif

#ifndef DLLEXPORT
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport) 
#else
#define DLLEXPORT
#endif
#endif

#define zero(o, s) memset(o, 0, s);

#define ERROR_MISSING_OPCODE    "missing operation code"
#define ERROR_UNRECOG_OPCODE    "unrecognized operation code"
#define ERROR_ILLEGAL_OPCODE    "illegal operation code format"
#define ERROR_ILLEGAL_OPCODE_V3 "not a format 4 operation code"
#define ERROR_MISSING_OPERAND   "missing or misplaced operand in mnemonic"
#define ERROR_ILLEGAL_OPERAND   "illegal operand in mnemonic"
#define ERROR_UNDEFINED_SYM     "undefined symbol in operand"
#define ERROR_MISSING_START     "missing or misplaced start statement"
#define ERROR_ILLEGAL_START     "illegal operand in start statement"
#define ERROR_DUPLICATE_START   "duplicate or misplaced start statement"
#define ERROR_OPERAND_END       "missing or misplaced operand in end statement"
#define ERROR_AFTER_END         "statement should not follow end statement"
#define ERROR_OPERAND_WORD      "missing or misplaced operand in word statement"
#define ERROR_ILLEGAL_WORD      "illegal operand in word statement"
#define ERROR_OPERAND_RESW      "missing or misplaced operand in resw statement"
#define ERROR_ILLEGAL_RESW      "illegal operand in resw statement"
#define ERROR_OPERAND_RESB      "missing or misplaced operand in resb statement"
#define ERROR_ILLEGAL_RESB      "illegal operand in resb statement"
#define ERROR_OPERAND_BYTE      "missing or misplaced operand in byte statement"
#define ERROR_ILLEGAL_BYTE      "illegal operand in byte statement"
#define ERROR_DUPLICATE_LABEL   "duplicate label definition"
#define ERROR_ILLEGAL_LABEL     "illegal format in label field"
#define ERROR_SUGGEST           "do you mean"
#define ERROR_ILLEGAL_INDIRECT  "illegal indirect operand"
#define ERROR_ILLEGAL_BASE      "illegal operand in base statement"
#define ERROR_ILLEGAL_LITERAL   "illegal operand in literal statement"
#define ERROR_ILLEGAL_EQU		"illegal equ statement"
#define ERROR_ILLEGAL_ORG		"illegal org statement"
#define ERROR_ILLEGAL_USE		"illegal use statement"

#define BYTE_INSTRUCTION_INVALID	0x00
#define BYTE_INSTRUCTION_HEX		0x01
#define BYTE_INSTRUCTION_CHAR		0x02

#define MIN_INT         (-2147483647L - 1)
#define DEFAULT_BLOCK   "default_block"

#define OPCODES_SIZE 59
static char* opcodes_mnemonic[OPCODES_SIZE] = {
    "ADD",
    "ADDF",
    "ADDR",
    "AND",
    "CLEAR",
    "COMP",
    "COMPF",
    "COMPR",
    "DIV",
    "DIVF",
    "DIVR",
    "FIX",
    "FLOAT",
    "HIO",
    "J",
    "JEQ",
    "JGT",
    "JLT",
    "JSUB",
    "LDA",
    "LDB",
    "LDCH",
    "LDF",
    "LDL",
    "LDS",
    "LDT",
    "LDX",
    "LPS",
    "MUL",
    "MULF",
    "MULR",
    "NORM",
    "OR",
    "RD",
    "RMO",
    "RSUB",
    "SHIFTL",
    "SHIFTR",
    "SIO",
    "SSK",
    "STA",
    "STB",
    "STCH",
    "STF",
    "STI",
    "STL",
    "STS",
    "STSW",
    "STT",
    "STX",
    "SUB",
    "SUBF",
    "SUBR",
    "SVC",
    "TD",
    "TIO",
    "TIX",
    "TIXR",
    "WD",
};

static unsigned char opcodes_format[59][4] = {
    { 0x18, 1, 0, 3 },
    { 0x58, 1, 0, 3 },
    { 0x90, 2, 1, 2 },
    { 0x40, 1, 0, 3 },
    { 0xB4, 1, 1, 2 },
    { 0x28, 1, 0, 3 },
    { 0x88, 1, 0, 3 },
    { 0xA0, 2, 1, 2 },
    { 0x24, 1, 0, 3 },
    { 0x64, 1, 0, 3 },
    { 0x9C, 2, 1, 2 },
    { 0xC4, 0, 0, 1 },
    { 0xC0, 0, 0, 1 },
    { 0xF4, 0, 0, 1 },
    { 0x3C, 1, 0, 3 },
    { 0x30, 1, 0, 3 },
    { 0x34, 1, 0, 3 },
    { 0x38, 1, 0, 3 },
    { 0x48, 1, 0, 3 },
    { 0x00, 1, 0, 3 },
    { 0x68, 1, 0, 3 },
    { 0x50, 1, 0, 3 },
    { 0x70, 1, 0, 3 },
    { 0x08, 1, 0, 3 },
    { 0x6C, 1, 0, 3 },
    { 0x74, 1, 0, 3 },
    { 0x04, 1, 0, 3 },
    { 0xD0, 1, 0, 3 },
    { 0x20, 1, 0, 3 },
    { 0x60, 1, 0, 3 },
    { 0x98, 2, 1, 2 },
    { 0xC8, 0, 0, 1 },
    { 0x44, 1, 0, 3 },
    { 0xD8, 1, 0, 3 },
    { 0xAC, 2, 1, 2 },
    { 0x4C, 0, 0, 3 },
    { 0xA4, 2, 1, 2 },
    { 0xA8, 2, 1, 2 },
    { 0xF0, 0, 0, 1 },
    { 0xEC, 1, 0, 3 },
    { 0x0C, 1, 0, 3 },
    { 0x78, 1, 0, 3 },
    { 0x54, 1, 0, 3 },
    { 0x80, 1, 0, 3 },
    { 0xD4, 1, 0, 3 },
    { 0x14, 1, 0, 3 },
    { 0x7C, 1, 0, 3 },
    { 0xE8, 1, 0, 3 },
    { 0x84, 1, 0, 3 },
    { 0x10, 1, 0, 3 },
    { 0x1C, 1, 0, 3 },
    { 0x5C, 1, 0, 3 },
    { 0x94, 2, 1, 2 },
    { 0xB0, 1, 0, 2 },
    { 0xE0, 1, 0, 3 },
    { 0xF8, 0, 0, 1 },
    { 0x2C, 1, 0, 3 },
    { 0xB8, 1, 1, 2 },
    { 0xDC, 1, 0, 3 },
};

struct SICLiteral {
    char type;
    vector<char> object_code;
    int address;
};

struct SICOpCode {
    string mnemonic;
    char opcode;
    char format;
    bool register_based;
    char operands;
};

struct SICSymbol {
    int address;
    bool is_macro;
    bool is_symbolic;
};

struct SICCodeLine {
    int address;
    string label;
    string mnemonic;
    string mnemonic_t;
    SICOpCode* opcode_ref;
    vector<string> operands;
    string operands_t;
    string comment;
    vector<char> object_code;
    vector<string> errors;
    bool is_indexed;
    bool is_comment;
    bool is_xe4;

    bool is_indirect;
    bool is_immediate;
    bool is_immediate_hex;

    bool is_variable;
    bool is_literal;

    bool expr_operands;

    string block;
};

