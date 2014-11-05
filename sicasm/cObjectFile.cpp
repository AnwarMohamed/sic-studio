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

#include "cObjectFile.h"

cObjectFile::cObjectFile(char* filename): cListFile(filename) {
    generate_object_code();
}

void cObjectFile::generate_object_code() {
    SICCodeLine* siccode_line;
    for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        if (siccode_line->errors.size() == 0) {

            if (siccode_line->mnemonic == "BYTE") {
                if (siccode_line->operand[0] == 'C') {
                    string str_array = siccode_line->operand.substr(
                        2, siccode_line->operand.size() - 3);
                    for (int j = 0; j < (int)str_array.size(); ++j) {
                        siccode_line->object_code.push_back(str_array[j]);
                    }
                }
                else if (siccode_line->operand[0] == 'X') {
                    siccode_line->object_code.push_back(
                        (char)hex_to_int(
                        (char*)siccode_line->operand.substr(2,3).c_str()));
                }
            }

            else if (siccode_line->mnemonic == "WORD") {
                append_object_code(siccode_line->object_code, 
                    str_to_int((char*)siccode_line->operand.c_str()));
            }

            else if (siccode_line->mnemonic == "RESW" ||
                     siccode_line->mnemonic == "RESB" ||
                     siccode_line->mnemonic == "START" ||
                     siccode_line->mnemonic == "END") {
                continue;
            }

            else {
                siccode_line->object_code.push_back(
                    siccode_line->opcode_ref->opcode);

                switch (siccode_line->opcode_ref->operands) {
                case 0:
                    append_object_code(siccode_line->object_code, (short)0);
                    break;
                case 1:
                    if (siccode_line->operand_indexed) {
                        append_object_code(siccode_line->object_code, 
                            (short)(_siccode_lines[_symbols_table[
                                siccode_line->operand.substr(
                                    0, siccode_line->operand.find(','))]
                            ]->address | 0x8000));
                    }
                    else {
                        append_object_code(siccode_line->object_code,
                            (short)(_siccode_lines[_symbols_table[
                                siccode_line->operand.substr(
                                    0, siccode_line->operand.find(','))]
                            ]->address));
                    }
                    break;
                case 2: {
                        string loperand = siccode_line->operand.substr(
                            0, siccode_line->operand.find(','));
                        string roperand = siccode_line->operand.substr(
                            siccode_line->operand.find(','), 
                            siccode_line->operand.size()-1);

                        if (siccode_line->opcode_ref->register_based) {

                        }
                        else {

                        }
                    }
                    break;
                }
            }
        }
        else {
            is_ready = false;
        }
    }
}

void cObjectFile::print_listfile() {
    SICCodeLine* siccode_line;
    for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        printf("%04X ", siccode_line->address);

        for (int j = 0; j < (int)min(3,siccode_line->object_code.size()); ++j) {
            printf("%02X", (unsigned char)siccode_line->object_code[j]);
        }

        for (int j = 0; j < (int)(6-2*siccode_line->object_code.size()); ++j) {
            printf(" ");
        }

        printf(" %-9s %-8s %-9s %-10s\n",
            siccode_line->label.c_str(),
            siccode_line->mnemonic.c_str(),
            siccode_line->operand.c_str(),
            siccode_line->comment.c_str());

        for (int j = 3; j < (int)siccode_line->object_code.size(); ++j) {
            if (j == 3) {
                printf("     ");
            }
            else if (j != 3 && (j % 3 == 0)) {
                printf("\n     ");
            }

            printf("%02X", (unsigned char)siccode_line->object_code[j]);

            if (j + 1 == siccode_line->object_code.size()) {
                printf("\n");
            }
        }

        for (int j = 0; j < (int)siccode_line->errors.size(); ++j) {
            printf(" **** %s\n", siccode_line->errors[j].c_str());
        }
    }
}

void cObjectFile::append_object_code(vector<char> &object_code, int x) {
    object_code.push_back(x >> 16);
    object_code.push_back(x >> 8);
    object_code.push_back(x & 0x000000ff);
}

void cObjectFile::append_object_code(vector<char> &object_code, short x) {
    object_code.push_back(x >> 8);
    object_code.push_back(x & 0x00ff);
}

cObjectFile::~cObjectFile() {
}
