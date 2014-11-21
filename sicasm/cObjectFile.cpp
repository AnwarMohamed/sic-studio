/*
*
*  Copyright (C) 2014  Anwar Mohamed <anwarelmakrahy[at]gmail.com>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to Anwar Mohamed
*  anwarelmakrahy[at]gmail.com
*
*/

#include "cObjectFile.h"

cObjectFile::cObjectFile(char* filename) : cListFile(filename) {
	generate_object_code();
	generate_objectfile();
}

void cObjectFile::generate_objectfile() {
	if (!is_ready) {
		return;
	}

	/* Header */
	_object_code += 'H';

	int program_name_size = _program_name.size();
	for (int i = 0; i < (int)(8 - program_name_size); ++i) {
		_program_name += ' ';
	}
	_object_code += _program_name.substr(0, 6);
	_object_code += int_to_hex_string(_start_address);
	_object_code += int_to_hex_string(_end_address - _start_address);
	_object_code += '\n';

	/* Body */
	SICCodeLine* siccode_line;
	char t_line_size;
	string _object_line;
	for (int i = 0; i < (int)_siccode_lines.size();) {
		siccode_line = _siccode_lines[i];
		_object_line.clear();

		if (siccode_line->object_code.size() == 0) {
			++i;
			continue;
		}

		_object_line += 'T';
		_object_line += int_to_hex_string(siccode_line->address);
		_object_line += "00";

		t_line_size = 0;
		while (i < (int)_siccode_lines.size()) {
			siccode_line = _siccode_lines[i];

			if (siccode_line->is_comment) {
				++i;
				continue;
			}

			if (siccode_line->object_code.size() == 0) {
				++i;
				break;
			}

			if (t_line_size + (siccode_line->object_code.size() * 2) > 60) {
				break;
			}

			for (int j = 0; j < (int)siccode_line->object_code.size(); ++j) {
				_object_line += char_to_hex_string(siccode_line->object_code[j]);
			}

			t_line_size += siccode_line->object_code.size() * 2;
			++i;
		}

		_object_line.replace(7, 2, char_to_hex_string(t_line_size / 2));
		_object_line += '\n';
		_object_code += _object_line;
	}

	/* Footer */
	_object_code += 'E';
	_object_code += int_to_hex_string(_start_address);
	_object_code += '\n';
}

void cObjectFile::generate_object_code() {
	SICCodeLine* siccode_line;
	bool base_relative = false;
	is_ready = true;

	for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
		siccode_line = _siccode_lines[i];

		if (siccode_line->is_comment) {
			continue;
		}

		else if (siccode_line->errors.size()) {
			is_ready = false;
			break;
		}

		if (siccode_line->mnemonic == "BYTE") {
			if (siccode_line->operands[0][0] == 'C') {
				string str_array = siccode_line->operands[0].substr(
					2, siccode_line->operands[0].size() - 3);
				for (int j = 0; j < (int)str_array.size(); ++j) {
					siccode_line->object_code.push_back(str_array[j]);
				}
			}
			else if (siccode_line->operands[0][0] == 'X') {
				append_object_code(siccode_line->object_code,
					siccode_line->operands[0].substr(
					2, siccode_line->operands[0].size() - 2));
			}
		}

		else if (siccode_line->mnemonic == "WORD") {
			append_object_code(siccode_line->object_code,
				str_to_int((char*)siccode_line->operands[0].c_str()));
		}

		else if (siccode_line->mnemonic == "RESW" ||
			siccode_line->mnemonic == "RESB" ||
			siccode_line->mnemonic == "START" ||
			siccode_line->mnemonic == "END") {
			continue;
		}

		else {
			siccode_line->object_code.push_back(
				siccode_line->opcode_ref->opcode);

			switch (siccode_line->opcode_ref->operands) {
			case 0:
				append_object_code(siccode_line->object_code, (short)0);
				siccode_line->object_code[0] |= 0x03;
				break;
			case 1:

				switch (siccode_line->opcode_ref->format) {
				case 2:
					/* Must handle register_based */
					siccode_line->object_code.push_back(
						(char)(_siccode_lines[_symbols_table[
							siccode_line->operands[0]]
						]->address));

					if (siccode_line->is_indexed) {
						siccode_line->object_code[1] |= 0x80;
					}
					break;
				case 3:
					if (!siccode_line->is_xe4) {
						if (siccode_line->is_immediate) {
							append_object_code(siccode_line->object_code,
								(short)(str_to_int((char*)
								siccode_line->operands[0].substr(1,
								siccode_line->operands[0].size()).c_str())));
							siccode_line->object_code[0] |= 0x01;
						}
						else if (siccode_line->is_immediate_hex) {
							append_object_code(siccode_line->object_code,
								int_to_hex_string(
								hex_to_int(
								(char*)siccode_line->operands[0].c_str()))
								.substr(2, 8));
							siccode_line->object_code[0] |= 0x03;
						}
						else {
							int address = 0;
							if (siccode_line->is_indirect) {
								siccode_line->object_code[0] &= 0xfe;
								siccode_line->object_code[0] |= 0x02;

								if (is_numeric(
									siccode_line->operands[0].substr(
									1, siccode_line->operands[0].size()))) {
									address = hex_to_int(
										(char*)siccode_line->operands[0].c_str());
								}
								else {
									address = _siccode_lines[_symbols_table[
										siccode_line->operands[0].substr(1, 
											siccode_line->operands[0].size())]
									]->address;
								}
							}
							else {
								siccode_line->object_code[0] |= 0x03;

								if (is_numeric(siccode_line->operands[0])) {
									address = hex_to_int(
										(char*)siccode_line->operands[0].c_str());
								}
								else {
									address = _siccode_lines[_symbols_table[
										siccode_line->operands[0]]
									]->address;
								}
							}

							if (address > 0x0fff) {
								address -= next_address(i);
								address &= 0x00000fff;
								append_object_code(
									siccode_line->object_code, (short)address);							
								siccode_line->object_code[1] |= 0x20;
							}
							else {
								address &= 0x00000fff;
								append_object_code(
									siccode_line->object_code, (short)address);
							}
						}
					}
					else {
						/* Handling Format 4 */
					}
					break;
				}

				if (siccode_line->is_indexed) {
					siccode_line->object_code[1] |= 0x80;
				}
				
				break;
			case 2: {
						string loperand = siccode_line->operands[0];
						string roperand = siccode_line->operands[1];

						if (siccode_line->opcode_ref->register_based) {

						}
						else {

						}
			}
				break;
			}
		}
	}
}

