#pragma once
#include "sicasm.h"
#include "cSourceFile.h"

using namespace std;

static char* opcodes_mnemonic[59] = {
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

static unsigned char opcodes_format[59][3] = {
    { 0x18, 1, 0},
    { 0x58, 1, 0},
    { 0x90, 2, 1},
    { 0x40, 1, 0},
    { 0xB4, 1, 1},
    { 0x28, 1, 0},
    { 0x88, 1, 0},
    { 0xA0, 2, 1},
    { 0x24, 1, 0},
    { 0x64, 1, 0},
    { 0x9C, 2, 1},
    { 0xC4, 0, 0},
    { 0xC0, 0, 0},
    { 0xF4, 0, 0},
    { 0x3C, 1, 0},
    { 0x30, 1, 0},
    { 0x34, 1, 0},
    { 0x38, 1, 0},
    { 0x48, 1, 0},
    { 0x00, 1, 0},
    { 0x68, 1, 0},
    { 0x50, 1, 0},
    { 0x70, 1, 0},
    { 0x08, 1, 0},
    { 0x6C, 1, 0},
    { 0x74, 1, 0},
    { 0x04, 1, 0},
    { 0xD0, 1, 0},
    { 0x20, 1, 0},
    { 0x60, 1, 0},
    { 0x98, 2, 1},
    { 0xC8, 0, 0},
    { 0x44, 1, 0},
    { 0xD8, 1, 0},
    { 0xAC, 2, 1},
    { 0x4C, 0, 0},
    { 0xA4, 2, 1},
    { 0xA8, 2, 1},
    { 0xF0, 0, 0},
    { 0xEC, 1, 0},
    { 0x0C, 1, 0},
    { 0x78, 1, 0},
    { 0x54, 1, 0},
    { 0x80, 1, 0},
    { 0xD4, 1, 0},
    { 0x14, 1, 0},
    { 0x7C, 1, 0},
    { 0xE8, 1, 0},
    { 0x84, 1, 0},
    { 0x10, 1, 0},
    { 0x1C, 1, 0},
    { 0x5C, 1, 0},
    { 0x94, 2, 1},
    { 0xB0, 1, 0},
    { 0xE0, 1, 0},
    { 0xF8, 0, 0},
    { 0x2C, 1, 0},
    { 0xB8, 1, 1},
    { 0xDC, 1, 0},
};

enum ListFileErrors {
    
};

struct ListFileOpCode {
    string mnemonic;
    char opcode;
    char format;
    bool register_based;
    char operands;
};


class cListFile: public cSourceFile {
public:
    cListFile(char* filename);
    ~cListFile();

    void print_listfile();

private:
    bool parse_siccode_lines();
    int hex_to_int(char* hex);
    int str_to_int(char* str);

    bool _end_set;
    bool _start_set;
    int _current_address;

    bool is_numeric(string& str);
    bool is_hex_number(string& str);
    bool is_word_str(string& str);
    bool is_alpha(string& str);
    void construct_symbol_table();
    bool parse_instructions();

protected:
    int _start_address;
    map<string, int> _symbols_table;
    map<string, ListFileOpCode*> _opcodes_table;
};

