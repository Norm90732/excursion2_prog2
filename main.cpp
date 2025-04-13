#include "TreeMake.h"
#include <iostream>
#include "FileInputOutput.h"
int main() {
    TreeMake treeMaker;
    treeMaker.printTree(treeMaker.root);
    std::cout << treeMaker.getFinalCost() << std::endl;
    writeFile(treeMaker.getFinalCost());
    return 0;
}
