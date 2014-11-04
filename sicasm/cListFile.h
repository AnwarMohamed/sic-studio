#pragma once
#include "sicasm.h"
#include "cSourceFile.h"

using namespace std;

struct ListFileLine
{
    int index;
    int address;
    string label;
    string instruction;
    string operands;
    string comment;
    vector<string> errors;
};

class cListFile: public cSourceFile
{
public:
    cListFile(char* filename);
    ~cListFile();
};

