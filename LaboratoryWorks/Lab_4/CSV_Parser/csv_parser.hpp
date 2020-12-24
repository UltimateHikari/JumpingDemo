#pragma once
#include "recursive.hpp"
#include <iostream>

using namespace std;


template <class... Args>
class CSVParser {
private:
    istream& fin;
    unsigned int skip;
    unsigned int linesInFile = 0;
    //input position indicator for actual start
    unsigned int ipg = 0; 

    void countLines(){
        string dummybuf;
        while(!fin.eof()){
            this->getline(dummybuf,fin);
            linesInFile++;
            if(linesInFile == skip){
                ipg = fin.tellg();
            }
        }
        if(skip > linesInFile){
            throw logic_error("skip impossible: file too small");
        }
        cout << "linesInFile: " <<linesInFile << endl;
        fin.seekg(ipg);
    }

    vector<string> parseStrings(string& str){
        vector<string> fields {""};
        int index = 0;
        for(char c:str){
            if(c == ','){
                fields.push_back("");
                index++;
            }else{
                fields[index].push_back(c);
            }
        }
        return fields;
    }

    tuple<Args...> parseTypes(string& str){
        vector<string> fields = parseStrings(str);
        tuple<Args...> t;
        auto it = fields.begin();
        typeRestore(it, t);
        return t;
    }
    
    bool getline(string& strbuf, istream& fin){
        fin >> strbuf;
        if(fin.eof()){
            return true;
        }
        return false;
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
            parser.getline(strbuf,fin);
            ipg = fin.tellg();
        }
        CSVIterator operator ++(){
            if(isEnd) return *this;
            if(parser.linesInFile - parser.skip > index){
                index++;
                fin.seekg(ipg);
                parser.getline(strbuf,fin);
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
            return parser.parseTypes(strbuf);
        }
    };

    explicit CSVParser(
        istream &file, 
        unsigned int skip_, 
        char row_delim = '\n', 
        char col_delim = ',', 
        char escape = '\"'
    ): fin(file), skip(skip_){
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


