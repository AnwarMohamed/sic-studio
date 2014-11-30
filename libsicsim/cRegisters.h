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

#pragma once
#include "sicsim.h"

class cRegisters
{
public:
	cRegisters();
	~cRegisters();

	string dump_register_str(string& name);
	unsigned int dump_register(string& name);

	void set_register(string& name, unsigned int val);
	void set_register(string& name, string& val);

private:
	unsigned int register_A;
	unsigned int register_B;
	unsigned int register_X;
	unsigned int register_S;
	unsigned int register_L;
	unsigned int register_T;
	unsigned int register_PC;
	unsigned int register_F;
	unsigned int register_SW;
};

