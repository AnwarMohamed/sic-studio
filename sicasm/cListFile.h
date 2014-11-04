#pragma once
#include "sicasm.h"
#include "cSourceFile.h"

using namespace std;

enum ListFileErrors {
	
};

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

private:
	bool parse_sourcefile_lines();
	int hex_to_int(char* hex);

	bool _end_set;

protected:
	int _start_address;
	map<string, int> _symbols_table;
	vector<ListFileLine*> _listfile_lines;
};

