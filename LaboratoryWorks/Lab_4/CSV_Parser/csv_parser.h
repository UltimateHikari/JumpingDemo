#pragma once
#include "recursive.h"
#include <iostream>

template <class... Args>
class CSVParser {
public:
    CSVParser(
        std::istream &file, 
        int skip, 
        char row_delim = '\n', 
        char col_delim = ',', 
        char escape = '\"'
    );
};


