//
// Created by Emil Khshiboun on 21/03/2021.
//

#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H

#include "LexicalAnalyzer.h"
#include <string>
#include <unordered_map>
#include "Expression.h"

typedef std::unordered_map<std::string, int> Variables;

class Calculator {
    std::string calc_name;
    LexicalAnalyzer lexicalAnalyzer;
    Variables vars;

    static void initTerminalTokens(TerminalTokens &);

public:
    explicit Calculator(const std::string &);

    void start();

};


#endif //CALCULATOR_CALCULATOR_H
