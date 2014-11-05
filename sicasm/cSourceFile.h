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

    void StartPass1();
    void StartPass2();

	BOOL is_ready;

protected:
    vector<SICCodeLine*> _siccode_lines;

private:
    void SplitWords();
};

