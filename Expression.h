//
// Created by Emil Khshiboun on 23/03/2021.
//

#ifndef CALCULATOR_EXPRESSION_H
#define CALCULATOR_EXPRESSION_H

#include <utility>
#include <vector>
#include "Token.h"
#include <exception>
#include <ostream>
#include <iostream>
#include "ExpressionFactory.h"
#include <unordered_map>

typedef std::vector<Token *> ExpTokens;

typedef enum {
    PARENTHESES = 0, Unary, MUL_DIV, ADD_SUB, NUM_ID
} Priorities;

class ExpressionException : public std::exception {
    std::string message;
public:
    explicit ExpressionException(const std::string &message = "") : message("ExpressionException: " + message) {}

    const char *what() const noexcept override {
        return message.c_str();
    }
};

class Expression {
    static std::unordered_map<std::string, double> *var_map;
protected:
    ExpTokens tokensList;
    unsigned int priority;

    double getVariable(const std::string &var_name);

    void setVariable(const std::string &var_name, double value);

public:

    unsigned int getPriority() const;

    explicit Expression(ExpTokens &tokens);

    virtual ~Expression();

    virtual double evaluate() {
        throw ExpressionException();
    }

    static void setVariablesMap(std::unordered_map<std::string, double> *varMap);


};

class IdExpression : public Expression {
private:
    friend ExpressionFactory;

    explicit IdExpression(ExpTokens &tokens);

public:

    double evaluate() override;
};

class NumExpression : public Expression {
private:
    friend ExpressionFactory;

    explicit NumExpression(ExpTokens &tokens);

public:

    double evaluate() override;
};

class BinOpExpression : public Expression {
private:
    friend ExpressionFactory;

    explicit BinOpExpression(ExpTokens &tokens);

public:

    double evaluate() override;
};

class UnaryExpression : public Expression {
private:
    friend ExpressionFactory;


    typedef enum {
        INC_ID, ID_INC, DEC_ID, ID_DEC, NEG
    } UnaryType;
    UnaryType unaryType;

    explicit UnaryExpression(ExpTokens &tokens, UnaryType type);

public:
    double evaluate() override;
};

class ParenthesesExpression : public Expression {
private:
    friend ExpressionFactory;

public:
    explicit ParenthesesExpression(ExpTokens &tokens);

    double evaluate() override;
};

#endif //CALCULATOR_EXPRESSION_H
