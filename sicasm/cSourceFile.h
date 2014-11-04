#pragma once
#include "cFile.h"
#include <vector>
#include <algorithm>
#include <string>
#include <map>

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

	BOOL is_ready;

    void StartPass1();
    void StartPass2();

protected:
	vector<SourceFileLine*> _sourcefile_lines;

private:
    void SplitWords();


};

