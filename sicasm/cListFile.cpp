#include "cListFile.h"

#include <cctype>
#include <climits>

#define zero(o, s) memset(o, 0, s);

#define ERROR_MISSING_OPCODE  "missing operation code"
#define ERROR_UNRECOG_OPCODE  "unrecognized operation code"
#define ERROR_MISSING_OPERAND "missing or misplaced operand in mnemonic"
#define ERROR_ILLEGAL_OPERAND "illegal operand in mnemonic"
#define ERROR_UNDEFINED_SYM   "undefined symbol in operand"
#define ERROR_MISSING_START   "missing or misplaced start statement"
#define ERROR_ILLEGAL_START   "illegal operand in start statement"
#define ERROR_DUPLICATE_START "duplicate or misplaced start statement"
#define ERROR_OPERAND_END     "missing or misplaced operand in end statement"
#define ERROR_AFTER_END       "statement should not follow end statement"
#define ERROR_OPERAND_WORD    "missing or misplaced operand in word statement"
#define ERROR_ILLEGAL_WORD    "illegal operand in word statement"
#define ERROR_OPERAND_RESW    "missing or misplaced operand in resw statement"
#define ERROR_ILLEGAL_RESW    "illegal operand in resw statement"
#define ERROR_OPERAND_RESB    "missing or misplaced operand in resb statement"
#define ERROR_ILLEGAL_RESB    "illegal operand in resb statement"
#define ERROR_OPERAND_BYTE    "missing or misplaced operand in byte statement"
#define ERROR_ILLEGAL_BYTE    "illegal operand in byte statement"
#define ERROR_DUPLICATE_LABEL "duplicate label definition"
#define ERROR_ILLEGAL_LABEL   "illegal format in label field"


