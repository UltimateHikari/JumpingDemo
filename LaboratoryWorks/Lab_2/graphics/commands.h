#pragma once
#include <vector> 
#include "window.h"

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(std::vector<int> &container, std::vector<Color> &colors) const = 0;
};

class BinaryCommand : public Command {
protected:
    int lField;
    int rField;
public:
    BinaryCommand(int lField, int rField);
};

class Assign : public BinaryCommand {
public:
    Assign(int ind, int val)
        : BinaryCommand(ind, val){};
    void execute(std::vector<int> &container, std::vector<Color> &colors) const;
};

class ComparisonLess : public BinaryCommand {
public:
    ComparisonLess(int ind1, int ind2)
        : BinaryCommand(ind1, ind2){};
    void execute(std::vector<int> &container, std::vector<Color> &colors) const;
};