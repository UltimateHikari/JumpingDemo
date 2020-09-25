#include <cstddef>
#include <cmath>
#include "tritset.h"

using namespace std;

size_t ind_to_chunk (size_t index){
    return ceil(2*index/sizeof(size_t));
}

size_t & TritSet :: get_chunk(size_t index){
    return this->array[2*index/sizeof(size_t)];
}

bool TritSet :: widen_to_fit(std::size_t index){
    size_t * expanded_array = new size_t[ind_to_chunk(index)]; //mb return false?
    for(int i = 0; i < ind_to_chunk(this->actual_size); i++){
        expanded_array[i] = this->array[i];
    }
    delete this->array;
    this->array = expanded_array;
    this->actual_size = ind_to_chunk(index)*sizeof(size_t)/2;
    return true;
}

TritSet::reference :: reference (int in_ind, TritSet& in_parent): parent(in_parent){
    index = in_ind;
}

size_t trit_bitify(Trit t){
    return static_cast<size_t>(t);
}
int get_inner_ind(size_t index){
    // ind of first of two bits but going from backwards
    return 2*index % (8*sizeof(size_t));
}

TritSet :: reference& TritSet::reference :: operator = (Trit t){
    if (t != Trit::Unknown){
        if(index > parent.actual_size){
            parent.widen_to_fit(index);
        }
        size_t & in_chunk = parent.get_chunk(index);
        int outer_index = 8*sizeof(size_t) - get_inner_ind(index); // num of GE bits
        // ********
        // **..**** // inner = 4;
        size_t lesser_trits = (in_chunk << outer_index) >> outer_index; // dumb i agree
        size_t larger_trits = in_chunk >> get_inner_ind(index);
        size_t a = get_inner_ind(index);
        size_t current_trit = (3 & trit_bitify(t)) << get_inner_ind(index);
        in_chunk = lesser_trits | current_trit | larger_trits;
    }
    return *this;
    // no out_of_range just unknown lol
}

TritSet::reference :: operator int(){
    int res = (parent.get_chunk(index) >> get_inner_ind(index))
            & static_cast<size_t>(3);
    return res;
}

TritSet::reference :: operator Trit(){
    int res = (parent.get_chunk(index) >> get_inner_ind(index))
            & static_cast<size_t>(3);
    return static_cast<Trit>(res);
}

TritSet :: TritSet(size_t size){
    array = new size_t[size];
    min_size = size;
    actual_size = size;
}

TritSet :: ~TritSet(){
    delete array;
}

size_t TritSet :: capacity(){
    return actual_size;
}

// constexpr Trit TritSet :: operator [](size_t index) const{
//     TritSet::reference ref(index, *this);
//     return ref;
// }

TritSet::reference TritSet :: operator [](size_t index){
    TritSet::reference ref(index, *this);
    return ref;
}

// void TritSet :: shrink(){
//     int new_size = min_size;
//     for(int i = min_size - 1; i < actual_size; i++){
//         if(this[i] != Trit::Unknown){
//             new_size = i + 1;
//         }
//     }
// }