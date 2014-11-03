#pragma once
#include "sicasm.h"

class cFile
{
    HANDLE hFile;
    HANDLE hMapping;
    BOOL   IsFile;
public:
    DWORD  BaseAddress;
    DWORD  FileLength;
    DWORD  Attributes;
    CHAR*  Filename;
    BOOL   IsReassembled;

    cFile(CHAR* Filename);
    cFile(CHAR* Buffer, DWORD Size);
    INT OpenFile(CHAR* Filename);
    ~cFile();
};