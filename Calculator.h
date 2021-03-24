//
// Created by Emil Khshiboun on 21/03/2021.
//

#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H

#include "Analyzer.h"
#include <string>
#include <unordered_map>
#include "Expression.h"

typedef std::unordered_map<std::string, int> Variables;

class Calculator {
    std::string calc_name;
    Analyzer lexicalAnalyzer;
    Variables vars;

    static void initTerminalTokens(TerminalTokens &);

public:
    explicit Calculator(const std::string &);

    void start();

    void printVariables();

    void processAssignment(Assignment &assignment);
};


#endif //CALCULATOR_CALCULATOR_H
