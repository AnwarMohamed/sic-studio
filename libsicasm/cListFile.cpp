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

#include "cListFile.h"

#include <cctype>
#include <climits>

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

#define BYTE_INSTRUCTION_INVALID	0x00
#define BYTE_INSTRUCTION_HEX		0x01
#define BYTE_INSTRUCTION_CHAR		0x02

cListFile::cListFile(char* filename) : cSourceFile(filename) {
    _end_set = false;
    _start_set = false;
    _address_offset = 0;

    is_ready = is_ready ? parse_siccode_lines() : false;
}

void cListFile::construct_symbol_table() {
    SICCodeLine* siccode_line;
    SICSymbol* symbol;

    for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        if (siccode_line->label.size()) {
            if (is_alpha(siccode_line->label)) {
                if (_symbols_table.count(siccode_line->label) == 0) {

                    if (siccode_line->mnemonic == "EQU") {
                        handle_equ_directive(siccode_line);
                    }
                    else {
                        symbol = new SICSymbol;
                        zero(symbol, sizeof(SICSymbol));
                        symbol->address = i;

                        _symbols_table[siccode_line->label] = symbol;

                        if (siccode_line->mnemonic == "RESB" ||
                            siccode_line->mnemonic == "RESW" ||
                            siccode_line->mnemonic == "BYTE" ||
                            siccode_line->mnemonic == "WORD") {
                            siccode_line->is_variable = true;
                        }
                    }
                }
                else
                    siccode_line->errors.push_back(ERROR_DUPLICATE_LABEL);
            }
            else
                siccode_line->errors.push_back(ERROR_ILLEGAL_LABEL);
        }
    }
}

void cListFile::handle_start_directive(SICCodeLine* code) {
    if (_start_set)
        code->errors.push_back(ERROR_DUPLICATE_START);

    else if (code->operands.size() == 1 &&
        is_hex_number(code->operands[0])) {
        _start_address = _current_address =
            hex_to_int((char*)code->operands[0].c_str());
        code->address = _start_address;
        _start_set = true;
        _program_name = code->label;
    }
    else {
        _start_address = _current_address = 0;
        code->errors.push_back(ERROR_ILLEGAL_START);
    }
}

void cListFile::handle_word_directive(SICCodeLine* code) {
    code->address = _current_address;
    _current_address += 3;

    if (code->operands.size() != 1 ||
        !is_word_str(code->operands[0])) {
        code->errors.push_back(ERROR_ILLEGAL_WORD);
    }
}


void cListFile::handle_byte_directive(SICCodeLine* code) {
    code->address = _current_address;

    if (code->operands.size() == 1) {
        switch (is_byte_instruction(code->operands[0])) {
        case BYTE_INSTRUCTION_INVALID:
            code->errors.push_back(ERROR_ILLEGAL_BYTE);
            break;
        case BYTE_INSTRUCTION_HEX:
            _current_address +=
                (code->operands[0].size() - 3) / 2;
            break;
        case BYTE_INSTRUCTION_CHAR:
            _current_address += (code->operands.size() - 3);
            break;
        }
    }
}

void cListFile::handle_resb_directive(SICCodeLine* code) {
    code->address = _current_address;

    if (code->operands.size() != 1 ||
        !is_word_str(code->operands[0])) {
        code->errors.push_back(ERROR_ILLEGAL_RESB);
        _current_address += 1;
    }
    else {
        _current_address += str_to_int(
            (char*)code->operands[0].c_str());
    }
}

void cListFile::handle_resw_directive(SICCodeLine* code) {
    code->address = _current_address;

    if (code->operands.size() != 1 ||
        !is_word_str(code->operands[0])) {
        code->errors.push_back(ERROR_ILLEGAL_RESW);
        _current_address += 3;
    }
    else {
        _current_address += (3 * str_to_int(
            (char*)code->operands[0].c_str()));
    }
}

