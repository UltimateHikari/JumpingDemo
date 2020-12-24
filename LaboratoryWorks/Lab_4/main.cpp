// /#include "CSV_Parser/csv_parser.h"
#include <iostream>
#include <fstream>
#include <tuple>
#include "CSV_Parser/csv_parser.hpp"
using namespace std;

int main() {
    // tuple<double> a(1.0);
    // tuple<int,float,double> t(1, 2.0f, 3.0);
    // cout << t << endl;
    ifstream fin("../in.txt");
    if(!fin){
        cout << "unable to open\n";
    }
    CSVParser<int, std::string, double> parser(fin, 0);
    for(auto i:parser){
        cout << i << endl;
    }  
    return 0;
} 