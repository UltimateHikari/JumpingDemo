#include "parse.h"

template<class Res>
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

template<>
class CastWrapper{
private:
    string res;
public:
    CastWrapper(const string& str):res(str){}
    
    operator string&(){
        return res;
    }
};

