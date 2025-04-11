//
// Created by njs94 on 4/5/2025.

#ifndef TREEMAKE_H
#define TREEMAKE_H
#include <algorithm>

#include "FileInputOutput.h"
class TreeMake {


public:
    std::vector<LogicValues*> logicValues;
    TreeMake() {
        logicValues = readFile();
        //reverse the vector to make the tree from output to input
        std::reverse(logicValues.begin(), logicValues.end());
    }
    //Fix deconstructor delete
    ~TreeMake() {
        std::erase(logicValues, logicValues.end());
    }

    //Struct for traversal
    struct Traverse : public LogicValues {
        LogicValues* data;
        LogicValues* left;
        LogicValues* right;
    };

    Traverse* buildTree(LogicValues* logic) {
        if(logic == nullptr) {
            return 0;
        }
        Traverse* root = new Traverse;
        root->data = logic;
        if (logic->isEQN == true) {

        }


            return root;
    }






};



#endif //TREEMAKE_H
