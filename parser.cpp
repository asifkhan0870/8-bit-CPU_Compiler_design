#include "parser.h"
#include "lexer.h"

#include<iostream>
#include<algorithm>

using namespace std;


map <string,int> VariableNode :: mem_loc;

int VariableNode :: counter =1;

NumberNode :: NumberNode (int val): val(val){};

void NumberNode :: generateCode(ostream &out){

    out<<" ldi A : "<<val<<endl;
    cout<<" ldi A "<<val<<endl;

    cout<<endl<<" Generating in b number node "<<endl;
}


void NumberNode :: generateL (ostream &out){

    out<< " ldi A : "<<val<<endl;
    cout << " ldi A : "<<val<<endl;
    cout<<endl<< "Generating in l number node "<<endl;
}

void NumberNode :: generateR(ostream &out){

    out<< "ldi B: "<<val<<endl;
    cout<< " ldi B: "<<val<<endl;
    cout<<endl<<" Generating in r number node"<<endl;
}



 VariableNode :: VariableNode (string name): name (name){
    

    if(mem_loc.find(name)==mem_loc.end()){

          mem_loc[name]=counter;
          counter++;
    }

    id=mem_loc[name];


}


void VariableNode :: generateCode (ostream &out){

    cout<<endl<<" Generating in b VariableNode "<<endl;
    cout<<endl<<" Giving memory space "<<endl;
    cout<<"mov M A : "<<mem_loc[name]<<endl;
    out<<"mov M A : "<<mem_loc[name]<<endl;

}

void VariableNode :: generateR(ostream &out){

     cout<<" in variable node R "<<endl;
     out<<" mov B M "<<mem_loc[name]<<endl;
     cout<<" mov B M "<<mem_loc[name]<<endl;

}


BinaryOpNode :: BinaryOpNode (shared_ptr<ASTNode>l, char op, shared_ptr<ASTNode>r) : l(l) , op(op), r(r){}


void BinaryOpNode :: generateCode(ostream & out){

    cout<<" in binary node "<<endl;
    l->generateL(out);
    r->generateR(out);

    switch(op){

        case '+': 
          out<<" add "<<endl;
          cout<<" add "<<endl;
          break;
        case '-':
          out<< " sub "<<endl;
          cout<<" sub "<<endl;
          break;
        case '#': 
          out<<"cmp"<<endl;
          cout<<"cmp"<<endl;
          break;


        default:
           throw runtime_error("Unsupported binary operations/operands....");      
    }

    
}



VariableDec:: VariableDec (string var , shared_ptr<ASTNode> val): var(var),val(val){


     if(mem_loc.find(var)==mem_loc.end()){

        VariableNode:: mem_loc[var]=VariableNode:: counter++;
     }
}

void VariableDec :: generateCode(ostream &out){

    if (mem_loc.find(var)==mem_loc.end()){

         mem_loc[var]=counter++;
    }

    cout<<" varialbe declaration in b "<<endl;
    val->generateCode(out);

    cout<<" storing value to memory location "<<endl;

    cout<<" mov M A "<<mem_loc[var]<<endl;
    cout<< " mov M A "<<mem_loc[var]<<endl;
}


ConditionalNode:: ConditionalNode (shared_ptr<ASTNode>condition,shared_ptr<ASTNode>then_br,shared_ptr<ASTNode>else_br):condition(condition),then_br(then_br),else_br(else_br){}



void ConditionalNode :: generateCode(ostream &out){


    condition->generateCode(out);
    out<<" jnz  %ELSEBR"<<endl;
    cout<<" jnz %ELSEBR"<<endl;


    then_br->generateCode(out);
    out<<" jmp %OUTOFIF"<<endl;
    cout<<" jmp %OUTOFIF"<<endl;



    out<<"ELSEBR: "<<endl;
    cout<<"ELSEBR :  "<<endl;


    if(else_br){

        else_br->generateCode(out);
    }

    out<<"OUTOFIT: "<<endl;
    cout<<"OUTOFIT: "<<endl;

}




void BlockNode:: generateCode(ostream &out){

    cout<<"started generating code "<<endl;
    
    for(auto each: st){

        each->generateCode(out);
    }

    cout<<"ended generating code "<<endl;
}


Parser:: Parser (vector<Token> &tokens) : tokens (tokens),pos(0){


   cout<< "initializing parser "<<endl;
}


shared_ptr<ASTNode>Parser:: parse(){

    shared_ptr<BlockNode>bNode=make_shared<BlockNode>();


    while(!isAtEnd()){
           bNode->addStat(parseStat());

    }

    return bNode;
}



shared_ptr<ASTNode>Parser::parseStat(){

    cout<<" in parsestate"<<endl;

    if(match({Tokentype::INT})){

        cout<<" Found int "<<endl;
        return parseVecDec();
    }
    else if(match({Tokentype:: IF})){

        cout<<" found if "<<endl;
        return parseCond();
    }
    else if(match({Tokentype::  INDENTIFIER})){
        
        cout<<" Found variable "<<endl;
        return parseVarAssign();
    }
    else{

        cout<<" found exp state"<<endl;
        return parseExpState();
    }
}



