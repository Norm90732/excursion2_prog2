#ifndef FILEINPUTOUTPUT_H
#define FILEINPUTOUTPUT_H
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

//Struct to define logic values
struct LogicValues {
    std::string name;
    std::string type;
    std::vector<LogicValues*> inputs;
    bool isOutput;
    bool isOutputOutput;
};

//Helper function to point to children of each node for the netlist reading
LogicValues* getLogicValues(const std::vector<LogicValues*>& logicValues,const std::string& name) {
    LogicValues* logic = nullptr;
    for (unsigned int i = 0; i < logicValues.size(); i++) {
        if (logicValues[i]->name == name) {
            return logicValues[i];
        }
    }
    return logic;
}

std::fstream myFile;
//Reads file input
std::vector<LogicValues*> readFile() {
    LogicValues* logic = nullptr;
    std::vector<LogicValues*> logicValues;

    std::ifstream myFile("input.txt");
    if (myFile.is_open()) {
        std::string line;
        std::vector<std::string> lines;

        //using std to make words for each line. Makes netlist parsing easier
        //https://cplusplus.com/reference/sstream/istringstream/
        while (getline(myFile, line)) {
            //Sets up tokenization
            std::vector <std::string> inputTokenSplit;
            std::istringstream iss(line);
            std::string token;
            //splits each line into tokens
            while (iss >> token) {
                inputTokenSplit.push_back(token);
            }
            if (inputTokenSplit.size() < 2) {continue;}
            //If the second word is INPUT
            if(inputTokenSplit[1] == "INPUT") {
                logic = new LogicValues();
                logic->name = inputTokenSplit[0];
                logic->type = "INPUT";
                logic->isOutput = false;
                logicValues.push_back(logic);
                std::cout << "Input Node of " << logic->name << std::endl;
                logic->isOutputOutput = false;
            }
            //If the second word is OUTPUT
            if(inputTokenSplit[1] == "OUTPUT") {
                logic = new LogicValues();
                logic->name = inputTokenSplit[0];
                logic->type = "OUTPUT";
                logic->isOutput = true;
                logicValues.push_back(logic);
                logic->isOutputOutput = true;
                std::cout << "Output Node of " << logic->name << std::endl;
            }
            //If the second word is =, it is an equation
            if (inputTokenSplit[1] == "=") {

                if (inputTokenSplit[2] == "AND") {
                    logic = new LogicValues();
                    logicValues.push_back(logic);
                    logic->name = inputTokenSplit[0];
                    logic->type = "AND";
                    logic->isOutput = false;
                    logic->isOutputOutput = false;
                    logic->inputs.push_back(getLogicValues(logicValues, inputTokenSplit[3]));
                    logic->inputs.push_back(getLogicValues(logicValues, inputTokenSplit[4]));
                }

                else if (inputTokenSplit[2] == "OR") {
                    logic = new LogicValues();
                    logicValues.push_back(logic);
                    logic->name = inputTokenSplit[0];
                    logic->type = "OR";
                    logic->isOutput = false;
                    logic->isOutputOutput = false;
                    logic->inputs.push_back(getLogicValues(logicValues, inputTokenSplit[3]));
                    logic->inputs.push_back(getLogicValues(logicValues, inputTokenSplit[4]));
                }

                else if (inputTokenSplit[2] == "NOT") {
                    logic = new LogicValues();
                    logicValues.push_back(logic);
                    logic->name = inputTokenSplit[0];
                    logic->type = "NOT";
                    logic->isOutput = false;
                    logic->isOutputOutput = false;
                    logic->inputs.push_back(getLogicValues(logicValues, inputTokenSplit[3]));
                }

                //checks if word 0, letter 0 is t, it assumes output
                else if (inputTokenSplit[0][0] != 't') {
                    logic = new LogicValues();
                    logicValues.push_back(logic);
                    logic->name = inputTokenSplit[0];
                    logic->type = inputTokenSplit[2];
                    logic->isOutput = true;
                    logic->isOutputOutput = false;
                    logic->inputs.push_back(getLogicValues(logicValues, inputTokenSplit[3]));
                    logic->inputs.push_back(getLogicValues(logicValues, inputTokenSplit[4]));
                }
            }
        }
        }
    return logicValues;
        }


#endif //FILEINPUTOUTPUT_H
