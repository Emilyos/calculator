//
// Created by Emil Khshiboun on 21/03/2021.
//

#ifndef CALCULATOR_TOKEN_H
#define CALCULATOR_TOKEN_H

#include <string>
#include <exception>
#include <ostream>

#define IS_TOKEN(token, type) ((token) != NULL && (token)->getType() == TokenType::type)


typedef enum {
    NUM, ID, ASSIGN, INC, DEC, BINOP, LPAREN, RPAREN, EXP, UNKNOWN
} TokenType;

class TokenInvalidOperationException : std::exception {
};

class Token {
    std::string id;
    void *data;
    TokenType type;
public:

    Token(TokenType type, const std::string &id = "");

    ~Token();

    TokenType getType();

    void setData(void *_data);

    void *getData();

    std::string getId();

    friend std::ostream &operator<<(std::ostream &os, const Token &token);
};


#endif //CALCULATOR_TOKEN_H