void cListFile::suggest_end_operand(SICCodeLine* code) {
    for (map<string, SICSymbol*>::iterator j = _symbols_table.begin();
        j != _symbols_table.end(); ++j) {
        if (_siccode_lines[j->second->address]->mnemonic == "START" &&
            _siccode_lines[j->second->address]->label.size()) {
            code->errors.push_back(
                suggest_operation(
                _siccode_lines[j->second->address]->label));
            break;
        }
    }
}

void cListFile::handle_end_directive(SICCodeLine* code) {
    code->address = _current_address;
    _end_address = code->address;
    _end_set = true;

    if (code->operands.size() != 1 ||
        _symbols_table.count(code->operands[0]) != 1) {
        code->errors.push_back(ERROR_UNDEFINED_SYM);

        if (_start_set){
            suggest_end_operand(code);
        }
    }
}

void cListFile::handle_indexed_operand(SICCodeLine* code) {
    code->is_indexed = true;

    if (code->operands_t == "@") {
        if (_symbols_table.count(code->operands[0]) == 1 &&
            _siccode_lines[_symbols_table[
                code->operands[0]]->address]->is_variable) {
            code->is_indirect = true;
        }
        else {
            code->errors.push_back(ERROR_UNDEFINED_SYM);
        }
    }
    else if (code->operands_t == "#") {
        if (_symbols_table.count(code->operands[0]) == 1 &&
            _siccode_lines[_symbols_table[
                code->operands[0]]->address]->is_variable) {
            code->is_indirect = true;
        }
        else if (is_hex_word(code->operands[0])) {

        }
        else {
            code->errors.push_back(ERROR_UNDEFINED_SYM);
        }
    }
    else if (is_hex_number(code->operands[0])) {

    }
    else if (_symbols_table.count(code->operands[0]) != 1) {
        code->errors.push_back(ERROR_UNDEFINED_SYM);
    }
}

void cListFile::handle_literal(SICCodeLine* code) {
    if (code->operands.size() == 1) {

        switch (is_byte_instruction(code->operands[0])) {
        case BYTE_INSTRUCTION_INVALID:
            code->errors.push_back(ERROR_ILLEGAL_LITERAL);
            break;
        case BYTE_INSTRUCTION_HEX:
            if (_literals_table.count(code->operands[0]) == 0)
                _literals_table[code->operands[0]] =
                generate_hex_literal(code);
            break;
        case BYTE_INSTRUCTION_CHAR:
            if (_literals_table.count(code->operands[0]) == 0)
                _literals_table[code->operands[0]] =
                generate_char_literal(code);
            break;
        }
    }
}

SICLiteral* cListFile::generate_hex_literal(SICCodeLine* code) {
    SICLiteral* literal = new SICLiteral;
    literal->address = -1;
    literal->type = BYTE_INSTRUCTION_HEX;

    string str_array =
        code->operands[0].substr(2, code->operands[0].size() - 2);

    int byte;
    for (int i = 0; i < (int)str_array.size() / 2; i++) {
        sscanf_s(str_array.substr(2 * i, 2 * i + 2).c_str(), "%02x", &byte);
        literal->object_code.push_back(byte & 0x000000ff);
    }

    map<string, SICLiteral*>::iterator lit_it;
    for (lit_it = _literals_table.begin();
        lit_it != _literals_table.end(); ++lit_it) {
        if (lit_it->second->object_code == literal->object_code) {
            delete literal;
            return lit_it->second;
        }
    }

    return literal;
}

SICLiteral* cListFile::generate_char_literal(SICCodeLine* code) {
    SICLiteral* literal = new SICLiteral;
    literal->address = -1;
    literal->type = BYTE_INSTRUCTION_CHAR;

    string str_array =
        code->operands[0].substr(2, code->operands[0].size() - 3);

    for (int j = 0; j < (int)str_array.size(); ++j)
        literal->object_code.push_back(str_array[j]);

    map<string, SICLiteral*>::iterator lit_it;
    for (lit_it = _literals_table.begin();
        lit_it != _literals_table.end(); ++lit_it) {
        if (lit_it->second->object_code == literal->object_code) {
            delete literal;
            return lit_it->second;
        }
    }

    return literal;
}

