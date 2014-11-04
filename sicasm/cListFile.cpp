#include "cListFile.h"

#include <cctype>
#include <climits>

#define zero(o, s) memset(o, 0, s);

#define ERROR_MISSING_OPCODE  "missing operation code"
#define ERROR_UNRECOG_OPCODE  "unrecognized operation code"
#define ERROR_MISSING_OPERAND "missing or misplaced operand in instruction"
#define ERROR_UNDEFINED_SYM   "undefined symbol in operand"
#define ERROR_MISSING_START   "missing or misplaced start statement"
#define ERROR_ILLEGAL_START   "illegal operand in start statement"
#define ERROR_DUPLICATE_START "duplicate or misplaced start statement"
#define ERROR_OPERAND_END     "missing or misplaced operand in end statement"
#define ERROR_AFTER_END       "statement should not follow end statement"
#define ERROR_OPERAND_WORD    "missing or misplaced operand in word statement"
#define ERROR_ILLEGAL_WORD    "illegal operand in word statement"
#define ERROR_OPERAND_RESW    "missing or misplaced operand in resw statement"
#define ERROR_ILLEGAL_RESW    "illegal operand in resw statement"
#define ERROR_OPERAND_RESB    "missing or misplaced operand in resb statement"
#define ERROR_ILLEGAL_RESB    "illegal operand in resb statement"
#define ERROR_OPERAND_BYTE    "missing or misplaced operand in byte statement"
#define ERROR_ILLEGAL_BYTE    "illegal operand in byte statement"
#define ERROR_DUPLICATE_LABEL "duplicate label definition"


cListFile::cListFile(char* filename) : cSourceFile(filename) {
	_end_set = false;
	is_ready = is_ready ? parse_sourcefile_lines() : false;
}

void cListFile::construct_symbol_table() {
	ListFileLine* listfile_line;

	for (int i = 0; i < (int)_sourcefile_lines.size(); ++i) {
		listfile_line = new ListFileLine;
		zero(listfile_line, sizeof(ListFileLine));

		if (_sourcefile_lines[i]->directive.size()) {
			if (_symbols_table.count(_sourcefile_lines[i]->directive) == 0) {
				_symbols_table[_sourcefile_lines[i]->directive] = i;
			}
			else {
				listfile_line->errors.push_back(ERROR_DUPLICATE_LABEL);
			}
		}

		_listfile_lines.push_back(listfile_line);
	}
}

bool cListFile::parse_instructions() {
	if (!_sourcefile_lines.size()) {
		return false;
	}

	if (_sourcefile_lines[0]->instruction == "START") {
		if (is_hex_number(_sourcefile_lines[0]->operand)) {
			_start_address = _current_address = 
				hex_to_int((char*)_sourcefile_lines[0]->operand.c_str());
		}
		else {
			_start_address = _current_address = 0;
			_listfile_lines[0]->errors.push_back(ERROR_ILLEGAL_START);
		}
	}
	else {
		_start_address = _current_address = 0;
		_listfile_lines[0]->errors.push_back(ERROR_MISSING_START);
	}

	for (int i = 1; i < _sourcefile_lines.size(); ++i) {
		if (_sourcefile_lines[i]->instruction.size()) {

			/* Handling START Operation */
			if (_sourcefile_lines[i]->instruction == "START") {
				_listfile_lines[i]->errors.push_back(ERROR_DUPLICATE_START);
			}

			/* Handling WORD Operation */
			else if (_sourcefile_lines[i]->instruction == "WORD") {
				_listfile_lines[i]->address = _current_address;
				_current_address += 3;

				if (!is_word_str(_sourcefile_lines[i]->operand)) {
					_listfile_lines[i]->errors.push_back(ERROR_ILLEGAL_WORD);
				}
			}

			/* Handling BYTE Operation */
			else if (_sourcefile_lines[i]->instruction == "BYTE") {
				_listfile_lines[i]->address = _current_address;

				if (_sourcefile_lines[i]->operand[0] == 'C') {
					_current_address += (_sourcefile_lines[i]->operand.size() - 3);
				}
				else if (_sourcefile_lines[i]->operand[0] == 'X') {
					_current_address += 1;
				}
				else {
					_listfile_lines[i]->errors.push_back(ERROR_ILLEGAL_BYTE);
				}
			}

			/* Handling RESW Operation */
			else if (_sourcefile_lines[i]->instruction == "RESW") {
				_listfile_lines[i]->address = _current_address;

				if (!is_word_str(_sourcefile_lines[i]->operand)) {
					_listfile_lines[i]->errors.push_back(ERROR_ILLEGAL_RESW);
					_current_address += 3;
				}
				else {
					_current_address += (3 * str_to_int(
						(char*)_sourcefile_lines[i]->operand.c_str()));
				}
			}

			/* Handling RESB Operation */
			else if (_sourcefile_lines[i]->instruction == "RESB") {
				_listfile_lines[i]->address = _current_address;

				if (!is_word_str(_sourcefile_lines[i]->operand)) {
					_listfile_lines[i]->errors.push_back(ERROR_ILLEGAL_RESB);
					_current_address += 1;
				}
				else {
					_current_address += str_to_int(
						(char*)_sourcefile_lines[i]->operand.c_str());
				}
			}


		}
	}

	return true;
}

bool cListFile::parse_sourcefile_lines() {

	construct_symbol_table();
	return parse_instructions();
}

bool cListFile::is_word_str(string& str) {
	return is_numeric(str) && (str_to_int((char*)str.c_str()) != 1 ? true : false);
}

bool cListFile::is_numeric(string& str) {
	for (int i = 0; i < (int)str.size(); ++i)
		if (!isdigit(str[i]))
			return false;
	return true;
}

bool cListFile::is_hex_number(string& str) {
	return str.size() ? hex_to_int((char*)str.c_str()) == -1 ? false : true: false;
}

int cListFile::str_to_int(char* str) {
	int result;
	sscanf_s(str, "%d", &result);
	return (unsigned int)result > 0x00ffffff ? -1 : result;
}

int cListFile::hex_to_int(char* hex) {
	int result;
	sscanf_s(hex, "%x", &result);
	return (unsigned int)result > 0x0000ffff ? -1 : result;
}

cListFile::~cListFile() {
}