char cObjectFile::encode_register(string &reg) {
	switch (reg[0]) {
	case 'A':
		return 0;
	case 'X':
		return 1;
	case 'L':
		return 2;
	case 'B':
		return 3;
	case 'S':
		return 4;
	case 'T':
		return 5;
	case 'F':
		return 6;
	default:
		return -1;
	}
}

void cObjectFile::print_objectfile(FILE* file) {
	fprintf(file ? file : stdout, "%s", _object_code.c_str());
}

void cObjectFile::print_listfile(FILE* file) {
	if (file) {
		fprintf(file,
			"SIC/XE Assembler\n"
			"By Anwar Mohamed ~ 2491\n"
			"anwarelmakrahy@gmail.com\n\n");
	}

	SICCodeLine* siccode_line;
	for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
		siccode_line = _siccode_lines[i];
		if (siccode_line->is_comment) {
			fprintf(file ? file : stdout,
				"            %s\n", siccode_line->comment.c_str());
		}
		else {
			fprintf(file ? file : stdout, "%04X ", siccode_line->address);

			for (int j = 0;
				j < (int)min(3, siccode_line->object_code.size()); ++j) {
				fprintf(file ? file : stdout,
					"%02X", (unsigned char)siccode_line->object_code[j]);
			}

			for (int j = 0;
				j < (int)(6 - 2 * siccode_line->object_code.size()); ++j) {
				fprintf(file ? file : stdout, " ");
			}

			fprintf(file ? file : stdout, " %-8s %-7s %-18s %s\n",
				siccode_line->label.c_str(),
				siccode_line->mnemonic.c_str(),
				merge_operands(siccode_line->operands).c_str(),
				siccode_line->comment.c_str());

			for (int j = 3; j < (int)siccode_line->object_code.size(); ++j) {
				if (j == 3) {
					fprintf(file ? file : stdout, "     ");
				}
				else if (j != 3 && (j % 3 == 0)) {
					fprintf(file ? file : stdout, "\n     ");
				}

				fprintf(file ? file : stdout,
					"%02X", (unsigned char)siccode_line->object_code[j]);

				if (j + 1 == siccode_line->object_code.size()) {
					fprintf(file ? file : stdout, "\n");
				}
			}

			for (int j = 0; j < (int)siccode_line->errors.size(); ++j) {
				fprintf(file ? file : stdout,
					" **** %s\n", siccode_line->errors[j].c_str());
			}
		}
	}
}

string cObjectFile::int_to_hex_string(int i) {
	char hex_str[7];
	sprintf_s(hex_str, "%06X", i & 0x00ffffff);
	return string(hex_str);
}

string cObjectFile::char_to_hex_string(char i) {
	char hex_str[3];
	sprintf_s(hex_str, "%02X", (unsigned char)i);
	return string(hex_str);
}

short cObjectFile::next_address(short i) {
	while (_siccode_lines[i + 1]->is_comment) {
		++i;
	}
	return _siccode_lines[i + 1]->address;
}

void cObjectFile::append_object_code(vector<char> &object_code, int x) {
	object_code.push_back(x >> 16);
	object_code.push_back(x >> 8);
	object_code.push_back(x & 0x000000ff);
}

void cObjectFile::append_object_code(vector<char> &object_code, short x) {
	object_code.push_back(x >> 8);
	object_code.push_back(x & 0x00ff);
}

void cObjectFile::append_object_code(vector<char> &object_code, string &bytes) {
	int byte;
	for (int i = 0; i < (int)bytes.size() / 2; i++) {
		sscanf_s(bytes.substr(2 * i, 2 * i + 2).c_str(), "%02x", &byte);
		object_code.push_back(byte & 0x000000ff);
	}
}

cObjectFile::~cObjectFile() {
}
