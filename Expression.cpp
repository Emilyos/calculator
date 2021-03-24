//
// Created by Emil Khshiboun on 23/03/2021.
//

#include "Expression.h"

std::unordered_map<std::string, int> *Expression::var_map;

Expression::Expression(ExpTokens &tokens) : tokensList(tokens) {}

unsigned int Expression::getPriority() const {
    return priority;
}

void Expression::setVariablesMap(std::unordered_map<std::string, int> *varMap) {
    Expression::var_map = varMap;
}


int Expression::getVariable(const std::string &var_name) {
    auto itr = Expression::var_map->find(var_name);
    if (itr == Expression::var_map->end()) {
        throw ExpressionException("Variable " + var_name + " does not exist!");
    }
    return itr->second;
}

void Expression::setVariable(const std::string &var_name, int value) {
    (*Expression::var_map)[var_name] = value;
//    Expression::var_map->insert(std::make_pair(var_name, value));
}


IdExpression::IdExpression(ExpTokens &tokens) : Expression(tokens) {}

NumExpression::NumExpression(ExpTokens &tokens) : Expression(tokens) {}

BinOpExpression::BinOpExpression(ExpTokens &tokens) : Expression(tokens) {}

UnaryExpression::UnaryExpression(ExpTokens &tokens, UnaryType type) : Expression(tokens), unaryType(type) {}

ParenthesesExpression::ParenthesesExpression(ExpTokens &tokens) : Expression(tokens) {}


int IdExpression::evaluate() {
    Token *token = tokensList[0];
    auto var_name = *(std::string *) token->getData();
    return getVariable(var_name);
}

int NumExpression::evaluate() {
    Token *token = tokensList[0];
    return *(int *) token->getData();
}

int BinOpExpression::evaluate() {
    auto *lhs = (Expression *) tokensList[0]->getData();
    auto *rhs = (Expression *) tokensList[2]->getData();
    switch (tokensList[1]->getId()[0]) {
        case '+':
            return lhs->evaluate() + rhs->evaluate();
        case '*':
            return lhs->evaluate() * rhs->evaluate();
        case '-':
            return lhs->evaluate() - rhs->evaluate();
        case '/':
            return lhs->evaluate() / rhs->evaluate();
        default:
            throw ExpressionException();
    }
}


int UnaryExpression::evaluate() {
    Token *id;
    std::string var_name;
    int return_val, new_val;
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

int ParenthesesExpression::evaluate() {
    auto exp = (Expression *) tokensList[0]->getData();
    return exp->evaluate();
}
