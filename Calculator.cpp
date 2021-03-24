//
// Created by Emil Khshiboun on 21/03/2021.
//

#include "Calculator.h"
#include "iostream"

#include <map>
#include "Expression.h"

using namespace std;


Calculator::Calculator(const string &calc_name) : vars() {
    this->calc_name = string(calc_name);
    TerminalTokens terminalTokens = TerminalTokens();
    initTerminalTokens(terminalTokens);
    this->lexicalAnalyzer = LexicalAnalyzer(terminalTokens);
    Expression::setVariablesMap(&vars);
}

void Calculator::start() {
    bool no_error = true;
    do {
        string next_line;
        std::cout << calc_name << "$ ";
        std::getline(std::cin, next_line);
        try {
            Assignment next_assignment = lexicalAnalyzer.lexAnalyze(next_line);
            lexicalAnalyzer.syntaxCheck(next_assignment);
            auto var_name = *(std::string *) (next_assignment[0]->getData());
            auto exp = (Expression *) next_assignment[2]->getData();
            auto val = exp->evaluate();
            vars[var_name] = val;
            std::cout << var_name << " = " << val << std::endl;
        } catch (exception &e) {
            no_error = false;
            std::cout << e.what() << std::endl;
        }
    } while (no_error);
}

void Calculator::initTerminalTokens(TerminalTokens &terminalTokens) {
    terminalTokens = {
            {INC,    "++"},
            {DEC,    "--"},
            {ASSIGN, "+="},
            {ASSIGN, "-="},
            {ASSIGN, "*="},
            {ASSIGN, "/="},
            {ASSIGN, "="},
            {BINOP,  "+"},
            {BINOP,  "-"},
            {BINOP,  "*"},
            {BINOP,  "/"},
            {LPAREN, "("},
            {RPAREN, ")"}
    };
}

