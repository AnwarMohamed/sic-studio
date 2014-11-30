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

    virtual void print_listfile(FILE* file=0);
private:
    bool parse_siccode_lines();
    
    bool _end_set;
    bool _start_set;
    int _current_address;

    
    bool is_hex_number(string& str);
    bool is_word_str(string& str);
    bool is_alpha(string& str);
    bool is_hex_byte(string& str);
    bool is_hex_word(string& str);
    
    void construct_symbol_table();
    bool parse_instructions();
    string suggest_operation(string operation);	

protected:
    int _start_address;
    int _end_address;
    string _program_name;

    map<string, int> _symbols_table;

    int str_to_int(char* str);
    int hex_to_int(char* hex);
	bool is_numeric(string& str);

	char encode_register(string &reg);

    string merge_operands(vector<string> &operands);
    bool starts_with(const string& haystack, const string& needle);
};
