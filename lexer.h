#ifndef LEXER_H
#define LEXER_H

#include<string>
#include<vector>

using namespace std;

enum class Tokentype{

    INT,
    INDENTIFIER,
    ASSIGN,
    NUBMER,
    PLUS,
    MINUS,
    LPAREN,
    RPAREN,
    IF,
    ELSE,
    EQ,
    LBRACE,
    RBRACE,
    SEMICOLON,
    END,
    EQUAL,
};


struct Token{
    Tokentype type;
    string value;
};


class Lexer {


    public: 
       Lexer (string & src);
       vector<Token>tokenize();

    private:
       string &src;
       size_t pos; 
};

#endif