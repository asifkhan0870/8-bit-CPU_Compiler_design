#ifndef  PARSER_H

#define PARSER_H

#include "lexer.h"
#include<ostream>
#include<map>
#include<vector>
#include<memory>
using namespace std;

class ASTNode {

    public:
      virtual void generateCode(ostream & out)=0;
      virtual void generateL(ostream & out)=0;
      virtual void generateR(ostream & out)=0;
};

// inherite another class publicaaly from ast node for number 

class NumberNode : public ASTNode{
    
    public: 
       int val;
       NumberNode(int val);
      virtual void generateCode(ostream & out)=0;
      virtual void generateL(ostream & out)=0;
      virtual void generateR(ostream & out)=0;
       

};

// inherite for variable

class VariableNode : public ASTNode{

    public:
        static map<string,int>mem,loc;
        static int counter;

        string name;

        int id;
        VariableNode(string name);
        VariableNode(){};

        virtual void generateCode(ostream & out)=0;
        virtual void generateL(ostream & out)=0;
        virtual void generateR(ostream & out)=0;

};


//some imp more class

class VariableDec: public VariableNode {

    public: 
        string var;
        shared_ptr<ASTNode>val;

        VariableDec(string var,shared_ptr<ASTNode>val);
         void generateCode(ostream & out);
         void generateL(ostream & out){};
         void generateR(ostream & out){};
};



class BinaryOpNode : public ASTNode {

    public:
       shared_ptr<ASTNode>l,r;

       char op;

       BinaryOpNode(shared_ptr<ASTNode>l, char op, shared_ptr<ASTNode>r);
        void generateCode(ostream & out){};
       void generateL(ostream & out){};
       void generateR(ostream & out);
};


class ConditionalNode : public ASTNode {

    public: 
        shared_ptr<ASTNode> condition, then_br,else_br;

        ConditionalNode(shared_ptr<ASTNode>condition,shared_ptr<ASTNode>then_br,shared_ptr<ASTNode>else_br);
        void generateCode(ostream & out);
        void generateL(ostream & out){};
        void generateR(ostream & out){};

};


class BlockNode : public ASTNode {

     public: 
        vector<shared_ptr<ASTNode>>st;
        BlockNode(){}
        void generateCode(ostream & out);
        void generateL(ostream & out){};
        void generateR(ostream & out){};
};


class Parser {

    public:
        Parser(vector<Token>tokens);
        shared_ptr<ASTNode>parse();

    public:
        vector<Token>tokens;
        size_t pos;


        shared_ptr<ASTNode>  parseExp();
        shared_ptr<ASTNode> parseExpStat();
        shared_ptr<ASTNode> parsePrim();
        shared_ptr<ASTNode> parseVarDec();
        shared_ptr<ASTNode> parseVarDec();
        shared_ptr<ASTNode> parseVarAssign();
        shared_ptr<ASTNode> parseCond();
        shared_ptr<ASTNode> parseBlock();



        bool match (vector<Tokentype> types);
        bool isAtEnd();

        bool check(Tokentype type);
        void advance ();
        Token previous();

        void consume (Tokentype type,string);


};

#endif
