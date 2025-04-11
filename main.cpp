#include "TreeMake.h"
#include <iostream>
#include "FileInputOutput.h"
int main() {
    std::vector<LogicValues*> newLogicValues = readFile();
    std::cout << "Total nodes: " << newLogicValues.size() << std::endl;

    for (int i = 0; i < newLogicValues.size(); i++) {
        std::cout << "Node " << i << ": " << newLogicValues[i]->name
                  << " (Type: " << newLogicValues[i]->type
                  << ", IsOutput: " << (newLogicValues[i]->isOutput ? "Yes" : "No") << ")" << std::endl;
        std::cout << "  Inputs: ";
        if (newLogicValues[i]->inputs.empty()) {
            std::cout << "None";
        } else {
            for (auto* input : newLogicValues[i]->inputs) {
                std::cout << input->name << " ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}