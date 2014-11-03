#pragma once
#include "cFile.h"
#include <vector>
#include <algorithm>
#include <string>
#include <map>

class cSourceFile: public cFile
{
public:
    cSourceFile(CHAR* Filename);
    ~cSourceFile(void);

    BOOL    isReady;
    map<string, UCHAR> OpTable;

    void StartPass1();
    void StartPass2();
private:
    void    SplitWords();
    vector<string> Words;

    void    ConstructOpTable();
};

