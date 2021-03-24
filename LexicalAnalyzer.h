//
// Created by Emil Khshiboun on 21/03/2021.
//

#ifndef CALCULATOR_LEXICALANALYZER_H
#define CALCULATOR_LEXICALANALYZER_H

#include "Token.h"
#include <utility>
#include <vector>
#include <string>
#include <exception>
#include <map>


typedef std::vector<Token *> Assignment;
typedef std::vector<Token> TerminalTokens;
const std::string ALPHA = {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const std::string DIGITS = {"0123456789"};

class LexicalAnalyzerException : public std::exception {
    std::string message;
public:
    explicit LexicalAnalyzerException(std::string error_message = "") : message(std::move(error_message)) {}

    const char *what() const _NOEXCEPT override {
        return message.c_str();
    }
};

class LexicalAnalyzer {
    TerminalTokens terminalTokens;
    std::map<int, int> parentheses;

    void _getTokensFrom(const std::string &, int, std::vector<Token *> &, Token * = nullptr);

    void _buildParenthesesIndex(const Assignment &);

public :
    explicit LexicalAnalyzer(const TerminalTokens &terminalTokens = TerminalTokens());


    Assignment lexAnalyze(const std::string &line_input);

    bool syntaxCheck(Assignment &assignment);

};

#endif //CALCULATOR_LEXICALANALYZER_H
