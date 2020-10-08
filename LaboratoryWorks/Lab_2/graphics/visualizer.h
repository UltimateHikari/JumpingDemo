#pragma once
#include <vector>
#include <list>
#include <memory>

#include "window.h"
#include "commands.h"

class Visualizer {
public:
    template <class Iter>
    void setContainer(Iter begin, Iter end);

    void pushCommand(Command *command);
    static Visualizer& getInstance();
    void draw();
    void clear();
private:
    Visualizer();
    std::list< std::unique_ptr<Command> > command_list;
    std::vector<int> elements;
    std::vector<Color> colors;  
};

template <class Iter>
void Visualizer::setContainer(Iter begin, Iter end) {
    while (begin != end) {
        elements.push_back(*begin);
        // /std::cerr << *begin << " ";
        begin++;
    }
    colors.resize(elements.size(), Color{255, 255, 255, 255});
}