void cListFile::handle_opcodes_single_operand(SICCodeLine* code) {
    if (code->operands.size()) {
        if (code->operands.size() == 2) {
            if (code->operands[1] == "X")
                handle_indexed_operand(code);
            else
                code->errors.push_back(ERROR_ILLEGAL_OPERAND);
        }
        else if (code->operands.size() == 1) {
            if (code->is_xe4) {
                if (_symbols_table.count(code->operands[0]) != 1)
                    code->errors.push_back(ERROR_UNDEFINED_SYM);
            }
            else if (is_hex_number(code->operands[0]))
                code->is_immediate_hex = true;

            else if (code->operands_t == "#") {
                code->is_immediate = true;

                if (is_word_str(code->operands[0])) {
                }
                else if (is_hex_word(code->operands[0])) {
                }
                else
                    code->errors.push_back(ERROR_UNDEFINED_SYM);
            }
            else if (code->operands_t == "@") {
                code->is_indirect = true;

                if (_symbols_table.count(code->operands[0]) == 1 &&
                    _siccode_lines[
                        _symbols_table[
                            code->operands[0]]->address]->is_variable) {
                }
                else
                    code->errors.push_back(ERROR_UNDEFINED_SYM);
            }

            /* Handling Literals */
            else if (code->operands_t == "=")
                handle_literal(code);

            else if (_symbols_table.count(code->operands[0]) != 1)
                code->errors.push_back(ERROR_UNDEFINED_SYM);
        }
        else
            code->errors.push_back(ERROR_ILLEGAL_OPERAND);
    }
    else
        code->errors.push_back(ERROR_MISSING_OPERAND);
}

void cListFile::handle_opcodes_dual_operands(SICCodeLine* code) {
    if (code->operands.size() == 2) {
        if ((code->mnemonic == "SHIFTL" || code->mnemonic == "SHIFTR") &&
            is_register(get_value_from_expression(code->operands[0])) &&
            is_byte(get_value_from_expression(code->operands[1]))) {

        }
        else if (is_register(get_value_from_expression(code->operands[0])) &&
            is_register(get_value_from_expression(code->operands[1]))) {

        }
        else
            code->errors.push_back(ERROR_ILLEGAL_OPERAND);
    }
    else
        code->errors.push_back(ERROR_ILLEGAL_OPERAND);
}

bool cListFile::is_byte(int number) {
    return number >= 0 && number < 16;
}

bool cListFile::is_register(int number) {
    switch (number) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
        return true;
    default:
        return false;
    }
}

int cListFile::get_value_from_expression(string& operand) {
    if (is_numeric(operand))
        return str_to_int((char*)operand.c_str());

    else if (is_alpha(operand)) {
        if (_symbols_table.count(operand) == 1)
            return get_symbol_value(operand);
    }
    return -1;
}

void cListFile::handle_opcodes(SICCodeLine* code) {

    if (code->mnemonic_t == "+")
        code->is_xe4 = true;

    code->address = _current_address;
    _current_address += 3;

    if (code->is_xe4 && code->opcode_ref->format != 3)
        code->errors.push_back(ERROR_ILLEGAL_OPCODE_V3);


    switch (code->opcode_ref->operands) {
    case 0:
        if (code->operands.size())
            code->errors.push_back(ERROR_ILLEGAL_OPERAND);

        break;
    case 1:
        handle_opcodes_single_operand(code);
        break;
    case 2:
        handle_opcodes_dual_operands(code);
        break;
    }
}

void cListFile::handle_symbol_expression(SICCodeLine* code, SICSymbol* symbol) {
    if (is_numeric(code->operands[0])) {
        symbol->address = str_to_int((char*)code->operands[0].c_str());
        _symbols_table[code->label] = symbol;
    }
    else if (_symbols_table.count(code->operands[0]) == 1) {
        symbol->is_symbolic = true;
        symbol->address = get_symbol_value(code->operands[0]);
        _symbols_table[code->label] = symbol;
    }
    else {
        code->errors.push_back(ERROR_ILLEGAL_EQU);
        delete symbol;
    }
}

