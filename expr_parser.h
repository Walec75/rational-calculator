#ifndef EXPR_PARSER_H
#define EXPR_PARSER_H


#include <QString>
#include <QStack>
#include "rational.h"




enum TokenType {
    INT, FLOAT, PLUS, MINUS, UNARY_MINUS, MUL, DIV, POW, LPAREN, RPAREN, RATIONAL, CONSTANT, ERROR
    // FUNCTION
};




class Token {
public:
    TokenType type;
    QString value;
    size_t pos;
    Token(TokenType type_, QString value_, size_t pos_) : type(type_), value(value_), pos(pos_) {}

    operator TokenType() const { return type; }
};

class expr_eval_error : public std::exception
{
    Token tok;
    QString message;
public:
    expr_eval_error(const QString& text, Token t);
    QString mess() const;
    Token get_token() const;

};


rational calculate(const QString& input, int precision = 3);
rational calculate_RPN(const std::vector<Token>& tokens, int precision);

class expr_parser
{
private:
    std::vector<Token> tokens;
    int pos = 0;

public:
    expr_parser(const QString& input);
    std::vector<Token> shunting_yard();
    void switch_input(const QString& input);

private:
    void tokenize(const QString& input);
};



#endif // EXPR_PARSER_H
