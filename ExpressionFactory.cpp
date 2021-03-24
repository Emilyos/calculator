//
// Created by Emil Khshiboun on 24/03/2021.
//

#include "ExpressionFactory.h"
#include "Expression.h"
#include "Token.h"


Token *ExpressionFactory::makeUnaryExpressionToken(Token *first, Token *second) {
    if ((!IS_TOKEN(first, ID) && !IS_TOKEN(second, ID))) {
        throw ExpressionFactoryException("Invalid arguments!");
    }
    UnaryExpression::UnaryType type;
    if (DEC_OR_INC(first)) {
        type = IS_TOKEN(first, INC) ? UnaryExpression::UnaryType::INC_ID : UnaryExpression::UnaryType::DEC_ID;
    } else if (DEC_OR_INC(second)) {
        type = IS_TOKEN(second, INC) ? UnaryExpression::UnaryType::ID_INC : UnaryExpression::UnaryType::ID_DEC;
    } else {
        throw ExpressionFactoryException("Invalid arguments!");
    }
    ExpTokens expTokens{{first, second}};
    auto exp = new UnaryExpression(expTokens, type);
    return MakeExpToken(exp);
}

Token *ExpressionFactory::makeIdExpressionToken(Token *token) {
    if (!IS_TOKEN(token, ID)) {
        throw ExpressionFactoryException("Invalid arguments!");
    }
    ExpTokens expTokens = {token};
    auto exp = new IdExpression(expTokens);
    return MakeExpToken(exp);
}

Token *ExpressionFactory::makeNumExpressionToken(Token *token) {
    if (!IS_TOKEN(token, NUM)) {
        throw ExpressionFactoryException("Invalid arguments!");
    }
    ExpTokens expTokens = {token};
    auto exp = new NumExpression(expTokens);
    return MakeExpToken(exp);
}

Token *ExpressionFactory::makeBinOpExpressionToken(Token *lhs, Token *op, Token *rhs) {
    if (!IS_TOKEN(lhs, EXP) || !IS_TOKEN(op, BINOP) || !IS_TOKEN(rhs, EXP)) {
        throw ExpressionFactoryException("Invalid arguments!");
    }
    ExpTokens expTokens{lhs, op, rhs};
    auto exp = new BinOpExpression(expTokens);
    return MakeExpToken(exp);
}

Token *ExpressionFactory::makeParenthesesExpressionToken(Token *leftParen, Token *token, Token *rightParen) {
    if (!IS_TOKEN(token, EXP)) {
        throw ExpressionFactoryException("Invalid arguments!");
    }
    ExpTokens expTokens{leftParen, token, rightParen};
    auto exp = new ParenthesesExpression(expTokens);
    return MakeExpToken(exp);
}

Token *ExpressionFactory::MakeExpToken(void *data) {
    auto new_token = new Token(EXP);
    new_token->setData(data);
    return new_token;
}
