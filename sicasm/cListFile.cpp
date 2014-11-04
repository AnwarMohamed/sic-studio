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

cListFile::cListFile(char* filename) : cSourceFile(filename) {
	_start_address = -1;
	_end_set = false;
	is_ready = parse_sourcefile_lines();
}

bool cListFile::parse_sourcefile_lines() {

	ListFileLine* listfile_line;
	int current_address = 0;

	for (int i = 0; i < (int)_sourcefile_lines.size(); ++i) {
		listfile_line = new ListFileLine;
		zero(listfile_line, sizeof(ListFileLine));

		/* Handling START Operation */
		if (_sourcefile_lines[i]->instruction == "START") {
			if (_sourcefile_lines[i]->operand.size() == 4) {
				if (_start_address == -1) {
					_start_address = hex_to_int(
						(char*)_sourcefile_lines[i]->operand.c_str());

					if (_start_address == -1) {
						listfile_line->errors.push_back(ERROR_ILLEGAL_START);
						listfile_line->address = current_address;
					}
					else {
						listfile_line->address = _start_address;
						current_address = _start_address;

						if (_sourcefile_lines[i]->directive.size()) {
							_symbols_table[_sourcefile_lines[i]->directive] = i;
						}
					}
				}
				else {
					listfile_line->errors.push_back(ERROR_DUPLICATE_START);

					listfile_line->address = hex_to_int(
						(char*)_sourcefile_lines[i]->operand.c_str());

					if (listfile_line->address == -1) {
						listfile_line->address = current_address;
					}
				}
			}
			else {
				listfile_line->errors.push_back(ERROR_MISSING_OPERAND);
				listfile_line->address = current_address;

				if (_start_address != -1) {
					listfile_line->errors.push_back(ERROR_DUPLICATE_START);
				}
			}
		}

		/* Handling END Operation */
		else if (_sourcefile_lines[i]->instruction == "END") {
			if (_sourcefile_lines[i]->operand.size()) {
				if (_symbols_table.count(_sourcefile_lines[i]->operand) != 1) {
					listfile_line->errors.push_back(ERROR_UNDEFINED_SYM);
				}
				else {
					_end_set = true;
				}

				if (_sourcefile_lines[i]->directive.size()) {
					_symbols_table[_sourcefile_lines[i]->directive] = i;
				}
			}
			else {
				listfile_line->errors.push_back(ERROR_OPERAND_END);
				listfile_line->address = current_address;
			}
		}

		/* Handling WORD Operation */
		else if (_sourcefile_lines[i]->instruction == "WORD") {
			if (_sourcefile_lines[i]->operand.size()) {
				if (is_numeric(_sourcefile_lines[i]->operand) &&
					str_to_int(
						(char*)_sourcefile_lines[i]->operand.c_str()) != -1) {
					listfile_line->errors.push_back(ERROR_UNDEFINED_SYM);
					listfile_line->address = current_address;
					current_address += 3;
				}
				else {
					listfile_line->errors.push_back(ERROR_ILLEGAL_WORD);
					listfile_line->address = current_address;
					current_address += 3;
				}

				if (_sourcefile_lines[i]->directive.size()) {
					_symbols_table[_sourcefile_lines[i]->directive] = i;
				}
			}
			else {
				listfile_line->errors.push_back(ERROR_OPERAND_WORD);
				listfile_line->address = current_address;
				current_address += 3;
			}
		}

		/* Handling RESW Operation */
		else if (_sourcefile_lines[i]->instruction == "RESW") {
			if (_sourcefile_lines[i]->operand.size()) {
				int operand_value;
				if (is_numeric(_sourcefile_lines[i]->operand) &&
					(operand_value = str_to_int(
						(char*)_sourcefile_lines[i]->operand.c_str())) != -1) {
					listfile_line->errors.push_back(ERROR_UNDEFINED_SYM);
					listfile_line->address = current_address;
					current_address += (3* operand_value);
				}
				else {
					listfile_line->errors.push_back(ERROR_ILLEGAL_RESW);
					listfile_line->address = current_address;
					current_address += 3;
				}

				if (_sourcefile_lines[i]->directive.size()) {
					_symbols_table[_sourcefile_lines[i]->directive] = i;
				}
			}
			else {
				listfile_line->errors.push_back(ERROR_OPERAND_RESW);
				listfile_line->address = current_address;
				current_address += 3;
			}
		}

		if (i == 0 && _start_address == -1) {
			listfile_line->errors.push_back(ERROR_MISSING_START);
		}

		if (_end_set && _sourcefile_lines[i]->instruction != "END") {
			listfile_line->errors.push_back(ERROR_AFTER_END);
		}

		_listfile_lines.push_back(listfile_line);
	}

	return true;
}

bool cListFile::is_numeric(string& str) {
	for (int i = 0; i < (int)str.size(); ++i)
		if (!isdigit(str[i]))
			return false;
	return true;
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
