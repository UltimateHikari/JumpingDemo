#pragma once
//#include "../graphics/visualizer.h"

class Elem {
private:
	int data;
	int index;
public:
    Elem(int data_, int index_)
	 : data(data_), index(index_) {}

    operator int() const {
        return data;
    }
    Elem& operator = (const Elem& el);
    int get_index() const;
};

bool operator < (Elem & a, Elem& b);