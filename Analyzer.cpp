//
// Created by Emil Khshiboun on 21/03/2021.
//

#include "Analyzer.h"
#include "sstream"
#include "Expression.h"
#include <string>
#include <stack>

#define IS_BINOP(c) (c == '+' || c == '-' || c == '/' || c =='*')

Analyzer::Analyzer(const TerminalTokens &terminalTokens) {
    this->terminalTokens = terminalTokens;
}

/**
 *  Lexical Analyzer - This is the first step in the analyzer,
 *  The purpose of this function is to parse the given input into well-defined tokens
 *  Note that this function does minimal effort in terms of syntax checking
 * @param line_input Input line to be parsed as token
 * @return TokenStream AKA list of Tokens
 */
TokenStream Analyzer::lexAnalyze(const std::string &line_input) {
    TokenStream assignment = TokenStream();
    std::stringstream sstream(line_input);
    std::string next;
    while (sstream >> next) {
        std::vector<Token *> tokens{};
        _getTokensFrom(next, 0, tokens);
        for (auto token: tokens) {
            if (token->getType() == TokenType::INC || token->getType() == TokenType::DEC) {
                throw LexicalAnalyzerException("Syntax Error!");
            }
            assignment.push_back(token);
            // std::cout << *token << std::endl;
        }
    }
    return assignment;
}


/**
 * Helper function to the lexical analyzer, used to parse a signle trimed word
 * This function is recursive.
 * @param word The word to parse into token
 * @param start the start index in the word
 * @param buffer the buffer to write the tokens to.
 * @param last_token this is helper parameter to keep track of the last inserted token by this specific function call.
 */
void Analyzer::_getTokensFrom(const std::string &word, int start, std::vector<Token *> &buffer,
                              Token *last_token) {
    const auto len = word.length();
    if (start >= len) return;
    if (IS_TOKEN(last_token, EXP) && std::isalnum(word[start])) {
        throw LexicalAnalyzerException("Syntax Error! " + word);
    }
    auto parser = [len, start, &word](const std::string &chars, TokenType type, size_t &next_pos) {
        auto pos = word.find_first_not_of(chars, start);
        if (pos == std::string::npos) {
            pos = len;
        }
        unsigned int id_size = ((len - start) == 1) || start == pos ? 1 : (pos - start);
        auto substr = word.substr(start, id_size);
        void *data = type == ID ? (void *) new std::string(substr) : new int(std::stoi(substr));
        auto *new_token = new Token(type);
        new_token->setData(data);
        next_pos = pos;
        return new_token;
    };
    if (std::isalpha(word[start])) {
        size_t next_pos;
        auto new_token = parser(ALPHA, ID, next_pos);
        if ((IS_TOKEN(last_token, INC) || IS_TOKEN(last_token, DEC))) {
            if (next_pos < len) {
                const char next_char = word[next_pos];
                if (!IS_BINOP(next_char) && next_char != ')')
                    throw LexicalAnalyzerException("Syntax Error! " + word);
            }
            auto insert_token = ExpressionFactory::makeUnaryExpressionToken(last_token, new_token);
            new_token = insert_token;
            buffer.pop_back();
        }
        buffer.push_back(new_token);
        return _getTokensFrom(word, next_pos, buffer, new_token);
    } else if ((IS_TOKEN(last_token, INC) || IS_TOKEN(last_token, DEC))) {
        throw LexicalAnalyzerException("Syntax Error! " + word);
    }
    if (std::isdigit(word[start])) {
        size_t pos;
        auto new_token = parser(DIGITS, TokenType::NUM, pos);
        buffer.push_back(new_token);
        return _getTokensFrom(word, pos, buffer, new_token);
    }
    for (auto &token: terminalTokens) {
        if (word.compare(start, token.getId().length(), token.getId()) == 0) {
            auto next_pos = start + token.getId().length();
            Token *insert_token = &token;
            if ((IS_TOKEN(&token, INC) || IS_TOKEN(&token, DEC)) && IS_TOKEN(last_token, ID)) { // case ID++/ID--
                auto new_token = ExpressionFactory::makeUnaryExpressionToken(last_token, &token);
                buffer.pop_back();
                insert_token = new_token;
                if (next_pos < len && word[next_pos] != ')') {
                    throw LexicalAnalyzerException("Syntax Error! " + word);
                }
            }
            buffer.push_back(insert_token);
            return _getTokensFrom(word, next_pos, buffer, insert_token);
        }
    }
    throw LexicalAnalyzerException("Syntax Error! " + word.substr(start));
}

