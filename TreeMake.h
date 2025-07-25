//
// Created by njs94 on 4/5/2025.

#ifndef TREEMAKE_H
#define TREEMAKE_H
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include "FileInputOutput.h"

//Technology Library of Gates
const int NOTCost = 2;
const int NAND2Cost = 3;
const int AND2Cost = 4;
const int NOR2Cost = 6;
const int OR2Cost = 4;
const int AOI21Cost = 7;
const int AOI22Cost = 7;


//Struct for traversal
struct Traverse : public LogicValues {
    LogicValues* data;
    Traverse* left;
    Traverse* right;
    int minCost = 9999999;
};

//Stores Gate info in the form of topologies and their children.
struct GateInfo {
    std::vector<std::string> topologies{};
    std::vector<std::vector<Traverse*>> allChildren{};
};

//Class that contains operations to make a netlist tree, nand-not tree, and find the minimum cost
class TreeMake {
public:
    //Logic values input
    std::vector<LogicValues*> logicValues;
    //makes a hashmap that stores costs
    std::unordered_map<std::string, int> costs;
    int nandNotCounter;

    Traverse* root;
    TreeMake() {
        nandNotCounter = 0;
        logicValues = readFile();
        //reverse the vector to make the tree from output to input
        std::reverse(logicValues.begin(), logicValues.end());
        //builds the root with the output of the netlist equation
        root = buildTree(logicValues[0]);
        NandNotTreeBuilder(root);
        //Technology library with a hashmap
        costs["NOT"] = 2;
        costs["NAND2"] = 3;
        costs["AND2"] = 4;
        costs["NOR2"] = 6;
        costs["OR2"] = 4;
        costs["AOI21"] = 7;
        costs["AOI22"] = 7;
        costMin(root);
    }
    //Builds the tree, goes left and right depending on the input size. We construct a left biased tree.
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
        else {
            node->left = nullptr;
            node->right = nullptr;
        }


