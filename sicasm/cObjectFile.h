#pragma once
#include "cListFile.h"

class cObjectFile: public cListFile
{
public:
    cObjectFile(CHAR* Filename);
    ~cObjectFile();
};

