//
// Created by njs94 on 4/5/2025.

#ifndef TREEMAKE_H
#define TREEMAKE_H
#include <algorithm>
#include <string>

#include "FileInputOutput.h"

const int NOTCost = 2;
const int NAND2Cost = 3;
const int AND2Cost = 4;
const int NOR2Cost = 6;
const int OR2Cost = 4;
const int AOI21Cost = 7;
const int AOI22Cost = 7;

/* struct LogicValues {
    std::string name;
    std::string type;
    std::vector<LogicValues*> inputs;
    bool NOT = false;
    bool isOutput;
    bool isEQN;
};
 */

//Struct for traversal
struct Traverse : public LogicValues {
    LogicValues* data;
    Traverse* left;
    Traverse* right;
    int minCost = -1;
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
            if (root->data->type == "OR") {
                root->data->type = "NAND";
                Traverse* oldChild1 = root->left;
                Traverse* oldChild2 = root->right;
                Traverse* newChild1 = new Traverse;
                LogicValues* not1 = new LogicValues;
                not1->type = "NOT";
                not1->isEQN = true;
                newChild1->data = not1;
                root->left = newChild1;
                newChild1->left = oldChild1;
                Traverse* newChild2 = new Traverse;
                LogicValues* not2 = new LogicValues;
                not2->type = "NOT";
                not2->isEQN = true;
                newChild2->data = not2;
                root->right = newChild2;
                newChild2->left = oldChild2;
            }
        }
        if (root->left != nullptr) {
            if (root->left->data->type == "AND") {
                root->left->data->type = "NAND";
                Traverse* oldChild = root->left;
                Traverse* newChild = new Traverse;
                LogicValues* notGate = new LogicValues;
                notGate->type = "NOT";
                notGate->isEQN = true;
                newChild->data = notGate;
                root->left = newChild;
                newChild->left = oldChild;
            }
        }

        if (root->right != nullptr) {
            if (root->right->type == "AND") {
                Traverse* oldChild = root->right;
                Traverse* newChild = new Traverse;
                LogicValues* notGate = new LogicValues;
                notGate->isEQN = true;
                notGate->type = "NOT";
                newChild->data = notGate;
                root->right = newChild;
                newChild->left = oldChild;
            }
        }

        if (root->left != nullptr) {
            NandNotTreeBuilder(root->left);
        }
        if (root->right != nullptr) {
            NandNotTreeBuilder(root->right);
        }
    }

    std::vector<std::string> getTopologies(Traverse* root) {
        std::vector<std::string> topologies{};
        std::vector<Traverse*> children{};
        if (root->data->type == "NOT") {
            topologies.push_back("NOT");
            //check for nullptr
            children.push_back(root->left);
        }
        if (root->data->type == "NAND") {
            topologies.push_back("NAND2");
            children.push_back(root->left);
            children.push_back(root->right);
        }
        if(root->data->type == "NOT" && root->left->data->type == "NAND") {
            topologies.push_back("AND2");
            children.push_back(root->left);
            children.push_back(root->right);
        }
        if (root->data->type == "NOT" && root->left->data->type == "NAND" && root->left->left->data->type == "NOT" &&
            root->left->right->data->type == "NOT") {
            topologies.push_back("NOR2");
            children.push_back(root->left->left->left);
            children.push_back(root->left->right->left);
        }
        if (root->data->type == "NAND" &&  root->left->data->type == "NOT" && root->right->data->type == "NOT") {
            topologies.push_back("OR2");
            children.push_back(root->left->left);
            children.push_back(root->right->left);
        }
        if (root->data->type == "NOT" && root->left->data->type == "NAND" && root->left->left->data->type == "NAND" &&
            root->left->right->data->type == "NOT") {
            topologies.push_back("AOI21");
            children.push_back(root->left->left->left);
            children.push_back(root->left->left->right);
            children.push_back(root->left->right->left);
        }
        if (root->data->type == "NOT" && root->left->data->type == "NAND" && root->left->left->data->type == "NAND" &&
            root->left->right->data->type == "NAND") {
            topologies.push_back("AOI22");
            children.push_back(root->left->left->left);
            children.push_back(root->left->left->right);
            children.push_back(root->left->right->left);
            children.push_back(root->left->right->right);
        }
        return topologies;
    }


    int costMin(Traverse* node) {
        std::string topology = "";
        std::vector<int> dp;

        if (node ->data == nullptr or node->type == "INPUT") {
            return 0;
        }
        if(node->minCost != -1) {
            return node->minCost;
        }
        int costLeft = 0;
        int costRight = 0;
    }
};
#endif //TREEMAKE_H