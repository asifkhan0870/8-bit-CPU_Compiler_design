
#include  "lexer.h"
#include<iostream>
using namespace std;


Lexer::Lexer (string &src ) : src (src), pos(0){}

vector<Token>Lexer:: tokenize(){


    vector<Token>tokens;

    while(pos<src.length()){


        char curr=src[pos];

        if(isspace(curr)){

            pos++;
            continue;
        }

        if(isalpha(curr)){

            string ident;

            while(isalnum(curr)){
                ident+=curr;
                pos++;

                if(pos>=src.length())break;

                curr=src[pos];
            }

            if(ident=="int") tokens.push_back({Tokentype::INT,"int"});
            else if(ident=="if")tokens.push_back({Tokentype::IF,"if"});
            else if(ident=="else")tokens.push_back({Tokentype::ELSE,"else"});

            else{
                tokens.push_back({Tokentype::IDENTIFIER,ident});
            }
        }
        else if(isdigit(curr){
               
               string number;

               while(isdigit(curr)){

                    number+=curr;
                    pos++;

                    if(pos>=src.length())break;

                    curr=src[pos];
               }

               tokens.push_back({Tokentype::NUMBER,number});
        }
        else{

            switch(curr){

                case '=':
                pos++;
                if(pos<src.length() && src[pos]=='='){

                    tokens.push_back({Tokentype::EQ,'#'}); // not work doe my binary node as a switch need char const 
                }
                else{

                    tokens.push_back({Tokentype::EQUAL,"="});

                    cout<<" In equal too "<<endl;
                    pos--;
                }
                break;

                case '+' : tokens.push_back({Tokentype::PLUS,"+"});break;
                case '-' : tokens.push_back({Tokentype::MINUS,"-"});break;
                case '(' : tokens.push_back({Tokentype::LPAREN,"("});break;
                case ')' : tokens.push_back({Tokentype::RPAREN,")"});break;
                case '{' : tokens.push_back({Tokentype::LBRACE,"{"});break;
                case '}' : tokens.push_back({Tokentype::PLUS,"}"});break;
                case ';' : tokens.push_back({Tokentype::SEMICOLON,";"});break;

                default:  cout<<"unknown character"<<curr<<endl;


            }
            pos++;
        }
    }


    tokens.push_back({Tokentype::END,""});

    return tokens;
}