/**
 * Used as part of the symantic check
 * This function reduces the rule Exp -> Exp BINOP Exp
 * @param assignment
 * @param start the start index in the assignement
 * @param end the past-the-end index of the assignment
 * @param mul_div used to determine
 */
void Analyzer::_buildBinaryExpression(Assignment &assignment, int start, int &end, const char ops[2]) {
    bool found_exp = false;
    int i = start;
    while (i < end) {
        Token *token = assignment[i];
        if (IS_TOKEN(token, EXP)) {
            if (found_exp) throw LexicalAnalyzerException("Syntax Error!");
            found_exp = true;
            i++;
        } else if (!IS_TOKEN(token, BINOP) || !found_exp) {
            throw LexicalAnalyzerException("Syntax Error!");
        } else { // found BINARY OPERATOR
            assert(token != NULL && !token->getId().empty());
            found_exp = false;
            const char op = token->getId()[0];
            if (op == ops[0] || op == ops[1]) {
                assert(i - 1 >= start && IS_TOKEN(assignment[i - 1], EXP));
                if ((i + 1) >= end || !IS_TOKEN(assignment[i + 1], EXP)) {
                    throw LexicalAnalyzerException("Syntax Error!");
                }
                auto new_token = ExpressionFactory::makeBinOpExpressionToken(assignment[i - 1], token,
                                                                             assignment[i + 1]);
                assignment.insert(assignment.begin() + i - 1, new_token);
                assignment.erase(assignment.begin() + i, assignment.begin() + i + 3);
                end -= 2;
                found_exp = true;
            } else {
                i++;
            }
        }
    }
}


void Analyzer::_buildExpressionWithoutParen(Assignment &assignment, int start, int end) {
    // Convert ID/NUM tokens into Exp
    for (int i = start; i < end; i++) {
        Token *token = assignment[i];
        if (IS_TOKEN(token, ID) || IS_TOKEN(token, NUM)) {
            auto new_token = (IS_TOKEN(token, ID)) ? ExpressionFactory::makeIdExpressionToken(token)
                                                   : ExpressionFactory::makeNumExpressionToken(token);
            assignment.insert(assignment.begin() + i, new_token);
            assignment.erase(assignment.begin() + i + 1);
        }
    }
    // now we should be able to reduce Exp -> Exp BINOP Exp only.
    int tmp_end = end;
    // start with higher precedences (mul & div)
    char mul_div_ops[2] = {'*', '/'};
    char add_sub_ops[2] = {'+', '-'};
    _buildBinaryExpression(assignment, start, tmp_end, mul_div_ops);
    // lower precedences (sub & add)
    _buildBinaryExpression(assignment, start, tmp_end, add_sub_ops);
}

void Analyzer::syntaxCheck(Assignment &assignment) {
    if (assignment.size() <= 2 || assignment[0]->getType() != ID || assignment[1]->getType() != ASSIGN) {
        throw LexicalAnalyzerException("Invalid assignment!");
    }
    _buildParenthesesIndex(assignment);
    for (auto ritr = left_parentheses.rbegin(); ritr != left_parentheses.rend(); ritr++) {
        int left = *ritr;
        int right;
        for (right = left + 1; right < assignment.size(); right++) {
            if (IS_TOKEN(assignment[right], RPAREN)) break;
        }
        if (right >= assignment.size()) throw LexicalAnalyzerException("Syntax Error!");
        _buildExpressionWithoutParen(assignment, left + 1, right);
        auto new_token = ExpressionFactory::makeParenthesesExpressionToken(assignment[left + 1]);
        assignment.insert(assignment.begin() + left, new_token);
        assignment.erase(assignment.begin() + left + 1, assignment.begin() + left + 4);

    }
    _buildExpressionWithoutParen(assignment, 2, assignment.size());
}

/**
 * Part of the syntax check, checks if the parentheses are balanced.
 * @param assignment
 */
void Analyzer::_buildParenthesesIndex(const Assignment &assignment) {
    left_parentheses.clear();
    int size = assignment.size();
    std::stack<int> Q{};
    for (int i = 0; i < size; i++) {
        if (assignment[i]->getType() == TokenType::LPAREN) {
            Q.push(i);
        } else if (assignment[i]->getType() == TokenType::RPAREN) {
            if (Q.empty()) {
                throw LexicalAnalyzerException("unbalanced left_parentheses!");
            }
            int left_paren = Q.top();
            Q.pop();
            if (i - left_paren <= 1) throw LexicalAnalyzerException("Invalid left_parentheses!");
            left_parentheses.insert(left_paren);
        }
    }
}
