#include <iostream>
#include <cassert>
#include "tritset.h"
using namespace std;

int main (){
    TritSet my_set(100);
    size_t allocated = my_set.capacity();
    assert(allocated >= 1000*2 / 8 / sizeof(size_t));

    //не выделяет никакой памяти
    my_set[1000] = Trit::Unknown; 
    assert(allocated == my_set.capacity());

    if(my_set[2000] == Trit::True){} 
    assert(allocated == my_set.capacity());

    my_set[1000] = Trit::True; 
    cout << my_set[1000] << " " << my_set.capacity() << endl;

    
    cout << "hello world" << endl;
    return EXIT_SUCCESS;
}