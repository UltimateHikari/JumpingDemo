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
    return ~(static_cast<size_t>(3) << get_inner_ind(index));
}

size_t form_filler_mask(int index, Trit t){
    return (static_cast<size_t>(t) << get_inner_ind(index));
}

void TritSet :: resize(std::size_t index){
    if(ind_to_chunk(index) == underlaying_capacity()) return;
    size_t * expanded_array = new size_t[ind_to_chunk(index)];
    for(int i = 0; i < ind_to_chunk(index); i++){
        expanded_array[i] = 0;
    }
    int last_significant_chunk = min(ind_to_chunk(index), ind_to_chunk(actual_size));
    for(int i = 0; i < last_significant_chunk; i++){
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

TritSet::reference :: operator int() const{
    size_t res = (parent.array[ind_to_chunk(index)] >> get_inner_ind(index))
            & static_cast<size_t>(3);
    return static_cast<int>(res);
}

TritSet::reference :: operator Trit() const{
    size_t res = (parent.array[ind_to_chunk(index)] >> get_inner_ind(index))
            & static_cast<size_t>(3);
    return static_cast<Trit>(res);
}

TritSet :: TritSet(size_t size){
    array = new size_t[ind_to_chunk(size)];
    for(int i = 0; i < ind_to_chunk(size); i++){
        array[i] = 0;
    }
    min_size = size;
    actual_size = size;
}

TritSet :: TritSet(const TritSet& set): 
    TritSet :: TritSet(set.capacity()){
    for(int i = 0; i < capacity(); i++){
        (*this)[i] = set[i];
    }
}

TritSet :: ~TritSet(){
    delete[] array;
}

size_t TritSet :: underlaying_capacity() const{
    return ind_to_chunk(actual_size); // not all bits of it are used
}

size_t TritSet :: capacity() const{
    return actual_size; // more logical as for container
}

TritSet::reference TritSet :: operator [](size_t index){
    return TritSet::reference(index, *this);
}

Trit TritSet :: operator [](size_t index) const{
    return TritSet::reference(index, *(const_cast<TritSet*>(this)));
}

std::size_t TritSet :: length() const{ //rework
    int new_size = 0;
    for(int i = 0; i < actual_size; i++){
        if((*this)[i] != Trit::Unknown){ 
            new_size = i + 1;
        }
    }
    return new_size;
}

void TritSet :: shrink(){
    resize(max(length(), min_size));
}

TritSet& TritSet :: operator &=(const TritSet& A){
    if(A.length() > capacity()){
        resize(A.length());
    }
    for(int i = 0; i < capacity(); i++){
        if((*this)[i] == Trit::False || A[i] == Trit::False){
            (*this)[i] = Trit::False;
        }
        if((*this)[i] == Trit::True && A[i] == Trit::True){
            (*this)[i] = Trit::True;
        }
    }
    return *this;
}

TritSet& TritSet :: operator |=(const TritSet& A){
    if(A.length() > capacity()){
        resize(A.length());
    }
    for(int i = 0; i < capacity(); i++){
        if((*this)[i] == Trit::False && A[i] == Trit::False){
            (*this)[i] = Trit::False;
        }
        if((*this)[i] == Trit::True || A[i] == Trit::True){
            (*this)[i] = Trit::True;
        }
    }
    return *this;
}

TritSet& TritSet :: operator ~(){
    for(int i = 0; i < capacity(); i++){
        if((*this)[i] == Trit::False){
            (*this)[i] = Trit::True;
        }
        if((*this)[i] == Trit::True){
            (*this)[i] = Trit::False;
        }
    }
    return *this;
}

TritSet operator & (const TritSet& A, const TritSet& B){
    return TritSet(A) &= B;
}

TritSet operator | (const TritSet& A, const TritSet& B){
    return TritSet(A) |= B;
}

unordered_map <Trit, int> TritSet :: cardinality(){
    unordered_map <Trit, int> res;
    for(int i = 0; i < length(); i++){
        res[(*this)[i]]++;
    }
    return res;
}

int TritSet :: cardinality(Trit value){
    return cardinality()[value];
}

void TritSet::trim(size_t last_index){
    for(int i = last_index; i < length(); i++){
        (*this)[i] = Trit::Unknown;
    }
}

bool operator == (const TritSet::reference& a, const TritSet::reference& b){
    return static_cast<Trit>(a) == b;
}
bool operator != (const TritSet::reference& a, const TritSet::reference& b){
    return static_cast<Trit>(a) != b;
}