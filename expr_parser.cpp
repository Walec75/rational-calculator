#include "expr_parser.h"

#include <map>
#include <cmath>
#include <QDebug>
#include <QTextStream>
#include <stack>


void print_tokens(std::vector<Token> tokens)
{
    for (const auto& tok : tokens) {
        qInfo() << tok.value;
    }
}


rational calculate(const QString& input, int precision)
{
    expr_parser parser(input);
    std::vector<Token> tokens = parser.shunting_yard();
    return calculate_RPN(tokens, precision);
}


expr_parser::expr_parser(const QString& input)
{
    tokenize(input);
}

expr_eval_error::expr_eval_error(const QString& text, Token t) : message(text), tok(t) {}

QString expr_eval_error::mess() const { return message; }
Token expr_eval_error::get_token() const { return tok; }


using TT = TokenType;

bool is_pre_unary(TokenType type)
{
    return (type == LPAREN || type == PLUS ||
            type == MUL || type == DIV ||
            type == MINUS || type == POW);
}


std::vector<Token> expr_parser::shunting_yard()
{
    std::vector<Token> output_q;
    std::stack<Token> ops;

    const std::map<TokenType, int> PRIORITY = {
        {PLUS, 1}, {MINUS, 1},
        {MUL, 2}, {DIV, 2},
        {UNARY_MINUS, 3},
        {POW, 4}
    };
    // Token* error_token = nullptr;
    for (size_t i = 0; i < tokens.size(); ++i) {
        Token& token = tokens[i];
        switch (token.type) {
        case INT:
        case FLOAT:
            output_q.push_back(token);
            break;
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case POW:
            if (token.type == MINUS && (i == 0 || is_pre_unary(tokens[i-1]))) {
                ops.push({UNARY_MINUS, token.value, i});
                break;
            }

            while (!ops.empty() && ops.top().type != LPAREN && PRIORITY.at(ops.top().type) >= PRIORITY.at(token.type)) {
                output_q.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
            break;
        case LPAREN:
            ops.push(token);
            break;
        case RPAREN:
            while (!ops.empty() && ops.top().type != LPAREN) {
                output_q.push_back(ops.top());
                ops.pop();
            }
            if (ops.empty() || ops.top().type != LPAREN) {
                throw expr_eval_error("Mismatched parentheses", {ERROR, "", 0});
            }
            ops.pop();
            break;
        default:
            throw expr_eval_error("Wtf is this: " + token.value, {ERROR, "", 0});
            break;
        }
    }
    while (!ops.empty()) {
        if (ops.top().type == LPAREN || ops.top().type == RPAREN)
            throw expr_eval_error("Mismatched parentheses", {ERROR, "", 0});
        output_q.push_back(ops.top());
        ops.pop();
    }

    return output_q;
}

TokenType get_number(size_t& i, const QString& input, QString& num)
{
    bool dot_seen = false;
    while (i < input.size() && (input[i].isDigit() || input[i] == '.')) {
        if (input[i] == '.') {
            if (dot_seen)
                throw expr_eval_error("More than one decimal point", {ERROR, input[i], i});
            dot_seen = true;
        }

        num += input[i];
        i++;
    }
    i--;
    return num == "" ? ERROR : (dot_seen ? FLOAT : INT);
}


void expr_parser::tokenize(const QString& input)
{
    for (size_t i = 0; i < input.size(); ++i) {
        QChar token = input[i];
        TokenType t;
        if      (token == '+') tokens.push_back({PLUS, token, i});
        else if (token == '-') tokens.push_back({MINUS, token, i});
        else if (token == '*') tokens.push_back({MUL, token, i});
        else if (token == '/') tokens.push_back({DIV, token, i});
        else if (token == '(') tokens.push_back({LPAREN, token, i});
        else if (token == ')') tokens.push_back({RPAREN, token, i});
        else if (token == '^') tokens.push_back({POW, token, i});
        else if (token.isSpace()) continue;
        else if (QString num = 0; (t = get_number(i, input, num)) != ERROR) tokens.push_back({t, num, i});
        else throw expr_eval_error("Invalid symbol: " + QString(token), {ERROR, token, i + 1});
    }
}


rational calculate_RPN(const std::vector<Token>& tokens, int precision)
{
    std::stack<rational> rat_stack;

    for (const Token& token : tokens) {
        switch (token.type) {
        case INT:
            rat_stack.emplace(token.value.toInt());
            break;
        case FLOAT:
            rat_stack.emplace(token.value.toDouble(), precision, true);
            break;
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case POW:
        {
            if (rat_stack.size() < 2) {
                throw expr_eval_error("Not enough operands for: " + token.value, token);
            }
            rational rhs = rat_stack.top();
            rat_stack.pop();
            rational lhs = rat_stack.top();
            rat_stack.pop();
            rational result;

            if (token.type == PLUS) result = lhs + rhs;
            else if (token.type == MINUS) result = lhs - rhs;
            else if (token.type == MUL) result = lhs * rhs;
            else if (token.type == POW) result = lhs.pow(rhs); // rhs implicitly converts to int
            else if (token.type == DIV) {
                if (rhs == rational(0))
                    throw expr_eval_error("Division by zero", token);
                result = lhs / rhs;
            }
            rat_stack.push(result);
            break;
        }
        case UNARY_MINUS:
            if (rat_stack.empty())
                throw expr_eval_error("Not enough operands for: " + token.value, token);
            rat_stack.top() = -rat_stack.top();
            break;
        default:
            throw expr_eval_error("Invalid token type: " + token.value, token);
        }
    }

    if (rat_stack.size() != 1)
        throw expr_eval_error("Invalid expression", {ERROR, "", 0});
    return rat_stack.top();
}

void expr_parser::switch_input(const QString& input)
{
    tokens.clear();
    tokenize(input);
}

