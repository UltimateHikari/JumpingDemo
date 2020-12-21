// /#include "CSV_Parser/csv_parser.h"
#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>
#include "CSV_Parser/demangle.hpp"
#include "CSV_Parser/recursive.hpp"

using namespace std;

//recursive out
template <class Ch, class Tr, class... Args, size_t I0>  
void tuple_out(basic_ostream<Ch, Tr> &os, const tuple<Args...> &t, index_sequence<I0>){
    os << type(get<I0>(t));
}

template <class Ch, class Tr, class... Args, size_t I0, size_t... I>  
void tuple_out(basic_ostream<Ch, Tr> &os, const tuple<Args...> &t, index_sequence<I0, I...>){
    os << type(get<I0>(t)) << ", ";
    tuple_out(os, t, index_sequence<I...>{});
}

template <class Ch, class Tr, class... Args>  
auto& operator<< (basic_ostream<Ch, Tr> &os, const tuple<Args...> &t){
    os << "[ ";
    tuple_out(os, t, index_sequence_for<Args...>{});
    os << " ]\n";
    return os;
}


vector<string> parseStrings(string& str){
    vector<string> fields {""};
    int index = 0;
    for(char c:str){
        if(c == ','){
            fields.push_back("");
            index++;
        }else{
            fields[index].push_back(c);
        }
    }
    return fields;
}


//template <class...Args>
// vector<Args...>parseTypes(){

// }



int main() {
    tuple<double> a(1.0);
    tuple<int,float,double> t(1, 2.0f, 3.0);
    cout << t;
    ifstream fin("../in.txt");
    if(!fin){
        cout << "unable to open\n";
    }
    string bufstr;
    getline(fin, bufstr);
    vector <string> v = parseStrings(bufstr);
    for(auto i:v){
        cout << i << endl;
    }

//       std::ifstream file("test.csv");  
//   CSVParser<int, std::string> parser(file, 0 /*skip first lines count*/);  
//   for (tuple<int, std::string> rs : parser) {  
//     std::cout << rs << std::endl;  
    return 0;
} 