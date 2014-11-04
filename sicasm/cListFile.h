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
	int str_to_int(char* str);

	bool _end_set;
	int _current_address;

	bool is_numeric(string& str);
	bool is_hex_number(string& str);
	bool is_word_str(string& str);
	void construct_symbol_table();
	bool parse_instructions();

protected:
	int _start_address;
	map<string, int> _symbols_table;
	vector<ListFileLine*> _listfile_lines;
};

