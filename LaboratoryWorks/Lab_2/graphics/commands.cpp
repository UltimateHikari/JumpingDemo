#include <vector>
#include "commands.h"

BinaryCommand :: BinaryCommand(int lField_, int rField_)
    : lField(lField_), rField(rField_) {}

void Assign :: execute(std::vector<int> &container, std::vector<Color> &colors) const {
    container[lField] = rField;
}

void ComparisonLess :: execute(std::vector<int> &container, std::vector<Color> &colors) const {
    colors[lField].set(128, 0, 128 , 1); // purple > springgreen (aka 0-255-0)
    colors[rField].set(128, 0, 128 , 1);
}