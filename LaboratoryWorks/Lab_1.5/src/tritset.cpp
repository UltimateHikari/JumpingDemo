#include <cstddef>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "tritset.h"

using namespace std;

size_t ind_to_chunk(size_t index){
    return ceil(static_cast<double>(2 * index) / (8 * sizeof(size_t)));
}

int get_inner_ind(int index){
    //index inside of chunk
    return ((2 * index) % (8 * sizeof(size_t)));
}

size_t form_zero_mask(int index){
    return ~(3 << get_inner_ind(index));
}

size_t form_filler_mask(int index, Trit t){
    return (static_cast<size_t>(t) << get_inner_ind(index));
}

void TritSet :: resize(std::size_t index){
    size_t * expanded_array = new size_t[ind_to_chunk(index)];
    for(int i = 0; i < ind_to_chunk(index); i++){
        expanded_array[i] = 0;
    }
    int last_preserved_chunk = min(ind_to_chunk(index), ind_to_chunk(actual_size));
    for(int i = 0; i < last_preserved_chunk; i++){
        expanded_array[i] = array[i];
    }
    delete[] array;
    array = expanded_array;
    actual_size = index;
}

TritSet::reference :: reference (int in_ind, TritSet& in_parent): parent(in_parent){
    index = in_ind;
}

TritSet :: reference& TritSet::reference :: operator = (Trit t){
    if(index > parent.actual_size && ind_to_chunk){
        if(t == Trit::Unknown) return *this;
        parent.resize(index);
    }
    size_t * chunk = parent.array + ind_to_chunk(index);
    *chunk &= form_zero_mask(index);
    *chunk |= form_filler_mask(index, t);
    return *this;
    // no out_of_range just unknown lol
}

TritSet::reference :: operator int(){
    int res = (parent.array[ind_to_chunk(index)] >> get_inner_ind(index))
            & static_cast<size_t>(3);
    return res;
}

TritSet::reference :: operator Trit(){
    int res = (parent.array[ind_to_chunk(index)] >> get_inner_ind(index))
            & static_cast<size_t>(3);
    return static_cast<Trit>(res);
}

TritSet :: TritSet(size_t size){
    size_t a = ind_to_chunk(size);
    array = new size_t[ind_to_chunk(size)];
    for(int i = 0; i < ind_to_chunk(size); i++){
        array[i] = 0;
    }
    min_size = size;
    actual_size = size;
}

TritSet :: ~TritSet(){
    delete[] array;
}

size_t TritSet :: underlaying_capacity(){
    return ind_to_chunk(actual_size); // not all bits of it are used
}

size_t TritSet :: capacity(){
    return actual_size; // more logical as for container
}

TritSet::reference TritSet :: operator [](size_t index){
    return TritSet::reference(index, *this);
}

void TritSet :: shrink(){
    int new_size = min_size;
    for(int i = min_size - 1; i < actual_size; i++){
        if((*this)[i] != Trit::Unknown){ 
            new_size = i + 1;
        }
    }
    resize(new_size);
}