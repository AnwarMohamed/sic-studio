#include "sicasm.h"

#pragma once
class cListLine
{
public:
	cListLine();
	~cListLine();


private:

	int address;
	string label;
	string mnemonic;
	string mnemonic_t;
	SICOpCode* opcode_ref;
	vector<string> operands;
	string operands_t;
	string comment;
	vector<char> object_code;
	vector<string> errors;
	bool is_indexed;
	bool is_comment;
	bool is_xe4;

	bool is_indirect;
	bool is_immediate;
	bool is_immediate_hex;

	bool is_variable;
	bool is_literal;
};

