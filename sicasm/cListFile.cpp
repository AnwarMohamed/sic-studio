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

cListFile::cListFile(char* filename) : cSourceFile(filename) {
    _end_set = false;
    _start_set = false;

    is_ready = is_ready ? parse_siccode_lines() : false;
}

void cListFile::construct_symbol_table() {
    SICCodeLine* siccode_line;

    for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        if (siccode_line->label.size()) {
            if (is_alpha(siccode_line->label)) {
                if (_symbols_table.count(siccode_line->label) == 0) {
                    _symbols_table[siccode_line->label] = i;

                    if (siccode_line->mnemonic == "RESB" ||
                        siccode_line->mnemonic == "RESW" ||
                        siccode_line->mnemonic == "BYTE" ||
                        siccode_line->mnemonic == "WORD") {
                        siccode_line->is_variable = true;
                    }
                }
                else {
                    siccode_line->errors.push_back(ERROR_DUPLICATE_LABEL);
                }
            }
            else {
                siccode_line->errors.push_back(ERROR_ILLEGAL_LABEL);
            }
        }
    }
}

bool cListFile::parse_instructions() {
    if (!_siccode_lines.size()) {
        return false;
    }

    _start_address = _current_address = 0;

    map<string, SICOpCode*>::iterator opcode_table_it;
    SICCodeLine* siccode_line;
    bool warn_no_start = false;

    for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        if (siccode_line->is_comment) {
            continue;
        }

        /* Handling START Directive */
        if (siccode_line->mnemonic == "START") {
            if (_start_set) {
                siccode_line->errors.push_back(ERROR_DUPLICATE_START);
            }
            else {
                if (siccode_line->operands.size() == 1 &&
                    is_hex_number(siccode_line->operands[0])) {
                    _start_address = _current_address =
                        hex_to_int((char*)siccode_line->operands[0].c_str());
                    siccode_line->address = _start_address;
                    _start_set = true;
                    _program_name = siccode_line->label;
                }
                else {
                    _start_address = _current_address = 0;
                    siccode_line->errors.push_back(ERROR_ILLEGAL_START);
                }
            }
        }

        /* Handling WORD Directive */
        else if (siccode_line->mnemonic == "WORD") {
            siccode_line->address = _current_address;            
            _current_address += 3;

            if (siccode_line->operands.size() != 1 ||
                !is_word_str(siccode_line->operands[0])) {
                siccode_line->errors.push_back(ERROR_ILLEGAL_WORD);
            }
        }

        /* Handling BYTE Directive */
        else if (siccode_line->mnemonic == "BYTE") {
            siccode_line->address = _current_address;

            if (siccode_line->operands.size() == 1 &&
                siccode_line->operands[0].size() >= 3 &&
                siccode_line->operands[0][0] == 'C' &&
                siccode_line->operands[0][1] == '\'' &&
                siccode_line->operands[0][
                    siccode_line->operands[0].size() - 1] == '\'') {
                _current_address += (siccode_line->operands.size() - 3);
            }
            else if (siccode_line->operands.size() == 1 &&
                is_hex_byte(siccode_line->operands[0])) {
                _current_address +=
                    (siccode_line->operands[0].size() - 3) / 2;
            }
            else {
                siccode_line->errors.push_back(ERROR_ILLEGAL_BYTE);
            }
        }

        /* Handling RESW Directive */
        else if (siccode_line->mnemonic == "RESW") {
            siccode_line->address = _current_address;

            if (siccode_line->operands.size() != 1 ||
                !is_word_str(siccode_line->operands[0])) {
                siccode_line->errors.push_back(ERROR_ILLEGAL_RESW);
                _current_address += 3;
            }
            else {
                _current_address += (3 * str_to_int(
                    (char*)siccode_line->operands[0].c_str()));
            }
        }

        /* Handling RESB Directive */
        else if (siccode_line->mnemonic == "RESB") {
            siccode_line->address = _current_address;

            if (siccode_line->operands.size() != 1 ||
                !is_word_str(siccode_line->operands[0])) {
                siccode_line->errors.push_back(ERROR_ILLEGAL_RESB);
                _current_address += 1;
            }
            else {
                _current_address += str_to_int(
                    (char*)siccode_line->operands[0].c_str());
            }
        }

        /* Handling END Directive */
        else if (siccode_line->mnemonic == "END") {
            siccode_line->address = _current_address;
            _current_address += 1;
            _end_address = siccode_line->address;
            _end_set = true;

            if (siccode_line->operands.size() != 1 ||
                _symbols_table.count(siccode_line->operands[0]) != 1) {
                siccode_line->errors.push_back(ERROR_UNDEFINED_SYM);

                if (_start_set){
                    for (map<string, int>::iterator j = _symbols_table.begin();
                        j != _symbols_table.end(); ++j) {
                        if (_siccode_lines[j->second]->mnemonic == "START" &&
                            _siccode_lines[j->second]->label.size()) {
                            siccode_line->errors.push_back(
                                suggest_operation(
                                _siccode_lines[j->second]->label));
                            break;
                        }
                    }
                }

            }
        }

		/* Handling BASE, NOBASE Directives */
		else if (siccode_line->mnemonic == "BASE" ||
			siccode_line->mnemonic == "NOBASE") {
			siccode_line->address = _current_address;

			if (siccode_line->operands.size()) {
				siccode_line->errors.push_back(ERROR_ILLEGAL_BASE);
			}
		}

        /* Handling Opcodes */
        else if ((opcode_table_it = _opcodes_table.find(
            siccode_line->mnemonic)) != _opcodes_table.end()) {

            if (siccode_line->mnemonic_t == "+") {
                siccode_line->is_xe4 = true;
            }

            siccode_line->address = _current_address;
            siccode_line->opcode_ref = opcode_table_it->second;
            _current_address += 3;

            if (siccode_line->is_xe4 &&
                siccode_line->opcode_ref->format != 3) {
                siccode_line->errors.push_back(ERROR_ILLEGAL_OPCODE_V3);
            }

            switch (opcode_table_it->second->operands)
            {
            case 0:
                if (siccode_line->operands.size()) {
                    siccode_line->errors.push_back(
                        ERROR_ILLEGAL_OPERAND);
                }
                break;
            case 1:
                if (siccode_line->operands.size()) {
                    if (siccode_line->operands.size() == 2) {
                        if (siccode_line->operands[1] == "X") {
                            siccode_line->is_indexed = true;

                            if (siccode_line->operands_t == "@") {                                
                                if (_symbols_table.count(
									siccode_line->operands[0]) == 1 &&
                                    _siccode_lines[_symbols_table[
                                        siccode_line->operands[0]]
                                    ]->is_variable) {
                                    siccode_line->is_indirect = true;
                                }
                                else {
                                    siccode_line->errors.push_back(
                                        ERROR_UNDEFINED_SYM);
                                }
                            }
                            else if (siccode_line->operands_t == "#") {
                                if (_symbols_table.count(
                                    siccode_line->operands[0]) == 1 &&
                                    _siccode_lines[_symbols_table[
                                        siccode_line->operands[0]]
                                    ]->is_variable) {
                                    siccode_line->is_indirect = true;
                                }
                                else if (is_hex_word(
									siccode_line->operands[0])) {
                                }
                                else {
                                    siccode_line->errors.push_back(
                                        ERROR_UNDEFINED_SYM);
                                }
                            }
                            else if (is_hex_number(
                                siccode_line->operands[0])) {
								
                            }
                            else {
                                if (_symbols_table.count(
                                    siccode_line->operands[0]) != 1) {
                                    siccode_line->errors.push_back(
                                        ERROR_UNDEFINED_SYM);
                                }
                            }
                        }
                        else {
                            siccode_line->errors.push_back(
                                ERROR_ILLEGAL_OPERAND);
                        }
                    }
                    else if (siccode_line->operands.size() == 1) {
                        if (siccode_line->is_xe4) {
                            if (_symbols_table.count(
                                siccode_line->operands[0]) != 1) {
                                siccode_line->errors.push_back(
                                    ERROR_UNDEFINED_SYM);
                            }
                        }
                        else if (is_hex_number(
                            siccode_line->operands[0])) {
                            siccode_line->is_immediate_hex = true;
                        }
                        else if (siccode_line->operands_t == "#") {
                            siccode_line->is_immediate = true;

                            if (is_word_str(
                                siccode_line->operands[0])) {
                            }
                            else if (is_hex_word(
                                siccode_line->operands[0])) {

                            }
                            else {
                                siccode_line->errors.push_back(
                                    ERROR_UNDEFINED_SYM);
                            }
                        }
                        else if (siccode_line->operands_t == "@") {
                            siccode_line->is_indirect = true;

                            if (_symbols_table.count(
                                siccode_line->operands[0]) == 1 &&
                                _siccode_lines[_symbols_table[
                                    siccode_line->operands[0]]
                                ]->is_variable) {

                            }
                            else {
                                siccode_line->errors.push_back(
                                    ERROR_UNDEFINED_SYM);
                            }
                        }
                        else {
                            if (_symbols_table.count(
                                siccode_line->operands[0]) != 1) {
                                siccode_line->errors.push_back(
                                    ERROR_UNDEFINED_SYM);
                            }
                        }
                    }
                    else {
                        siccode_line->errors.push_back(
                            ERROR_ILLEGAL_OPERAND);
                    }
                }
                else {
                    siccode_line->errors.push_back(
                        ERROR_MISSING_OPERAND);
                }
                break;
            case 2:
                if (siccode_line->operands.size() == 2) {
                    if ((siccode_line->mnemonic == "SHIFTL" ||
                        siccode_line->mnemonic == "SHIFTR") &&
						encode_register(siccode_line->operands[0]) != -1 &&
						str_to_int(
							(char*)siccode_line->operands[1].c_str()) < 16) {

                    }
					else if (encode_register(siccode_line->operands[0]) != -1 &&
						encode_register(siccode_line->operands[1]) != -1) {

					}
					else {
						siccode_line->errors.push_back(
							ERROR_ILLEGAL_OPERAND);
					}
                }
                else {
                    siccode_line->errors.push_back(
                        ERROR_ILLEGAL_OPERAND);
                }
                break;
            }
        }

        /* Handling Unknown Opcodes */
        else {
            siccode_line->address = _current_address;
            siccode_line->errors.push_back(ERROR_UNRECOG_OPCODE);

            if (starts_with(siccode_line->mnemonic, "STAR") ||
                starts_with(siccode_line->mnemonic, "TART") ||
                starts_with(siccode_line->mnemonic, "STRT")) {
                siccode_line->errors.push_back(suggest_operation("START"));
            }
            else if (siccode_line->mnemonic == "ND" ||
                siccode_line->mnemonic == "EN") {
                siccode_line->errors.push_back(suggest_operation("END"));
            }
            else if (starts_with(siccode_line->mnemonic, "RES") ||
                starts_with(siccode_line->mnemonic, "RS") ||
                starts_with(siccode_line->mnemonic, "ES") ||
                starts_with(siccode_line->mnemonic, "RE")) {
                siccode_line->errors.push_back(suggest_operation("RESW\' or \'RESB"));
            }
            else if (starts_with(siccode_line->mnemonic, "ORD") ||
                starts_with(siccode_line->mnemonic, "WRD") ||
                starts_with(siccode_line->mnemonic, "WOR")) {
                siccode_line->errors.push_back(suggest_operation("WORD"));
            }
            else if (starts_with(siccode_line->mnemonic, "YTE") ||
                starts_with(siccode_line->mnemonic, "BYT") ||
                starts_with(siccode_line->mnemonic, "BTE")) {
                siccode_line->errors.push_back(suggest_operation("BYTE"));
            }
            else if (starts_with(siccode_line->mnemonic, "YTE") ||
                starts_with(siccode_line->mnemonic, "BYT") ||
                starts_with(siccode_line->mnemonic, "BTE")) {
                siccode_line->errors.push_back(suggest_operation("BYTE"));
            }
            else if (_start_set)    {
                for (int j = 0; j < OPCODES_SIZE; ++j) {
                    if (starts_with(opcodes_mnemonic[j],
                        siccode_line->mnemonic) ||
                        starts_with(siccode_line->mnemonic,
                        opcodes_mnemonic[j])) {
                        siccode_line->errors.push_back(
                            suggest_operation(opcodes_mnemonic[j]));
                        break;
                    }
                }
            }
        }

        /* Handling Instructions after END statement */
        if (_end_set && siccode_line->mnemonic != "END") {
            siccode_line->errors.push_back(ERROR_AFTER_END);
        }

        if (_current_address && !warn_no_start && !_start_set) {
            warn_no_start = true;
            _start_address = _current_address = 0;
            _siccode_lines[0]->errors.push_back(ERROR_MISSING_START);
        }
    }

    return true;
}

bool cListFile::starts_with(const string& original, const string& checkable) {
    if (checkable.length() > original.length()) {
        return false;
    }

    for (int i = 0; i < (int)checkable.length(); ++i) {
        if (checkable[i] != original[i]) {
            return false;
        }
    }

    return true;
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
