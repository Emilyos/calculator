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
    this->lexicalAnalyzer = Analyzer(terminalTokens);
    Expression::setVariablesMap(&vars);
}

void Calculator::start() {
    bool no_error = true;

    do {
        string next_line;
        std::cout << calc_name << "$ ";
        std::getline(std::cin, next_line);
        if (cin.eof()) {
            printVariables();
            return;
        }
        try {
            Assignment next_assignment = lexicalAnalyzer.lexAnalyze(next_line);
            if (next_assignment.empty()) continue;
            lexicalAnalyzer.syntaxCheck(next_assignment);
            processAssignment(next_assignment);
            delete next_assignment[0];
            delete next_assignment[2];
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

void Calculator::printVariables() {
    std::cout << std::endl;
    for (const auto &var : vars) {
        std::cout << var.first << "=" << var.second << " ";
    }
    std::cout << std::endl;
}

void Calculator::processAssignment(Assignment &assignment) {
    //assert(assignment.size() == 3 && (IS_TOKEN(assignment[0], ID)
    //                                && IS_TOKEN(assignment[1], ASSIGN) &&
    //                              IS_TOKEN(assignment[2], EXP)));
    auto var_name = *(std::string *) assignment[0]->getData();
    auto exp = (Expression *) assignment[2]->getData();
    auto assign_type = assignment[1]->getId();

    if (assign_type == "=") {
        vars[var_name] = exp->evaluate();
    }
    // any other assignment, the variable should be already in the variables map
    if (vars.find(var_name) == vars.end()) throw LexicalAnalyzerException("Unknown variable " + var_name);
    if (assign_type == "+=") {
        vars[var_name] += exp->evaluate();
    } else if (assign_type == "-=") {
        vars[var_name] -= exp->evaluate();
    } else if (assign_type == "*=") {
        vars[var_name] *= exp->evaluate();
    } else if (assign_type == "/=") {
        vars[var_name] /= exp->evaluate();
    }
}

