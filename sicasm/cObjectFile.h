#pragma once
#include "cListFile.h"

class cObjectFile: public cListFile {
public:
    cObjectFile(char* filename);
    ~cObjectFile();

    void print_listfile();

private:
    void generate_object_code();
    void append_object_code(vector<char> &object_code, int x);
    void append_object_code(vector<char> &object_code, short x);
};

