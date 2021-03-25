//
// Created by Emil Khshiboun on 23/03/2021.
//

#include "Expression.h"

std::unordered_map<std::string, double> *Expression::var_map;

Expression::Expression(ExpTokens &tokens) : tokensList(tokens) {}

unsigned int Expression::getPriority() const {
    return priority;
}

void Expression::setVariablesMap(std::unordered_map<std::string, double> *varMap) {
    Expression::var_map = varMap;
}


double Expression::getVariable(const std::string &var_name) {
    auto itr = Expression::var_map->find(var_name);
    if (itr == Expression::var_map->end()) {
        throw ExpressionException("Variable " + var_name + " does not exist!");
    }
    return itr->second;
}

void Expression::setVariable(const std::string &var_name, double value) {
    (*Expression::var_map)[var_name] = value;
}

Expression::~Expression() {
    for (auto token:tokensList) {
        if (IS_TOKEN(token, ID) || IS_TOKEN(token, NUM) || IS_TOKEN(token, EXP)) {
            delete token;
        }
    }
    tokensList.clear();
}


IdExpression::IdExpression(ExpTokens &tokens) : Expression(tokens) {
    priority = Priorities::NUM_ID;
}

NumExpression::NumExpression(ExpTokens &tokens) : Expression(tokens) {
    priority = Priorities::NUM_ID;
}

BinOpExpression::BinOpExpression(ExpTokens &tokens) : Expression(tokens) {
    if (tokens[1]->getId().compare("+") == 0 || tokens[1]->getId().compare("-") == 0) {
        priority = Priorities::ADD_SUB;
    } else {
        priority = Priorities::MUL_DIV;
    }
}

UnaryExpression::UnaryExpression(ExpTokens &tokens, UnaryType type) : Expression(tokens), unaryType(type) {
    priority = Priorities::Unary;
}

ParenthesesExpression::ParenthesesExpression(ExpTokens &tokens) : Expression(tokens) {
    priority = Priorities::PARENTHESES;
}


double IdExpression::evaluate() {
    Token *token = tokensList[0];
    auto var_name = *(std::string *) token->getData();
    return getVariable(var_name);
}

double NumExpression::evaluate() {
    Token *token = tokensList[0];
    return *(double *) token->getData();
}

double BinOpExpression::evaluate() {
    auto *left_exp = (Expression *) tokensList[0]->getData();
    auto *right_exp = (Expression *) tokensList[2]->getData();
    double lhs, rhs;
    if (left_exp->getPriority() > right_exp->getPriority()) { // lower val -> higher priority
        // right side first:
        rhs = right_exp->evaluate();
        lhs = left_exp->evaluate();
    } else {
        // left side first
        lhs = left_exp->evaluate();
        rhs = right_exp->evaluate();
    }
    switch (tokensList[1]->getId()[0]) {
        case '+':
            return lhs + rhs;
        case '*':
            return lhs * rhs;
        case '-':
            return lhs - rhs;
        case '/':
            if (rhs == 0) {
                throw ExpressionException("Divide by zero!!!!!!!");
            }
            return lhs / rhs;
        default:
            throw ExpressionException("Unknown operator!");
    }
}


double UnaryExpression::evaluate() {
    Token *id;
    std::string var_name;
    double return_val, new_val;
    switch (unaryType) {
        case INC_ID:
            id = tokensList[1];
            var_name = *(std::string *) id->getData();
            return_val = getVariable(var_name) + 1;
            new_val = return_val;
            break;
        case DEC_ID:
            id = tokensList[1];
            var_name = *(std::string *) id->getData();
            return_val = getVariable(var_name) - 1;
            new_val = return_val;
            break;
        case ID_INC:
            id = tokensList[0];
            var_name = *(std::string *) id->getData();
            return_val = getVariable(var_name);
            new_val = return_val + 1;
            break;
        case ID_DEC:
            id = tokensList[0];
            var_name = *(std::string *) id->getData();
            return_val = getVariable(var_name);
            new_val = return_val - 1;
            break;
    }
    setVariable(var_name, new_val);
    return return_val;
}

double ParenthesesExpression::evaluate() {
    auto exp = (Expression *) tokensList[1]->getData();
    return exp->evaluate();
}
