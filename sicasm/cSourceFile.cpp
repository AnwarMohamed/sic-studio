#include "cSourceFile.h"

cSourceFile::cSourceFile(CHAR* Filename): cFile(Filename) {
    isReady = FALSE;
    if (BaseAddress) {
        SplitWords();
        ConstructOpTable();
    }
}

void cSourceFile::ConstructOpTable()
{
    OpTable["ADD"] =    0x18;
    OpTable["ADDF"] =   0x58;
    OpTable["ADDR"] =   0x90;
    OpTable["AND"] =    0x40;
    OpTable["CLEAR"] =  0xB4;
    OpTable["COMP"] =   0x28;
    OpTable["COMPF"] =  0x88;
    OpTable["COMPR"] =  0xA0;
    OpTable["DIV"] =    0x24;
    OpTable["DIVF"] =   0x64;
    OpTable["DIVR"] =   0x9C;
    OpTable["FIX"] =    0xC4;
    OpTable["FLOAT"] =  0xC0;
    OpTable["HIO"] =    0xF4;
    OpTable["J"] =      0x3C;
    OpTable["JEQ"] =    0x30;
    OpTable["JGT"] =    0x34;
    OpTable["JLT"] =    0x38;
    OpTable["JSUB"] =   0x48;
    OpTable["LDA"] =    0x00;
    OpTable["LDB"] =    0x68;
    OpTable["LDCH"] =   0x50;
    OpTable["LDF"] =    0x70;
    OpTable["LDL"] =    0x08;
    OpTable["LDS"] =    0x6C;
    OpTable["LDT"] =    0x74;
    OpTable["LDX"] =    0x04;
    OpTable["LPS"] =    0xD0;
    OpTable["MUL"] =    0x20;
    OpTable["MULF"] =   0x60;
    OpTable["MULR"] =   0x98;
    OpTable["NORM"] =   0xC8;
    OpTable["OR"] =     0x44;
    OpTable["RD"] =     0xD8;
    OpTable["RMO"] =    0xAC;
    OpTable["RSUB"] =   0x4C;
    OpTable["SHIFTL"] = 0xA4;
    OpTable["SHIFTR"] = 0xA8;
    OpTable["SIO"] =    0xF0;
    OpTable["SSK"] =    0xEC;
    OpTable["STA"] =    0x0C;
    OpTable["STB"] =    0x78;
    OpTable["STCH"] =   0x54;
    OpTable["STF"] =    0x80;
    OpTable["STI"] =    0xD4;
    OpTable["STL"] =    0x14;
    OpTable["STS"] =    0x7C;
    OpTable["STSW"] =   0xE8;
    OpTable["STT"] =    0x84;
    OpTable["STX"] =    0x10;
    OpTable["SUB"] =    0x1C;
    OpTable["SUBF"] =   0x5C;
    OpTable["SUBR"] =   0x94;
    OpTable["SVC"] =    0xB0;
    OpTable["TD"] =     0xE0;
    OpTable["TIO"] =    0xF8;
    OpTable["TIX"] =    0x2C;
    OpTable["TIXR"] =   0xB8;
    OpTable["WD"] =     0xDC;
}

void cSourceFile::SplitWords()
{
    UINT StartPtr=0;
    for (UINT i = 0; i < FileLength; i++) {
        if (!isascii(((CHAR*)BaseAddress)[i]))
            return;
        else if(((CHAR*)BaseAddress)[i] == '.') {
            StartPtr = i;
            for (i; i<FileLength; i++)
                if ((i+1 == FileLength && i+StartPtr) || 
                    ((CHAR*)BaseAddress)[i-1] == '\n' || 
                    ((CHAR*)BaseAddress)[i-1] == '\r')
                    Words.push_back(string((CHAR*)BaseAddress, StartPtr, 
											i-StartPtr));
        }
        else if (((CHAR*)BaseAddress)[i] == ' ' || 
            ((CHAR*)BaseAddress)[i] == '\t' ||
            ((CHAR*)BaseAddress)[i] == '\n' || 
            ((CHAR*)BaseAddress)[i] == '\r') {
            if (i-1 >=0 && 
                (((CHAR*)BaseAddress)[i-1] != ' ' && 
                ((CHAR*)BaseAddress)[i-1] != '\t' && 
                ((CHAR*)BaseAddress)[i-1] != '\n' && 
                ((CHAR*)BaseAddress)[i-1] != '\r'))
                Words.push_back(string((CHAR*)BaseAddress, StartPtr,
										i-StartPtr));
        }

        if (i + 1 != FileLength &&
            (((CHAR*)BaseAddress)[i] == ' ' || 
            ((CHAR*)BaseAddress)[i] == '\t' || 
            ((CHAR*)BaseAddress)[i] == '\n' || 
            ((CHAR*)BaseAddress)[i] == '\r') &&
            (((CHAR*)BaseAddress)[i+1] != ' ' && 
            ((CHAR*)BaseAddress)[i+1] != '\t' && 
            ((CHAR*)BaseAddress)[i+1] != '\n' && 
            ((CHAR*)BaseAddress)[i+1] != '\r'))
            StartPtr = i+1;

        if (i + 1 == FileLength && i != StartPtr)
            Words.push_back(string((CHAR*)BaseAddress, StartPtr,
									i-StartPtr+1));
    }

    for (UINT i = 0; i < Words.size(); i++)
        transform(Words[i].begin(), Words[i].end(), Words[i].begin(), 
			      ::toupper);
}

cSourceFile::~cSourceFile(void) {
}
