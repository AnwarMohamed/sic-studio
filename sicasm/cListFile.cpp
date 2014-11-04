#include "cListFile.h"

#include <cctype>
#include <climits>

#define zero(o, s) memset(o, 0, s);

#define ERROR_MISSING_OPCODE  "missing operation code"
#define ERROR_UNRECOG_OPCODE  "unrecognized operation code"
#define ERROR_MISSING_OPERAND "missing or misplaced operand in instruction"
#define ERROR_MISSING_START   "missing or misplaced start statement"
#define ERROR_ILLEGAL_START   "illegal operand in start statement"
#define ERROR_DUPLICATE_START "duplicate or misplaced start statement"

cListFile::cListFile(char* filename) : cSourceFile(filename) {
	start_address = -1;
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
				if (start_address == -1) {
					start_address = hex_to_int(
						(char*)_sourcefile_lines[i]->operand.c_str());

					if (start_address == -1) {
						listfile_line->errors.push_back(ERROR_ILLEGAL_START);
						listfile_line->address = current_address;
					}
					else {
						listfile_line->address = start_address;
						current_address += 3;
					}
				}
				else {
					listfile_line->errors.push_back(ERROR_DUPLICATE_START);
					listfile_line->address = current_address;
				}
			}
			else {
				listfile_line->errors.push_back(ERROR_MISSING_OPERAND);
				listfile_line->address = current_address;

				if (start_address != -1) {
					listfile_line->errors.push_back(ERROR_DUPLICATE_START);
				}
			}
		}


		if (i == 0 && start_address == -1) {
			listfile_line->errors.push_back(ERROR_MISSING_START);
		}

		return true;
	}
}

int cListFile::hex_to_int(char* hex) {
	int result;
	sscanf_s(hex, "%x", &result);
	return result > USHRT_MAX ? -1 : result;
}

cListFile::~cListFile() {
}
