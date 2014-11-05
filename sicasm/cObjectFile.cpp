#include "cObjectFile.h"

cObjectFile::cObjectFile(char* filename): cListFile(filename) {
    generate_object_code();
}

void cObjectFile::generate_object_code() {
    SICCodeLine* siccode_line;
    for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        if (siccode_line->errors.size() == 0) {

            if (siccode_line->mnemonic == "BYTE") {

            }

            else if (siccode_line->mnemonic == "WORD") {
                append_object_code(siccode_line->object_code, 
                    str_to_int((char*)siccode_line->operand.c_str()));
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
            }

        }
    }
}

void cObjectFile::print_listfile() {
    SICCodeLine* siccode_line;
    for (int i = 0; i < (int)_siccode_lines.size(); ++i) {
        siccode_line = _siccode_lines[i];

        printf("%04X ", siccode_line->address);

        for (int j = 0; j < min(siccode_line->object_code.size(), 6); ++j) {
            printf("%02X", (unsigned char)siccode_line->object_code[j]);
        }

        printf(" %-9s %-8s %-9s %-10s\n",
            siccode_line->label.c_str(),
            siccode_line->mnemonic.c_str(),
            siccode_line->operand.c_str(),
            siccode_line->comment.c_str());

        for (int j = 0; j < (int)siccode_line->errors.size(); ++j) {
            printf(" **** %s\n", siccode_line->errors[j].c_str());
        }
    }
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

cObjectFile::~cObjectFile() {
}
