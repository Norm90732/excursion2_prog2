//
// Created by njs94 on 4/5/2025.

#ifndef TREEMAKE_H
#define TREEMAKE_H
#include <algorithm>

#include "FileInputOutput.h"

//Struct for traversal
struct Traverse : public LogicValues {
    LogicValues* data;
    Traverse* left;
    Traverse* right;
};


class TreeMake {
public:
    std::vector<LogicValues*> logicValues;
    Traverse* root;
    TreeMake() {
        logicValues = readFile();
        //reverse the vector to make the tree from output to input
        std::reverse(logicValues.begin(), logicValues.end());
        root = buildTree(logicValues[0]);
        NandNotTreeBuilder(root);
    }

    Traverse* buildTree(LogicValues* logic) {
        if(logic == nullptr) {
            return nullptr;
        }

        Traverse* node = new Traverse;
        node->data = logic;
        if (logic->inputs.size() == 2) {
            node->left = buildTree(logic->inputs[0]);
            node->right = buildTree(logic->inputs[1]);
        }
        else if (logic->inputs.size() == 1) {
            node->left = buildTree(logic->inputs[0]);
            node->right = nullptr;
        }
        else if (logic->inputs.size() == 0) {
            node->left = nullptr;
            node->right = nullptr;
        }


        return node;
    }


    void printTree(Traverse* root) {
        if (root == nullptr) {
            return;
        }
        printTree(root->left);
        printTree(root->right);
        std::cout << root->data->name << " ";
        std::cout << root->data->type << " ";
        std::cout << root->data->NOT << " , ";
    }

    void NandNotTreeBuilder(Traverse* root) {
        if (root == nullptr) {
            return;
        }

        if (root->data->inputs.size() == 2) {
            if(root->data->type == "AND") {
                root->data->type = "NAND";
                root->data->NOT = !root->data->NOT;
            }
            else if (root->data->type == "OR") {
                root->data->type = "NAND";
                root->left->NOT = !root->left->NOT;
                root->right->NOT = !root->right->NOT;
            }
        }

        NandNotTreeBuilder(root->left);
        NandNotTreeBuilder(root->right);

    }
};
#endif //TREEMAKE_H