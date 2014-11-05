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

                    }
                    else {

                    }
                    break;
                case 2:
                    break;
                }
            }

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
