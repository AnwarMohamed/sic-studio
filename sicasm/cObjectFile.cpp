#include "cObjectFile.h"

cObjectFile::cObjectFile(char* filename): cListFile(filename) {
    generate_object_code();
}

void cObjectFile::generate_object_code() {
    SICCodeLine* siccode_line;
    for (int i = 0; i < _siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        if (siccode_line->errors.size() == 0) {



        }
    }
}

cObjectFile::~cObjectFile(void) {
}
