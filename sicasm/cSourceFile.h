#pragma once
#include "cFile.h"
#include <vector>
#include <algorithm>
#include <string>
#include <map>

enum SourceFileError
{

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

    BOOL isReady;
    map<string, UCHAR> OpTable;

    void StartPass1();
    void StartPass2();
private:
    void SplitWords();
	vector<SourceFileLine*> _source_file_lines;

    void ConstructOpTable();
};

