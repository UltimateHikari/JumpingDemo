#pragma once
#include <iostream>
#include <tuple>
using namespace std;

template <class Ch, class Tr, class... Args>  
auto& operator<< (basic_ostream<Ch, Tr> &os, const tuple<Args...> &t);  