void cListFile::handle_equ_directive(SICCodeLine* code) {
    if (!code->label.size() || code->operands.size() != 1) {
        code->errors.push_back(ERROR_ILLEGAL_EQU);
        return;
    }
    else if (!is_alpha(code->label)) {
        code->errors.push_back(ERROR_ILLEGAL_LABEL);
        return;
    }

    SICSymbol* symbol = new SICSymbol;
    zero(symbol, sizeof(SICSymbol));
    symbol->is_macro = true;

    handle_symbol_expression(code, symbol);
}

void cListFile::handle_org_directive(SICCodeLine* code) {
    if (!code->operands.size()) {
        _current_address -= _address_offset;
        _address_offset = 0;
    }
    else if (code->operands.size() == 1 &&
        get_value_from_expression(code->operands[0]) >= 0) {
        _current_address -= _address_offset;
        _address_offset = get_value_from_expression(code->operands[0]);
        _current_address += _address_offset;
    }
    else
        code->errors.push_back(ERROR_ILLEGAL_ORG);
}

int cListFile::get_symbol_value(string& key) {
    return _symbols_table[key]->address;
}

bool cListFile::parse_instructions() {
    if (!_siccode_lines.size())
        return false;

    _start_address = _current_address = 0;

    map<string, SICOpCode*>::iterator opcode_table_it;
    SICCodeLine* siccode_line;
    bool warn_no_start = false;

    for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        _current_address += _address_offset;

        if (siccode_line->is_comment) {
            _current_address += _address_offset;
            continue;
        }

        /* Handling START Directive */
        if (siccode_line->mnemonic == "START")
            handle_start_directive(siccode_line);

        /* Handling WORD Directive */
        else if (siccode_line->mnemonic == "WORD")
            handle_word_directive(siccode_line);

        /* Handling BYTE Directive */
        else if (siccode_line->mnemonic == "BYTE")
            handle_byte_directive(siccode_line);

        /* Handling RESW Directive */
        else if (siccode_line->mnemonic == "RESW")
            handle_resw_directive(siccode_line);

        /* Handling RESB Directive */
        else if (siccode_line->mnemonic == "RESB")
            handle_resb_directive(siccode_line);

        /* Handling EQU Directive */
        else if (siccode_line->mnemonic == "EQU") {
            siccode_line->address = _current_address;
            handle_equ_directive(siccode_line);
        }

        /* Handling ORG Directive */
        else if (siccode_line->mnemonic == "ORG")
            handle_org_directive(siccode_line);

        /* Handling END Directive */
        else if (siccode_line->mnemonic == "END") {
            handle_end_directive(siccode_line);
            i += handle_literal_directive(siccode_line, i);
        }

        /* Handling LTORG Directive */
        else if (siccode_line->mnemonic == "LTORG") {
            siccode_line->address = _current_address;
            i += handle_literal_directive(siccode_line, i);
        }

        /* Handling BASE, NOBASE Directives */
        else if (siccode_line->mnemonic == "BASE" ||
            siccode_line->mnemonic == "NOBASE") {
            siccode_line->address = _current_address;

            if (siccode_line->operands.size())
                siccode_line->errors.push_back(ERROR_ILLEGAL_BASE);
        }

        /* Handling Opcodes */
        else if ((opcode_table_it = _opcodes_table.find(
            siccode_line->mnemonic)) != _opcodes_table.end()) {
            siccode_line->opcode_ref = opcode_table_it->second;
            handle_opcodes(siccode_line);
        }

        /* Handling Unknown Opcodes */
        else
            handle_unknown_opcodes(siccode_line);

        /* Handling Instructions after END statement */
        if (_end_set && siccode_line->mnemonic != "END")
            siccode_line->errors.push_back(ERROR_AFTER_END);


        if (_current_address && !warn_no_start && !_start_set) {
            warn_no_start = true;
            _start_address = _current_address = 0;
            _siccode_lines[0]->errors.push_back(ERROR_MISSING_START);
        }

        _current_address -= _address_offset;
    }

    return true;
}