cListFile::cListFile(char* filename) : cSourceFile(filename) {
    _end_set = false;
    _start_set = false;

    ListFileOpCode* opcode;
    for (int i = 0; i < 59; ++i)
    {
        opcode = new ListFileOpCode;
        zero(opcode, sizeof(ListFileOpCode));

        opcode->mnemonic = opcodes_mnemonic[i];
        opcode->opcode = opcodes_format[i][0];
        opcode->operands = opcodes_format[i][1];
        opcode->register_based = opcodes_format[i][2] ? true : false;

        _opcodes_table[opcodes_mnemonic[i]] = opcode;
    }

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

    if (_siccode_lines[0]->mnemonic == "START") {
        if (is_hex_number(_siccode_lines[0]->operand)) {
            _start_address = _current_address = 
                hex_to_int((char*)_siccode_lines[0]->operand.c_str());
            _siccode_lines[0]->address = _start_address;
            _start_set = true;
        }
        else {
            _start_address = _current_address = 0;
            _siccode_lines[0]->errors.push_back(ERROR_ILLEGAL_START);
        }
    }
    else {
        _start_address = _current_address = 0;
        _siccode_lines[0]->errors.push_back(ERROR_MISSING_START);
    }

    map<string, ListFileOpCode*>::iterator opcode_table_it;
    SICCodeLine* siccode_line;

    for (int i = _start_set?1:0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        if (siccode_line->mnemonic.size()) {

            /* Handling START Directive */
            if (siccode_line->mnemonic == "START") {
                siccode_line->errors.push_back(ERROR_DUPLICATE_START);
            }

            /* Handling WORD Directive */
            else if (siccode_line->mnemonic == "WORD") {
                siccode_line->address = _current_address;
                _current_address += 3;

                if (!is_word_str(siccode_line->operand)) {
                    siccode_line->errors.push_back(ERROR_ILLEGAL_WORD);
                }
            }

            /* Handling BYTE Directive */
            else if (siccode_line->mnemonic == "BYTE") {
                siccode_line->address = _current_address;

                if (siccode_line->operand[0] == 'C') {
                    _current_address += (siccode_line->operand.size() - 3);
                }
                else if (siccode_line->operand[0] == 'X') {
                    _current_address += 1;
                }
                else {
                    siccode_line->errors.push_back(ERROR_ILLEGAL_BYTE);
                }
            }

            /* Handling RESW Directive */
            else if (siccode_line->mnemonic == "RESW") {
                siccode_line->address = _current_address;

                if (!is_word_str(siccode_line->operand)) {
                    siccode_line->errors.push_back(ERROR_ILLEGAL_RESW);
                    _current_address += 3;
                }
                else {
                    _current_address += (3 * str_to_int(
                        (char*)siccode_line->operand.c_str()));
                }
            }

            /* Handling RESB Directive */
            else if (siccode_line->mnemonic == "RESB") {
                siccode_line->address = _current_address;

                if (!is_word_str(siccode_line->operand)) {
                    siccode_line->errors.push_back(ERROR_ILLEGAL_RESB);
                    _current_address += 1;
                }
                else {
                    _current_address += str_to_int(
                        (char*)siccode_line->operand.c_str());
                }
            }

            /* Handling END Directive */
            else if (siccode_line->mnemonic == "END") {
                siccode_line->address = _current_address;
                _current_address += 1;
                _end_set = true;

                if (_symbols_table.count(siccode_line->operand) != 1) {
                    siccode_line->errors.push_back(ERROR_UNDEFINED_SYM);
                }
            }

            /* Handling Opcodes */
            else if ((opcode_table_it = _opcodes_table.find(
                siccode_line->mnemonic)) != _opcodes_table.end()) {
                siccode_line->address = _current_address;
                _current_address += 3;

                switch (opcode_table_it->second->operands)
                {
                case 0:
                    if (siccode_line->operand.size()) {
                        siccode_line->errors.push_back(
                            ERROR_ILLEGAL_OPERAND);
                    }
                    break;
                case 1:
                    if (siccode_line->operand.size()) {
                        if (siccode_line->operand.find(',') != -1) {
                            if (siccode_line->operand.find_last_of(',')
                                != siccode_line->operand.find(',') ||
                                siccode_line->operand.find(',') + 2
                                != siccode_line->operand.size() ||
                                siccode_line->operand[
                                    siccode_line->operand.find(',') + 1]
                                != 'X') {
                                siccode_line->errors.push_back(
                                    ERROR_ILLEGAL_OPERAND);
                            }
                        }
                        else if (_symbols_table.count(
                            siccode_line->operand) != 1) {
                            siccode_line->errors.push_back(
                                ERROR_UNDEFINED_SYM);
                        }
                    }
                    else {
                        siccode_line->errors.push_back(
                            ERROR_MISSING_OPERAND);
                    }
                    break;
                case 2:
                    if (siccode_line->operand.find_last_of(',') != -1 &&
                        siccode_line->operand.find_last_of(',')
                        == siccode_line->operand.find(',')) {
                        if (siccode_line->mnemonic == "SHIFTL" ||
                            siccode_line->mnemonic == "SHIFTR") {

                        }
                        else {

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
            }

            /* Handling Instructions after END statement */
            if (_end_set && siccode_line->mnemonic != "END") {
                siccode_line->errors.push_back(ERROR_AFTER_END);
            }
        }
    }

    return true;
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
    for (int i = 0; i < (int)str.size(); ++i)
    if (!isalpha(str[i]))
        return false;
    return true;
}

bool cListFile::is_hex_number(string& str) {
    return str.size() ? hex_to_int((char*)str.c_str()) == -1 ? false : true: false;
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

void cListFile::print_listfile() {
    SICCodeLine* siccode_line;
    for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        printf("%04x %-9s %-8s %-9s %-10s\n",
            siccode_line->address,
            siccode_line->label.c_str(),
            siccode_line->mnemonic.c_str(),
            siccode_line->operand.c_str(),
            siccode_line->comment.c_str());

        for (int j = 0; j < (int)siccode_line->errors.size(); ++j) {
            printf(" **** %s\n", siccode_line->errors[j].c_str());
        }
    }
}

cListFile::~cListFile() {
}
