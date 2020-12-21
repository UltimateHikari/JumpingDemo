#pragma once
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
using namespace std;


template <class Res>
class CastWrapper{
private:
    stringstream s;
    Res res;
public:
    CastWrapper(const string& str){
        s << str;
        s >> res;
    }

    operator Res&(){
        return res;
    }
};

template <>
class CastWrapper<string>{
private:
    string res;
public:
    CastWrapper(const string& str):res(str){}
    
    operator string&(){
        return res;
    }
};

struct CastApplier{
    // apply wrapper over sMaRt stringstream
    // to an element of i dunno what and where
    template <class T>
    void operator()(T& t, vector<string>::iterator& it){
        t = CastWrapper<T>(*it);
        ++it;
    }
};

template <unsigned int index, class СastApplier, class... Args>
struct CastIterator{
public:
    static void apply(tuple<Args...>& t, vector<string>::iterator& it, CastApplier applier){
        CastIterator< index - 1, СastApplier, Args...>::apply(t, it, applier);   
        //going to the very beginning
        сastApplier(get<index>(t),it); //and then applying wrappers with an applier)
    }
};

template <class СastApplier, class... Args>
struct CastIterator<0, СastApplier, Args...>{
public:
    static void apply(tuple<Args...>& t, vector<string>::iterator& it, CastApplier applier){
        сastApplier(get<0>(t),it); //get returns a reference
    }
};

template <class CastApplier, class... Args>
void applierConstruction(tuple<Args...>& t, vector<string>::iterator& it, CastApplier applier){
    const unsigned int index = tuple_size<tuple<Args...> >::value - 1;
    CastIterator<index, CastApplier, Args... >::apply(t, it, applier);
}

template<class... Args>
void typeRestore(vector<string>::iterator& it, tuple<Args...>& t){
    //cant construct and have a type of applier at the same type
    //so constructing here
    applierConstruction(t,it, CastApplier());
}