int cListFile::handle_literal_directive(SICCodeLine* code, int index) {
    vector<SICCodeLine*>::iterator code_it;
    map<string, SICLiteral*>::iterator lit_it;
    SICLiteral* literal;
    SICCodeLine* line;
    int generated_sum = 0;

    for (lit_it = _literals_table.begin();
        lit_it != _literals_table.end(); ++lit_it) {

        literal = lit_it->second;
        if (literal->address == -1) {
            line = new SICCodeLine;
            zero(line, sizeof(SICCodeLine));

            code_it = _siccode_lines.begin() + index + 1 + generated_sum++;

            line->label = '*';
            line->mnemonic = lit_it->first;
            line->address = _current_address;
            line->is_literal = true;
            line->object_code = literal->object_code;
            _siccode_lines.insert(code_it, line);

            _current_address += literal->object_code.size();
            literal->address = line->address;
        }
    }

    return generated_sum;
}

void cListFile::handle_unknown_opcodes(SICCodeLine* code) {
    code->address = _current_address;
    code->errors.push_back(ERROR_UNRECOG_OPCODE);

    if (starts_with(code->mnemonic, "STAR") ||
        starts_with(code->mnemonic, "TART") ||
        starts_with(code->mnemonic, "STRT"))
        code->errors.push_back(suggest_operation("START"));

    else if (code->mnemonic == "ND" ||
        code->mnemonic == "EN")
        code->errors.push_back(suggest_operation("END"));

    else if (starts_with(code->mnemonic, "RES") ||
        starts_with(code->mnemonic, "RS") ||
        starts_with(code->mnemonic, "ES") ||
        starts_with(code->mnemonic, "RE"))
        code->errors.push_back(suggest_operation("RESW\' or \'RESB"));

    else if (starts_with(code->mnemonic, "ORD") ||
        starts_with(code->mnemonic, "WRD") ||
        starts_with(code->mnemonic, "WOR"))
        code->errors.push_back(suggest_operation("WORD"));

    else if (starts_with(code->mnemonic, "YTE") ||
        starts_with(code->mnemonic, "BYT") ||
        starts_with(code->mnemonic, "BTE"))
        code->errors.push_back(suggest_operation("BYTE"));

    else if (starts_with(code->mnemonic, "YTE") ||
        starts_with(code->mnemonic, "BYT") ||
        starts_with(code->mnemonic, "BTE"))
        code->errors.push_back(suggest_operation("BYTE"));

    else if (_start_set)    {
        for (int j = 0; j < OPCODES_SIZE; ++j) {
            if (starts_with(opcodes_mnemonic[j], code->mnemonic) ||
                starts_with(code->mnemonic, opcodes_mnemonic[j])) {
                code->errors.push_back(
                    suggest_operation(opcodes_mnemonic[j]));
                break;
            }
        }
    }
}

bool cListFile::starts_with(const string& original, const string& checkable) {
    if (checkable.length() > original.length())
        return false;

    for (int i = 0; i < (int)checkable.length(); ++i)
    if (checkable[i] != original[i])
        return false;

    return true;
}

char cListFile::is_byte_instruction(string& inst) {
    if (inst.size() >= 3 &&
        inst[1] == '\'' && inst[inst.size() - 1] == '\'') {
        if (inst[0] == 'X' && inst.size() % 2 == 1) {

            for (int i = 2; i < (int)inst.size() - 1; ++i) {
                if (!((inst[i] >= 'A' && inst[i] <= 'F') ||
                    (inst[i] >= 'a' && inst[i] <= 'f') ||
                    (inst[i] >= '0' && inst[i] <= '9'))) {
                    return BYTE_INSTRUCTION_INVALID;
                }
            }
            return BYTE_INSTRUCTION_HEX;
        }
        else if (inst[0] == 'C') {
            return BYTE_INSTRUCTION_CHAR;
        }
    }

    return BYTE_INSTRUCTION_INVALID;
}

bool cListFile::is_hex_word(string& str) {
    return is_hex_byte(str) && ((str.size() - 3) / 2 <= 3);
}

