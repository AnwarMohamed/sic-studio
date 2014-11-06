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
#include "cFile.h"
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <sstream>
#include<iostream>

using namespace std;

class cSourceFile: public cFile {
public:
    cSourceFile(char* filename);
    ~cSourceFile();

	bool is_ready;

protected:
    vector<SICCodeLine*> _siccode_lines;
    map<string, SICOpCode*> _opcodes_table;

private:
    void parse_sourcefile();
    string _sourcefile_line;

    void trim_string(string &str);
    void split_strings(SICCodeLine* line, string &str, bool skip = false);
};

