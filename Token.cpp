//
// Created by Emil Khshiboun on 21/03/2021.
//

#include "Token.h"
#include "Expression.h"

Token::Token(TokenType type, const std::string &id) : data(NULL) {
    this->type = type;
    this->id = std::string(id);
}

void Token::setData(void *_data) {
    if (type != TokenType::NUM && type != TokenType::BINOP && type != TokenType::ID && type != TokenType::EXP) {
        throw TokenInvalidOperationException();
    }
    this->data = _data;
}

void *Token::getData() {
    if (type != TokenType::NUM && type != TokenType::BINOP && type != TokenType::ID && type != TokenType::EXP) {
        throw TokenInvalidOperationException();
    }
    return data;
}

Token::~Token() {
    if (data != NULL) {
        switch (type) {
            case ID:
                delete (std::string *) data;
                break;
            case EXP:
                delete (Expression *) data;
                break;
            case NUM:
                delete (double *) data;
                break;
            default:
                break;
        }
    }
}

std::string Token::getId() {
    return id;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    if (token.type == ID && token.data != NULL) {
        os << "<ID," << *(std::string *) token.data << ">";
    } else if (token.type == NUM && token.data != NULL) {
        os << "<NUM," << *(double *) token.data << ">";
    } else if (token.type == EXP && token.data != NULL) {
        os << "<EXP>";
    } else {
        os << "<" << token.id << ">";
    }
    return os;
}

TokenType Token::getType() {
    return type;
}
