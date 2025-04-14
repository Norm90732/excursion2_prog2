#include "TreeMake.h"
#include <iostream>
#include "FileInputOutput.h"
int main() {
    //Makes a treeMaker object, reads the file inputs as a default constructor
    TreeMake treeMaker;
    //Prints the NAND not tree
    treeMaker.printTree(treeMaker.root);
    //Prints the final cost of the tree
    std::cout << treeMaker.getFinalCost() << std::endl;
    //writes the cost to the output.txt.
    writeFile(treeMaker.getFinalCost());
    return 0;
}
