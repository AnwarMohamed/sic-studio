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
#include "cSourceFile.h"

#define zero(o, s) memset(o, 0, s);

cSourceFile::cSourceFile(char* filename): cFile(filename) {
	is_ready = true;

    if (!BaseAddress || !FileLength) {
        is_ready = false;
        return;
    }

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

    parse_sourcefile();
}

void cSourceFile::parse_sourcefile() {
    char *ptr = (char*)BaseAddress, *lptr = ptr;
    SICCodeLine* line;

    for (int i = 0; i < (int)FileLength-1;) {
        if (ptr[i] == '\r' && ptr[i + 1] == '\n' || ptr[i] == '\n' ||
            i + 2 == FileLength) {
            _sourcefile_line = string(lptr, &ptr[(i + 2 == FileLength)? i+2: i] - lptr);
            transform(_sourcefile_line.begin(), _sourcefile_line.end(), 
                _sourcefile_line.begin(), ::toupper);
            
            trim_string(_sourcefile_line);
            line = new SICCodeLine;
            zero(line, sizeof(SICCodeLine));

            string label_word;
            int find_result = _sourcefile_line.find(' ');
            if (find_result != string::npos) {
                label_word = _sourcefile_line.substr(0, find_result);
            }
            else {
                label_word = _sourcefile_line;
            }


            if (label_word[0] == '.') {
                line->label = _sourcefile_line;
                line->is_comment = true;
            }
            else if (label_word[0] == '+') {
                if (_opcodes_table.count(label_word.substr(
                    1, label_word.size() - 1)) == 1) {
                    split_strings(line, _sourcefile_line, true);
                }
                else {
                    split_strings(line, _sourcefile_line);
                }
            }
            else if (_opcodes_table.count(label_word) == 1) {
                split_strings(line, _sourcefile_line, true);
            }
            else {
                split_strings(line, _sourcefile_line);
            }

            _siccode_lines.push_back(line);
            lptr = &ptr[(i += 2)];
        }
        else {
            ++i;
        }
    }
}

void cSourceFile::split_strings(SICCodeLine* line, string &str, bool skip) {
    string operands;

    int find_result = str.find(' '), last_result = 0;
    if (find_result != string::npos) {
        if (skip) {
            line->mnemonic = str.substr(last_result, find_result);
        }
        else {
            line->label = str.substr(last_result, find_result);
        }

        last_result = find_result + 1;
        find_result = str.find(' ', last_result);
        if (find_result != string::npos) {
            if (skip) {
                operands = str.substr(last_result, find_result - last_result);
            }
            else {
                line->mnemonic =
                    str.substr(last_result, find_result - last_result);
            }

            last_result = find_result + 1;
            find_result = str.find(' ', last_result);
            if (find_result != string::npos) {
                
                if (skip) {
                    line->comment = str.substr(last_result, str.size() - last_result);
                }
                else {
                    operands = str.substr(last_result, find_result - last_result);

                    last_result = find_result + 1;
                    line->comment = str.substr(last_result, str.size() - last_result);
                }
            }
            else {                
                if (skip) {
                    line->comment = str.substr(last_result, str.size() - last_result);
                }
                else {
                    operands = str.substr(last_result, str.size() - last_result);
                }
            }
        }
        else {
            if (skip) {
                operands = str.substr(last_result, str.size() - last_result);
            }
            else {
                line->mnemonic = str.substr(last_result, str.size() - last_result);
            }
        }
    }
    else {
        if (skip) {
            line->mnemonic = str;
        }
        else {
            line->label = str;
        }
    }

    if (line->mnemonic == "BYTE") {
        if (operands.size() >= 3 && 
            operands[0] == 'C' &&
            operands[1] == '\'') {
            operands += ' ' + line->comment;

            last_result = 0;
            find_result = operands.find('\'');
            if (find_result != string::npos) {

                last_result = find_result + 1;
                find_result = operands.find('\'', last_result);
                if (find_result != string::npos) {
                    line->operands.push_back(operands.substr(0, find_result+1));

                    if (find_result + 1 == operands.size()) {
                        line->comment.clear();
                    }
                    else {
                        line->comment = operands.substr(
                            find_result + 1, operands.size() - find_result - 1);
                    }
                }
                else {
                    line->operands.push_back(operands);
                    line->comment.clear();
                }
            }
        }
        else {
            line->operands.push_back(operands);
        }
    }

    else if (operands.size()) {
        last_result = 0;
        if (operands.find(',', last_result) != string::npos) {       
            while ((find_result = operands.find(',', last_result)) 
                != string::npos) {
                line->operands.push_back(operands.substr(
                    last_result, find_result - last_result));
                last_result = find_result + 1;
            }
            line->operands.push_back(operands.substr(
                last_result, operands.size() - last_result));
        }
        else {
            line->operands.push_back(operands);
        }
    }
}

void cSourceFile::trim_string(string &str) {
    while (str.size() && str[0] == ' ') {
        str = str.erase(0, 1);
    }

    while (str.size() && str[str.size() - 1] == ' ') {
        str = str.erase(str.size() - 1, 1);
    }

    for (int i = 0; i < (int)str.size() - 1;) {
        if (str[i] == ' ' && str[i + 1] == ' ') {
            str = str.erase(i, 1);
        }
        else {
            ++i;
        }
    }
}

cSourceFile::~cSourceFile(void) {
}
