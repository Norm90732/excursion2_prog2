//
// Created by njs94 on 4/5/2025.
//

#ifndef TREEMAKE_H
#define TREEMAKE_H
#include <algorithm>

#include "FileInputOutput.h"
class TreeMake {
private:
    std::vector<LogicValues*> logicValues;


public:
    TreeMake() {
        logicValues = readFile();
        //reverse the vector to make the tree from output to input
        std::reverse(logicValues.begin(), logicValues.end());
    }
    ~TreeMake() {
        std::erase(logicValues, logicValues.end());
    }

    //Struct for traversal
    struct Traverse {
        LogicValues* data;
        LogicValues* left;
        LogicValues* right;
    };
    //Makes the netlist tree recursively
    void traverseMake(LogicValues* root) {
        if (root == nullptr)
            return;

        //Implement recursion for this
    }




};



#endif //TREEMAKE_H
