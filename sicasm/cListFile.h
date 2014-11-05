#pragma once
#include "sicasm.h"
#include "cSourceFile.h"

using namespace std;

class cListFile: public cSourceFile {
public:
    cListFile(char* filename);
    ~cListFile();

    virtual void print_listfile();

private:
    bool parse_siccode_lines();
    int hex_to_int(char* hex);

    bool _end_set;
    bool _start_set;
    int _current_address;

    bool is_numeric(string& str);
    bool is_hex_number(string& str);
    bool is_word_str(string& str);
    bool is_alpha(string& str);
    void construct_symbol_table();
    bool parse_instructions();

protected:
    int _start_address;
    map<string, int> _symbols_table;
    map<string, SICOpCode*> _opcodes_table;

    int str_to_int(char* str);
};

