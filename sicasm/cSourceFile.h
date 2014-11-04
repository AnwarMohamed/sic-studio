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

struct SourceFileLine
{
	string directive;
	string instruction;
	string operand;
	string comment;
	int errors;
};


class cSourceFile: public cFile
{
public:
    cSourceFile(CHAR* Filename);
    ~cSourceFile(void);

    void StartPass1();
    void StartPass2();

	BOOL is_ready;

protected:
	vector<SourceFileLine*> _sourcefile_lines;

private:
    void SplitWords();


};

