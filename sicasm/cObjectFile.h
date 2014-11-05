#pragma once
#include "cListFile.h"

class cObjectFile: public cListFile {
public:
    cObjectFile(char* filename);
    ~cObjectFile();

private:
    void generate_object_code();
};

