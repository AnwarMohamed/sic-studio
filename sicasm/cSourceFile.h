#pragma once
#include "cFile.h"
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <sstream>
#include<iostream>

using namespace std;

enum SourceFileError
{
	kOk = 0,
	kInvalidInstruction = 1 << 0,
};

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

