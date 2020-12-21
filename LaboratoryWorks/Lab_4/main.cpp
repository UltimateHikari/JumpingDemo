#include "CSV_Parser/csv_parser.h"
#include <iostream>
#include <tuple>
#include <string>
#include <typeinfo>
#include <utility>
#include "CSV_Parser/demangle.hpp"

using namespace std;


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

int main() {
    tuple<double> a(1.0);
    tuple<int,float,double> t(1, 2.0f, 3.0);
    cout << t;
    return 0;
} 