#include "element.h"
#include <vector>
#include <iostream>

bool operator < (Elem & a, Elem& b){
    bool res = (static_cast<int>(a) < static_cast<int>(b));
    if(!res){
        std::cerr << "swapping elems on indexes " << a.get_index() << 
            " and " << b.get_index() << std::endl;
        swap_elem_indexes(a,b);
    }
    return res;
}

void swap_elem_indexes(Elem & a, Elem & b){
    int c = a.get_index();
    a.set_index(b.get_index());
    b.set_index(c);
}

void Elem :: set_index(int index_){
    index = index_;
}

int Elem :: get_index(){
    return index;
}