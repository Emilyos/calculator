//
// Created by Emil Khshiboun on 21/03/2021.
//

#ifndef CALCULATOR_ANALYZER_H
#define CALCULATOR_ANALYZER_H

#include "Token.h"
#include <utility>
#include <vector>
#include <string>
#include <exception>
#include <set>


typedef std::vector<Token *> TokenStream;
typedef std::vector<Token *> Assignment;
typedef std::vector<Token> TerminalTokens;


class LexicalAnalyzerException : public std::exception {
    std::string message;
public:
    explicit LexicalAnalyzerException(std::string error_message = "") : message(std::move(error_message)) {}

    const char *what() const _NOEXCEPT override {
        return message.c_str();
    }
};

class Analyzer {
private:
    std::string ALPHA = {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    std::string DIGITS = {"0123456789"};
    TerminalTokens terminalTokens;

    std::set<int> left_parentheses;

    void _getTokensFrom(const std::string &, int, std::vector<Token *> &, Token * = nullptr);

    void _buildParenthesesIndex(const Assignment &);

    void _buildBinaryExpression(Assignment &assignment, int start, int &end, const char ops[2]);

    void _buildExpressionWithoutParen(Assignment &assignment, int start, int end);

public :
    explicit Analyzer(const TerminalTokens &terminalTokens = TerminalTokens());


    TokenStream lexAnalyze(const std::string &line_input);

    void syntaxCheck(Assignment &assignment);

};

#endif //CALCULATOR_ANALYZER_H
