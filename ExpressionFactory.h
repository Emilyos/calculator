//
// Created by Emil Khshiboun on 24/03/2021.
//

#ifndef CALCULATOR_EXPRESSIONFACTORY_H
#define CALCULATOR_EXPRESSIONFACTORY_H
#define DEC_OR_INC(token) (IS_TOKEN(token,INC)|| IS_TOKEN(token,DEC))

#include "Token.h"

class ExpressionFactoryException : public std::exception {
    std::string message;
public:
    ExpressionFactoryException(const std::string &message = "") : message("ExpressionFactoryException" + message) {}

    const char *what() const _NOEXCEPT override {
        return message.c_str();
    }
};

class ExpressionFactory {
private:
    static inline Token *MakeExpToken(void *data);

public:
    static Token *makeUnaryExpressionToken(Token *first, Token *second);

    static Token *makeIdExpressionToken(Token *token);

    static Token *makeNumExpressionToken(Token *token);

    static Token *makeBinOpExpressionToken(Token *lhs, Token *op, Token *rhs);

    static Token *makeParenthesesExpressionToken(Token *token);
};


#endif //CALCULATOR_EXPRESSIONFACTORY_H
