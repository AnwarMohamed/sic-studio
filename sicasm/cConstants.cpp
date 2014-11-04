#include "cConstants.h"


cConstants::cConstants()
{
	construct_optable();
}


cConstants::~cConstants()
{
}


void cConstants::construct_optable()
{
	kOpTable["ADD"] = 0x18;
	kOpTable["ADDF"] = 0x58;
	kOpTable["ADDR"] = 0x90;
	kOpTable["AND"] = 0x40;
	kOpTable["CLEAR"] = 0xB4;
	kOpTable["COMP"] = 0x28;
	kOpTable["COMPF"] = 0x88;
	kOpTable["COMPR"] = 0xA0;
	kOpTable["DIV"] = 0x24;
	kOpTable["DIVF"] = 0x64;
	kOpTable["DIVR"] = 0x9C;
	kOpTable["FIX"] = 0xC4;
	kOpTable["FLOAT"] = 0xC0;
	kOpTable["HIO"] = 0xF4;
	kOpTable["J"] = 0x3C;
	kOpTable["JEQ"] = 0x30;
	kOpTable["JGT"] = 0x34;
	kOpTable["JLT"] = 0x38;
	kOpTable["JSUB"] = 0x48;
	kOpTable["LDA"] = 0x00;
	kOpTable["LDB"] = 0x68;
	kOpTable["LDCH"] = 0x50;
	kOpTable["LDF"] = 0x70;
	kOpTable["LDL"] = 0x08;
	kOpTable["LDS"] = 0x6C;
	kOpTable["LDT"] = 0x74;
	kOpTable["LDX"] = 0x04;
	kOpTable["LPS"] = 0xD0;
	kOpTable["MUL"] = 0x20;
	kOpTable["MULF"] = 0x60;
	kOpTable["MULR"] = 0x98;
	kOpTable["NORM"] = 0xC8;
	kOpTable["OR"] = 0x44;
	kOpTable["RD"] = 0xD8;
	kOpTable["RMO"] = 0xAC;
	kOpTable["RSUB"] = 0x4C;
	kOpTable["SHIFTL"] = 0xA4;
	kOpTable["SHIFTR"] = 0xA8;
	kOpTable["SIO"] = 0xF0;
	kOpTable["SSK"] = 0xEC;
	kOpTable["STA"] = 0x0C;
	kOpTable["STB"] = 0x78;
	kOpTable["STCH"] = 0x54;
	kOpTable["STF"] = 0x80;
	kOpTable["STI"] = 0xD4;
	kOpTable["STL"] = 0x14;
	kOpTable["STS"] = 0x7C;
	kOpTable["STSW"] = 0xE8;
	kOpTable["STT"] = 0x84;
	kOpTable["STX"] = 0x10;
	kOpTable["SUB"] = 0x1C;
	kOpTable["SUBF"] = 0x5C;
	kOpTable["SUBR"] = 0x94;
	kOpTable["SVC"] = 0xB0;
	kOpTable["TD"] = 0xE0;
	kOpTable["TIO"] = 0xF8;
	kOpTable["TIX"] = 0x2C;
	kOpTable["TIXR"] = 0xB8;
	kOpTable["WD"] = 0xDC;
}