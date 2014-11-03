#pragma once
#include "sicasm.h"
#include "cSourceFile.h"

using namespace std;

STRUCT SIC_LIST_LINE
{
    UINT Index;
    UINT Address;
    string Label;
    string Opcode;
    string Operands;
    string Comment;
    string Error;
};

class cListFile: public cSourceFile
{
public:
    cListFile(CHAR* Filename);
    ~cListFile();

};

