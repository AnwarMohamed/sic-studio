#pragma once
#include "sicasm.h"
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

#include "cSourceFile.h"

using namespace std;

class cListFile: public cSourceFile {
public:
    cListFile(char* filename);
    ~cListFile();

    virtual void print_listfile();

private:
    bool parse_siccode_lines();
    int hex_to_int(char* hex);

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

    int str_to_int(char* str);
};

