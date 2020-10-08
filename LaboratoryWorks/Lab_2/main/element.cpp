#include <vector>
#include <iostream>

#include "element.h"
#include "commands.h"
#include "visualizer.h"

Elem& Elem :: operator = (const Elem& el){
    if(el != *this){
        //std::cerr << "Assigning " << el << " at "  <<  el.get_index() << " to "  << (*this) << " at " << get_index() << std::endl;
        Visualizer::getInstance().pushCommand(new Assign(get_index(), el));
        data = static_cast<int>(el);
    }
    return *this;
}

bool operator < (Elem & a, Elem& b){
    bool res = (static_cast<int>(a) < static_cast<int>(b));
    //std::cerr << "Comparing " << a << " at "  <<  a.get_index() << " and "  << b << " at " << b.get_index() << std::endl;
    Visualizer::getInstance().pushCommand(new ComparisonLess(a.get_index(), b.get_index()));
    return res;
}

int Elem :: get_index() const {
    return index;
}