shared_ptr<ASTNode>Parser::parseVarAssign (){

      
      cout<<" in var assign"<<endl;

      string var=previous().value;

      // sematic

      if(VariableNode::mem_loc.find(var)==VariableNode::mem_loc.end()){

        throw runtime_error("Semantic Error: Varialbe " + var + " not declared..");



      }


      consume(Tokentype:: EQUAL, "expect= ");

      shared_ptr<ASTNode>val=parseExp();

    consume(Tokentype::SEMICOLON, " expect ; ");
    return make_shared<VariableDec>(var,val);
}


shared_ptr<ASTNode>Parser::parseVarDec(){


    cout<<" in vardec"<<endl;
    consume(Tokentype::INDENTIFIER, "expect var");
    
    cout<<" got variable - "<<previous().value<<endl;


    // semantic check

    if(VariableNode:: mem_loc.find(previous().value)!=VariableNode::mem_loc.end()){

        throw runtime_error("Semantic error: Variable "+ previous().value+ "already declared..");

    }


    string var=previous().value;
    shared_ptr<ASTNode>val=nullptr;



    if(match({Tokentype::EQUAL})){

        cout<<" matched = "<<endl;
        val=parseExp();
    }
    else{

        cout<<" did not found : "<<tokens[pos].value<<endl;

        consume(Tokentype::SEMICOLON, " expect ; ");

        return make_shared<VariableNode>(var);
    }


    consume(Tokentype:: SEMICOLON, " expect ; ");

    return  make_shared<VariableDec>(var,val);

}


shared_ptr<ASTNode> Parser :: parserCond(){

    cout<< " in parser condition "<<endl;
    consume(Tokentype::LPAREN, "expect (");
    shared_ptr<ASTNode>condition1=parseExp();
    consume(Tokentype::RPAREN, "expect )");

    shared_ptr<ASTNode>then_br1=parseBlock();
    shared_ptr<ASTNode>else_br1=nullptr
    
    if(match({Tokentype::ELSE})){

        else_br1=parseBlock();
    }

    return make_shared<ConditionalNode>(condition1,then_br1,else_br1);


}


shared_ptr<ASTNode>Parser:: parseExpstat(){

    cout<<" in parseExpstat "<<endl;
    shared_ptr<ASTNode>exp=parseExp();

    consume(Tokentype::SEMICOLON, " expect ; ");
    return exp;
}


shared_ptr<ASTNode> Parser:: parseExp(){

    cout<< " in parseExp"<<endl;
    cout<<" parse expression for left "<<endl;
    shared_ptr<ASTNode>l=parsePrim();



    while(match({Tokentype:: PLUS, Tokentype:: MINUS, Tokentypea::EQ})){

        char op=previous().value[0];

        cout<<" parse expression for right "<<endl;

        cout<< tokens[pos].value<<endl;
        shared_ptr<ASTNode>r=parsePrim();

        l=make_shared<BinaryOpNode>(l,op,r);
    }

    return l;
}

shared_ptr<ASTNode> Parser:: parsePrim(){

    cout<<" in parseprim"<<endl;

    if(match({Tokentype::NUMBER})){

          cout<< " got number "<<endl;
          cout<<previous().value<<" returning "<<endl;

          return make_shared<NumberNode>(stoi(previous().value));

    }
    else if(match({Tokentype::INDENTIFIER })){

        return make_shared<VariableNode>(previous().value);
        cout<<" got var"<<endl;
    }

    else if(match({Tokentype::LPAREN})){

        cout<<" got ("<<endl;
        shared_ptr<ASTNode>exp=parseExp();

        consume(Tokentype::RPAREN,"expect )");

        cout<<" got )"<<endl;

        return exp;
    }

    cout<<tokens[pos].value<<endl;
    throw runtime_error("unexpected token");

}


bool Parser:: match(vector<Tokentype>types){


    auto it=find_if(types.begin(), types.end(),[&](Tokentype type){

        return type==tokens[pos].type;
    });

    if(it!=types.end()){
        pos++;
        return true;


    }

    cout<< "had "<<tokens[pos].value<<endl;
    return false;
}


bool Parser:: isAtEnd(){

    return pos>=tokens.size() || tokens[pos].type==Tokentype::END;
}

bool Parser:: check(Tokentype type){

    if(isAtEnd())[
        return false;
    ]

    return tokens[pos].type==type;
}

Token Parser:: previous(){

    return tokens[pos-1];
}

void Parser:: advance(){


    if(!isAtEnd()){

        pos++;
        return;


    }

    cout<<" ran out of tokens"<<endl;
    cout<<pos<<endl;
}


void Parser:: consume(Tokentype type, string errorMessage){

    if(check(type)){

        advance();

    }
    else{

        cout<<errorMessage<<' \n'<<" found : "<<tokens[pos].value<<endl;
        throw runtime_error("Wrong token present ");
    }
}



