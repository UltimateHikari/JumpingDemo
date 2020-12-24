#pragma once
#include "recursive.hpp"
#include "demangle.hpp"
#include <iostream>
#include <string>
// #include <typeinfo>
// #include <utility>

using namespace std;

//recursive out
template <class Ch, class Tr, class... Args, size_t I0>  
void tuple_out(basic_ostream<Ch, Tr> &os, const tuple<Args...> &t, index_sequence<I0>){
    //os << type(get<I0>(t));
    os << get<I0>(t);
}

template <class Ch, class Tr, class... Args, size_t I0, size_t... I>  
void tuple_out(basic_ostream<Ch, Tr> &os, const tuple<Args...> &t, index_sequence<I0, I...>){
    //os << type(get<I0>(t)) << ", ";
    os << get<I0>(t) << ", ";
    tuple_out(os, t, index_sequence<I...>{});
}

template <class Ch, class Tr, class... Args>  
auto& operator<< (basic_ostream<Ch, Tr> &os, const tuple<Args...> &t){
    os << "[ ";
    tuple_out(os, t, index_sequence_for<Args...>{});
    os << " ]";
    return os;
}

template <class... Args>
class CSVParser {
private:
    istream& fin;
    unsigned int skip;
    unsigned int linesInFile = 0;
    //input position indicator for actual start
    unsigned int ipg = 0; 
    char row_delim = '\n';
    char col_delim;
    char escape;
    bool lookForDelims = true;

    void countLines(){
        string dummybuf;
        while(!fin.eof()){
            getline(fin,dummybuf);
            linesInFile++;
            if(linesInFile == skip){
                ipg = fin.tellg();
            }
        }
        if(skip > linesInFile){
            throw logic_error("skip impossible: file too small");
        }
        //cout << "linesInFile: " << linesInFile << endl;
        fin.seekg(ipg);
    }

    vector<string> parseStrings(string& str, int lineIndex){
        vector<string> fields {""};
        int index = 0;
        int escapeStartField;
        for(char c:str){
            if(lookForDelims){
                if(c == col_delim){
                    fields.push_back("");
                    index++;
                }else if(c == escape){
                    lookForDelims = false;
                    escapeStartField = index + 1;
                }else{
                    fields[index].push_back(c);
                }
            }else{
                if(c == escape){
                    lookForDelims = true;
                }else{
                    fields[index].push_back(c);
                }
            }
        }
        if(lookForDelims == false) 
            throw logic_error("escape sequence left open on line " + to_string(lineIndex) 
            + " ( opened at field " + to_string(escapeStartField) + " )");
        if(fields.size() != tuple_size<tuple<Args...>>::value) 
            throw logic_error("wrong number of fields on line " + to_string(lineIndex));
        return fields;
    }

    tuple<Args...> parseTypes(string& str,int index){
        vector<string> fields = parseStrings(str, index);
        tuple<Args...> t;
        auto it = fields.begin();
        typeRestore(it, t);
        return t;
    }

public:
    class CSVIterator{
    private:
        string strbuf;
        istream& fin;
        CSVParser& parser;
        int index = 1; //from 1
        bool isEnd;
        //input position indicator for last read
        unsigned int ipg = 0;
        
    public:
        CSVIterator(
            istream& fin_,
            CSVParser<Args...>& parser_,
            bool isEnd_ = false
        ):parser(parser_), fin(fin_), isEnd(isEnd_)
        {
            ipg = parser.ipg;
            fin.seekg(ipg);
            getline(fin,strbuf);
            ipg = fin.tellg();
        }
        CSVIterator operator ++(){
            if(isEnd) return *this;
            if(parser.linesInFile - parser.skip > index){
                index++;
                fin.seekg(ipg);
                getline(fin,strbuf);
                ipg = fin.tellg();
            }else{
                //be come.. end
                isEnd = true;
                index = 1;
            }
            //cout << index << " " << isEnd <<  endl;
            return *this;
        }
        bool operator ==(const CSVIterator& other){
            // cout << endl << (&(parser) == &(other.parser)) << 
            //     (isEnd == other.isEnd) <<
            //     (index == other.index) <<
            //     (strbuf == other.strbuf) <<  endl;
            if(
                &(parser) == &(other.parser) 
                && isEnd == other.isEnd 
                && index == other.index
                )
            {
                return true;
            }
            return false;
        }
        bool operator !=(const CSVIterator& other){
            return !(*this == other);
        }
        tuple<Args...> operator*(){
            //cant have exceptions in Detroit
            if(isEnd) return tuple<Args...>();
            return parser.parseTypes(strbuf, index);
        }
    };

    explicit constexpr CSVParser(
        istream &file, 
        unsigned int skip_, 
        /*char row_delim_ = '\n', */// defined by csv docs
        char col_delim_ = ',',
        char escape_ = '\"'
    ): fin(file), skip(skip_), 
        col_delim(col_delim_), 
        escape(escape_)
    {
        countLines();
    };

    CSVIterator begin(){
        if(linesInFile == skip){
            return end(); //nothing to do here lol
        }
        return CSVIterator(fin, *this);
    }
    CSVIterator end(){
        return CSVIterator(fin, *this, true);
    }
};


