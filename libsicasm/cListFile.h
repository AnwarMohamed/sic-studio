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
#include "sicasm.h"
#include "cSourceFile.h"

using namespace std;

class DLLEXPORT cListFile : public cSourceFile {
public:
    cListFile(char* filename);
    ~cListFile();

    virtual void print_listfile(FILE* file = 0);
private:
    bool parse_siccode_lines();

    bool _end_set;
    bool _start_set;
    int _current_address;
    int _address_offset;


    bool is_hex_number(string& str);
    bool is_word_str(string& str);
    bool is_alpha(string& str);
    bool is_hex_byte(string& str);
    bool is_hex_word(string& str);

    void construct_symbol_table();
    bool parse_instructions();
    string suggest_operation(string operation);

    void handle_start_directive(SICCodeLine* code);
    void handle_word_directive(SICCodeLine* code);
    void handle_byte_directive(SICCodeLine* code);
    void handle_resb_directive(SICCodeLine* code);
    void handle_resw_directive(SICCodeLine* code);
    void handle_end_directive(SICCodeLine* code);
    void handle_equ_directive(SICCodeLine* code);
    void handle_org_directive(SICCodeLine* code);
    int handle_literal_directive(SICCodeLine* code, int index);
    void suggest_end_operand(SICCodeLine* code);
    void handle_opcodes(SICCodeLine* code);
    void handle_unknown_opcodes(SICCodeLine* code);
    void handle_opcodes_single_operand(SICCodeLine* code);
    void handle_opcodes_dual_operands(SICCodeLine* code);
    void handle_indexed_operand(SICCodeLine* code);
    void handle_literal(SICCodeLine* code);
    void handle_symbol_expression(SICCodeLine* code, SICSymbol* symbol);
    int get_value_from_expression(string& operand);

    SICLiteral* generate_hex_literal(SICCodeLine* code);
    SICLiteral* generate_char_literal(SICCodeLine* code);

    bool is_register(int number);
    bool is_byte(int number);

protected:
    int _start_address;
    int _end_address;
    string _program_name;

    map<string, SICSymbol*> _symbols_table;
    map<string, SICLiteral*> _literals_table;

    int str_to_int(char* str);
    int hex_to_int(char* hex);
    bool is_numeric(string& str);

    char encode_register(string &reg);

    string merge_operands(vector<string> &operands);
    bool starts_with(const string& haystack, const string& needle);

    char is_byte_instruction(string& inst);
    int get_symbol_value(string& key);
};