        return node;
    }

    //Post Order traversal print function for checking our work
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

    //Converts the build tree into a nand not tree
    void NandNotTreeBuilder(Traverse* root) {
        if (root == nullptr) {
            return;
        }
        //NandNotCounter is used to check a node
        if (nandNotCounter == 0) {
            nandNotCounter = 1;
            //If its and, replace it with nand, attach a not above the new NAND.
            if (root->data->type == "AND") {
                root->data->type = "NAND";
                Traverse* newNode = new Traverse;
                LogicValues* not1 = new LogicValues;
                not1->type = "NOT";
                not1->isEQN = true;
                newNode->data = not1;
                newNode->left = root;
                newNode->right = nullptr;
                this->root = newNode;
            }
        }
        //If its OR, replace the inputs with NOT
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
            newChild1->right = nullptr;
            Traverse* newChild2 = new Traverse;
            LogicValues* not2 = new LogicValues;
            not2->type = "NOT";
            not2->isEQN = true;
            newChild2->data = not2;
            root->right = newChild2;
            newChild2->left = oldChild2;
            newChild2->right = nullptr;
        }
        if (root->left && root->left->data->type == "AND") {
            root->left->data->type = "NAND";
            Traverse* oldChild = root->left;
            Traverse* newChild = new Traverse;
            LogicValues* notGate = new LogicValues;
            notGate->type = "NOT";
            notGate->isEQN = true;
            newChild->data = notGate;
            root->left = newChild;
            newChild->left = oldChild;
            newChild->right = nullptr;
        }
        if (root->right && root->right->data->type == "AND") {
            root->right->data->type = "NAND";
            Traverse* oldChild = root->right;
            Traverse* newChild = new Traverse;
            LogicValues* notGate = new LogicValues;
            notGate->isEQN = true;
            notGate->type = "NOT";
            newChild->data = notGate;
            root->right = newChild;
            newChild->left = oldChild;
            newChild->right = nullptr;
        }
        nandNotCounter += 1;
        if (root->left) {
            NandNotTreeBuilder(root->left);
        }
        if (root->right) {
            NandNotTreeBuilder(root->right);
        }
    }

    //Defines the topologies based on the technology library, it identifies the topologies
    GateInfo getTopologies(Traverse* root) {
        std::vector<std::string> topologies{};
        std::vector<std::vector<Traverse*>> allChildren{};
        if (root->data->type == "NOT") {
            topologies.push_back("NOT");
            //check for nullptr
            std::vector<Traverse*> children{};
            children.push_back(root->left);
            allChildren.push_back(children);
        }
        if (root->data->type == "NAND") {
            topologies.push_back("NAND2");
            std::vector<Traverse*> children{};
            children.push_back(root->left);
            children.push_back(root->right);
            allChildren.push_back(children);
        }
        if(root->data->type == "NOT" && root->left != nullptr && root->left->data->type == "NAND") {
            topologies.push_back("AND2");
            std::vector<Traverse*> children;
            children.push_back(root->left->left);
            children.push_back(root->left->right);
            allChildren.push_back(children);
        }
        if (root->data->type == "NOT" && root->left->data->type == "NAND" && root->left->left->data->type == "NOT" &&
            root->left->right->data->type == "NOT") {
            topologies.push_back("NOR2");
            std::vector<Traverse*> children;
            children.push_back(root->left->left->left);
            children.push_back(root->left->right->left);
            allChildren.push_back(children);
        }
        if (root->data->type == "NAND" &&  root->left->data->type == "NOT" && root->right->data->type == "NOT") {
            topologies.push_back("OR2");
            std::vector<Traverse*> children;
            children.push_back(root->left->left);
            children.push_back(root->right->left);
            allChildren.push_back(children);
        }
        if (root->data->type == "NOT" && root->left->data->type == "NAND" && root->left->left->data->type == "NAND" &&
            root->left->right->data->type == "NOT") {
            topologies.push_back("AOI21");
            std::vector<Traverse*> children;
            children.push_back(root->left->left->left);
            children.push_back(root->left->left->right);
            children.push_back(root->left->right->left);
            allChildren.push_back(children);
        }
        if (root->data->type == "NOT" && root->left->data->type == "NAND" && root->left->left->data->type == "NAND" &&
            root->left->right->data->type == "NAND") {
            topologies.push_back("AOI22");
            std::vector<Traverse*> children;
            children.push_back(root->left->left->left);
            children.push_back(root->left->left->right);
            children.push_back(root->left->right->left);
            children.push_back(root->left->right->right);
            allChildren.push_back(children);
        }

        GateInfo ourTopology;
        ourTopology.allChildren = allChildren;
        ourTopology.topologies = topologies;

        if (root->data->name == "F") {
            std::cout << "Number of topologies: " << ourTopology.allChildren.size() << std::endl;
            std::cout << "type: " << root->data->type << std::endl;
            std::cout << "child type: " << root->left->data->type << std::endl;
        }

        return ourTopology;
    }

    //Finds the minimum cost traversing the tree and stores the minimum cost
    void costMin(Traverse* node) {
        if (node->left == nullptr && node->right == nullptr) {
            node->minCost = 0;
            return;
        }
        if (node->left != nullptr) {
            costMin(node->left);
        }
        if (node->right != nullptr) {
            costMin(node->right);
        }
        GateInfo ourTopology = getTopologies(node);
        for (int i = 0; i < ourTopology.allChildren.size(); i++) {
            int costOfChildren = 0;
            for (int j = 0; j < ourTopology.allChildren[i].size(); j++) {
                costOfChildren += ourTopology.allChildren[i][j]->minCost;
            }
            node->minCost = std::min(node->minCost, costs[ourTopology.topologies[i]] + costOfChildren);
        }
        std::cout << node->data->name << " " << node->minCost << std::endl;
    }
//Returns min cost
    int getFinalCost() {
        return root->minCost;
    }
};
#endif //TREEMAKE_H