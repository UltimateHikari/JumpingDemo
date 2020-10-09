#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>

#include "../graphics/visualizer.h"
#include "element.h"

std::vector<Elem> create(int size) {
    std::srand(1000);
    std::vector<Elem> vect;
    std::set<int> used;
    int index = 0;
    while (vect.size() != size) {
        int val = std::rand() % size;
        if (!used.count(val)) {
            used.insert(val);
            vect.push_back(Elem(val, index));
            index++;
        }
    }
    Visualizer::getInstance().setContainer(vect.begin(), vect.end());
    Visualizer::getInstance().clear();
    return vect;
}

int main(int argc, char **argv) {
    std::vector<Elem> vect{create(100)};  
    Visualizer::getInstance().clear();
    //std::sort(vect.begin(), vect.end());
    std::stable_sort(vect.begin(), vect.end());
    Visualizer::getInstance().draw();
    return 0;
}

// int zsh_main(int argc, char **argv){
//     std::vector<Elem> vect{create(5)};
//     std::sort(vect.begin(), vect.end());
//     for(auto i : vect) std::cerr << i;
//     std::cout << "Hello World" << std::endl;
//     return 0;
// }