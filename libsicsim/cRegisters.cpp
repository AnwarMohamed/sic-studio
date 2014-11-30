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

#include "cRegisters.h"

cRegisters::cRegisters()
{
	register_A  = 0xFFFFFFFF;
	register_B  = 0xFFFFFFFF;
	register_X  = 0xFFFFFFFF;
	register_S  = 0xFFFFFFFF;
	register_L  = 0xFFFFFFFF;
	register_T  = 0xFFFFFFFF;
	register_PC = 0xFFFFFFFF;
	register_F  = 0xFFFFFFFF;
	register_SW = 0xFFFFFFFF;
}

void cRegisters::set_register(string& name, unsigned int val) {
	if (name == "A") {
		register_A = val;
	}
	else if (name == "L") {
		register_L = val;
	}
	else if (name == "B") {
		register_B = val;
	}
	else if (name == "S") {
		register_S = val;
	}
	else if (name == "T") {
		register_T = val;
	}
	else if (name == "X") {
		register_X = val;
	}
	else if (name == "PC") {
		register_PC = val;
	}
	else if (name == "F") {
		register_F = val;
	}
	else if (name == "SW") {
		register_SW = val;
	}
}

void cRegisters::set_register(string& name, string& val) {
	if (val.size() != 6) {
		return;
	}

	char result[7];
	sscanf_s(result, "%06X", val.c_str());

	if (name == "A") {
		register_A = atoi(result);
	}
	else if (name == "L") {
		register_L = atoi(result);
	}
	else if (name == "B") {
		register_B = atoi(result);
	}
	else if (name == "S") {
		register_S = atoi(result);
	}
	else if (name == "T") {
		register_T = atoi(result);
	}
	else if (name == "X") {
		register_X = atoi(result);
	}
	else if (name == "PC") {
		register_PC = atoi(result);
	}
	else if (name == "F") {
		register_F = atoi(result);
	}
	else if (name == "SW") {
		register_SW = atoi(result);
	}
}

string cRegisters::dump_register_str(string& name) {
	char results[9];

	if (name == "A") {
		sprintf_s(results, "%08X", register_A);
	}
	else if (name == "L") {
		sprintf_s(results, "%08X", register_L);
	}
	else if (name == "B") {
		sprintf_s(results, "%08X", register_B);
	}
	else if (name == "S") {
		sprintf_s(results, "%08X", register_S);
	}
	else if (name == "T") {
		sprintf_s(results, "%08X", register_T);
	}
	else if (name == "X") {
		sprintf_s(results, "%08X", register_X);
	}
	else if (name == "PC") {
		sprintf_s(results, "%08X", register_PC);
	}
	else if (name == "F") {
		sprintf_s(results, "%08X", register_F);
	}
	else if (name == "SW") {
		sprintf_s(results, "%08X", register_SW);
	}
	else {
		return "FFFFFF";
	}

	return string(results).substr(2, 9);
}

unsigned int cRegisters::dump_register(string& name) {
	if (name == "A") {
		return register_A;
	}
	else if (name == "L") {
		return register_L;
	}
	else if (name == "B") {
		return register_B;
	}
	else if (name == "S") {
		return register_S;
	}
	else if (name == "T") {
		return register_T;
	}
	else if (name == "X") {
		return register_X;
	}
	else if (name == "PC") {
		return register_PC;
	}
	else if (name == "F") {
		return register_F;
	}
	else if (name == "SW") {
		return register_SW;
	}
	else {
		return 0xFFFFFFFF;
	}
}

cRegisters::~cRegisters()
{
}
