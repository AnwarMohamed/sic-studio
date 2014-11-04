#include "cSourceFile.h"

cSourceFile::cSourceFile(CHAR* Filename): cFile(Filename) {
	is_ready = FALSE;
    if (BaseAddress) {
		SplitWords();
    }
}


void cSourceFile::SplitWords() {

	stringstream s;
	const char* token = "HELLO";
	s << token;
	cout << s.str() << endl;

/*
    UINT StartPtr = 0;
    for (UINT i = 0; i < FileLength; ++i) {
        if (!isascii(((CHAR*)BaseAddress)[i]))
            return;
        else if(((CHAR*)BaseAddress)[i] == '.') {
            StartPtr = i;
            for (i; i < FileLength; ++i)
                if ((i + 1 == FileLength && i+StartPtr) || 
                    ((CHAR*)BaseAddress)[i - 1] == '\n' || 
                    ((CHAR*)BaseAddress)[i - 1] == '\r')
                    _sourcefile_lines.push_back(string((CHAR*)BaseAddress, StartPtr, 
											i-StartPtr));
        }
        else if (((CHAR*)BaseAddress)[i] == ' ' || 
            ((CHAR*)BaseAddress)[i] == '\t' ||
            ((CHAR*)BaseAddress)[i] == '\n' || 
            ((CHAR*)BaseAddress)[i] == '\r') {
            if (i - 1 >= 0 && 
                (((CHAR*)BaseAddress)[i-1] != ' ' && 
                ((CHAR*)BaseAddress)[i-1] != '\t' && 
                ((CHAR*)BaseAddress)[i-1] != '\n' && 
                ((CHAR*)BaseAddress)[i-1] != '\r'))
                _sourcefile_lines.push_back(string((CHAR*)BaseAddress, StartPtr,
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
            _sourcefile_lines.push_back(string((CHAR*)BaseAddress, StartPtr,
									i-StartPtr+1));
    }

    for (UINT i = 0; i < _sourcefile_lines.size(); ++i)
        transform(_sourcefile_lines[i].begin(), _sourcefile_lines[i].end(), _sourcefile_lines[i].begin(), 
			      ::toupper);*/
}

cSourceFile::~cSourceFile(void) {
}