bool cListFile::is_hex_byte(string& str) {
    if (str.size() >= 5 && str.size() % 2 == 1 &&
        str[0] == 'X' && str[1] == '\'' &&
        str[str.size() - 1] == '\'') {

        for (int i = 2; i < (int)str.size() - 1; ++i) {
            if ((str[i] >= 'A' && str[i] <= 'F') ||
                (str[i] >= 'a' && str[i] <= 'f') ||
                (str[i] >= '0' && str[i] <= '9')) {

            }
            else {
                return false;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

string cListFile::suggest_operation(string operation) {
    return string(ERROR_SUGGEST) + " \'" + operation + "\'?";
}

bool cListFile::parse_siccode_lines() {
    construct_symbol_table();
    return parse_instructions();
}

bool cListFile::is_word_str(string& str) {
    return is_numeric(str) && (str_to_int((char*)str.c_str()) != -1 ? true : false);
}

bool cListFile::is_numeric(string& str) {
    for (int i = 0; i < (int)str.size(); ++i)
    if (!isdigit(str[i]))
        return false;
    return true;
}

bool cListFile::is_alpha(string& str) {
    bool has_alpha = false;
    for (int i = 0; i < (int)str.size(); ++i) {
        if (i == 0 && isdigit(str[i]))
            return false;
        if (!isalnum(str[i]))
            return false;
        if (isalpha(str[i]))
            has_alpha = true;
    }
    return has_alpha;
}

bool cListFile::is_hex_number(string& str) {
    for (int i = 0; i < (int)str.size(); ++i) {
        if ((str[i] >= 'A' && str[i] <= 'F') ||
            (str[i] >= 'a' && str[i] <= 'f') ||
            (str[i] >= '0' && str[i] <= '9')) {
        }
        else {
            return false;
        }
    }
    return str.size() ? (hex_to_int((char*)str.c_str()) == -1 ? false : true) : false;
}

int cListFile::str_to_int(char* str) {
    int result;
    sscanf_s(str, "%d", &result);
    return (unsigned int)result > 0x00ffffff ? -1 : result;
}

int cListFile::hex_to_int(char* hex) {
    int result;
    sscanf_s(hex, "%x", &result);
    return (unsigned int)result > 0x0000ffff ? -1 : result;
}

void cListFile::print_listfile(FILE* file) {
    if (file) {
        fprintf(file,
            "SIC/XE Assembler\n"
            "By Anwar Mohamed ~ 2491\n"
            "anwarelmakrahy@gmail.com\n\n");
    }

    SICCodeLine* siccode_line;
    for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        if (siccode_line->is_comment) {
            fprintf(file ? file : stdout,
                "     %s\n", siccode_line->comment.c_str());
        }
        else {
            fprintf(file ? file : stdout, "%04X %-8s %-7s %-18s %s\n",
                siccode_line->address,
                siccode_line->label.c_str(),
                (siccode_line->mnemonic_t + siccode_line->mnemonic).c_str(),
                (siccode_line->operands_t + merge_operands(siccode_line->operands)).c_str(),
                siccode_line->comment.c_str());

            for (int j = 0; j < (int)siccode_line->errors.size(); ++j) {
                fprintf(file ? file : stdout,
                    " **** %s\n", siccode_line->errors[j].c_str());
            }
        }
    }
}

string cListFile::merge_operands(vector<string> &operands) {
    string merged;

    if (operands.size() == 0) {

    }
    else if (operands.size() == 1) {
        merged = operands[0];
    }
    else {
        for (int i = 0; i < (int)operands.size(); ++i) {
            if (i != 0) {
                merged += ",";
            }
            merged += operands[i];
        }
    }
    return merged;
}

char cListFile::encode_register(string &reg) {
    if (reg.size() != 1) {
        return -1;
    }

    switch (reg[0]) {
    case 'A':
        return 0;
    case 'X':
        return 1;
    case 'L':
        return 2;
    case 'B':
        return 3;
    case 'S':
        return 4;
    case 'T':
        return 5;
    case 'F':
        return 6;
    default:
        return -1;
    }
}

cListFile::~cListFile() {
}
