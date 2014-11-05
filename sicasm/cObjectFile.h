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
#include "cListFile.h"

class cObjectFile: public cListFile {
public:
    cObjectFile(char* filename);
    ~cObjectFile();

    void print_listfile();
    void print_objectfile();
    void generate_objectfile();

private:
    void generate_object_code();
    void append_object_code(vector<char> &object_code, int x);
    void append_object_code(vector<char> &object_code, short x);
};

