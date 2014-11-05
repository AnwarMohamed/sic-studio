#include "cListFile.h"

#include <cctype>
#include <climits>

#define zero(o, s) memset(o, 0, s);

#define ERROR_MISSING_OPCODE  "missing operation code"
#define ERROR_UNRECOG_OPCODE  "unrecognized operation code"
#define ERROR_MISSING_OPERAND "missing or misplaced operand in instruction"
#define ERROR_ILLEGAL_OPERAND "illegal operand in instruction"
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
	_start_set = false;

	ListFileOpCode* opcode;
	for (int i = 0; i < 59; ++i)
	{
		opcode = new ListFileOpCode;
		zero(opcode, sizeof(ListFileOpCode));

		opcode->mnemonic = opcodes_mnemonic[i];
		opcode->opcode = opcodes_format[i][0];
		opcode->operands = opcodes_format[i][1];
		opcode->register_based = opcodes_format[i][2] ? true : false;

		_opcodes_table[opcodes_mnemonic[i]] = opcode;
	}

	is_ready = is_ready ? parse_sourcefile_lines() : false;
}

void cListFile::construct_symbol_table() {
	ListFileLine* listfile_line;

	for (int i = 0; i < (int)_sourcefile_lines.size(); ++i) {
		listfile_line = new ListFileLine;
		zero(listfile_line, sizeof(ListFileLine));

		if (_sourcefile_lines[i]->label.size()) {
			if (_symbols_table.count(_sourcefile_lines[i]->label) == 0) {
				_symbols_table[_sourcefile_lines[i]->label] = i;
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
			_listfile_lines[0]->address = _start_address;
			_start_set = true;
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

	map<string, ListFileOpCode*>::iterator opcode_table_it;

	for (int i = _start_set?1:0; i < (int)_sourcefile_lines.size(); ++i) {
		if (_sourcefile_lines[i]->instruction.size()) {

			/* Handling START Directive */
			if (_sourcefile_lines[i]->instruction == "START") {
				_listfile_lines[i]->errors.push_back(ERROR_DUPLICATE_START);
			}

			/* Handling WORD Directive */
			else if (_sourcefile_lines[i]->instruction == "WORD") {
				_listfile_lines[i]->address = _current_address;
				_current_address += 3;

				if (!is_word_str(_sourcefile_lines[i]->operand)) {
					_listfile_lines[i]->errors.push_back(ERROR_ILLEGAL_WORD);
				}
			}

			/* Handling BYTE Directive */
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

			/* Handling RESW Directive */
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

			/* Handling RESB Directive */
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

			/* Handling END Directive */
			else if (_sourcefile_lines[i]->instruction == "END") {
				_listfile_lines[i]->address = _current_address;
				_current_address += 1;
				_end_set = true;

				if (_symbols_table.count(_sourcefile_lines[i]->operand) != 1) {
					_listfile_lines[i]->errors.push_back(ERROR_UNDEFINED_SYM);					
				}
			}

			/* Handling Opcodes */
			else if ((opcode_table_it = _opcodes_table.find(
				_sourcefile_lines[i]->instruction)) != _opcodes_table.end()) {
				_listfile_lines[i]->address = _current_address;
				_current_address += 3;

				_listfile_lines[i]->object_code |= opcode_table_it->second->opcode << 16;

				switch (opcode_table_it->second->operands)
				{
				case 0:
					if (_sourcefile_lines[i]->operand.size()) {
						_listfile_lines[i]->errors.push_back(
							ERROR_ILLEGAL_OPERAND);
					}
					break;
				case 1:
					if (_sourcefile_lines[i]->operand.size()) {
						if (_sourcefile_lines[i]->operand.find(',') != -1) {
							if (_sourcefile_lines[i]->operand.find_last_of(',')
								!= _sourcefile_lines[i]->operand.find(',') ||
								_sourcefile_lines[i]->operand.find(',') + 2
								!= _sourcefile_lines[i]->operand.size() ||
								_sourcefile_lines[i]->operand[
									_sourcefile_lines[i]->operand.find(',')+1]
								!= 'X') {
								_listfile_lines[i]->errors.push_back(
									ERROR_ILLEGAL_OPERAND);
							}
						}
						else if (_symbols_table.count(
							_sourcefile_lines[i]->operand) != 1) {
							_listfile_lines[i]->errors.push_back(
								ERROR_UNDEFINED_SYM);
						}
						else {
							_listfile_lines[i]->object_code |= _listfile_lines[
								_symbols_table[
									_sourcefile_lines[i]->operand]]->address;
						}
					}
					else {
						_listfile_lines[i]->errors.push_back(
							ERROR_MISSING_OPERAND);
					}
					break;
				case 2:
					if (_sourcefile_lines[i]->operand.find_last_of(',') != -1 &&
						_sourcefile_lines[i]->operand.find_last_of(',')
						== _sourcefile_lines[i]->operand.find(',')) {
						if (_sourcefile_lines[i]->instruction == "SHIFTL" ||
							_sourcefile_lines[i]->instruction == "SHIFTR") {

						}
						else {

						}
					}	
					else {
						_listfile_lines[i]->errors.push_back(
							ERROR_ILLEGAL_OPERAND);
					}
					break;
				}

	

			}

			/* Handling Unknown Opcodes */
			else {
				_listfile_lines[i]->address = _current_address;
				_listfile_lines[i]->errors.push_back(ERROR_UNRECOG_OPCODE);
			}


			if (_end_set && _sourcefile_lines[i]->instruction != "END") {
				_listfile_lines[i]->errors.push_back(ERROR_AFTER_END);
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
	return is_numeric(str) && (str_to_int((char*)str.c_str()) != -1 ? true : false);
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

void cListFile::print_listfile() {
	ListFileLine* listfile_line;
	SourceFileLine* sourcefile_line;

	for (int i = 0; i < (int)_listfile_lines.size(); ++i) {
		listfile_line = _listfile_lines[i];
		sourcefile_line = _sourcefile_lines[i];

		printf("%04x %06x %-9s %-8s %-9s %-10s\n",
			listfile_line->address,
			(unsigned int)listfile_line->object_code & 0x00ffffff,
			sourcefile_line->label.c_str(),
			sourcefile_line->instruction.c_str(),
			sourcefile_line->operand.c_str(),
			sourcefile_line->comment.c_str());

		for (int j = 0; j < (int)listfile_line->errors.size(); ++j) {
			printf(" **** %s\n", listfile_line->errors[j].c_str());
		}
	}
}

cListFile::~cListFile() {
}
