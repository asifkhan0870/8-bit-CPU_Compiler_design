#include<iostream>
#include<fstream>
#include "lexer.h"

#include "parser.h"

using namespace std;

void printTokens (vector<Token> &tokens){


    for(Token t : tokens){
          
          cout<<"Token value "<<t.value<<" token number "<<static_cast<int>(t.type)<<endl;

    }
}



int main(){

    ifstream inputFile("input.txt");

    ofstream outFile("output.asm");


    if(!outFile.is_open()){

        cerr<<"output file not open"<<endl;
    }


    string sourceCode(istreambuf_iterator<char>(inputFile), istream_iterator<char>());


    inputFile.close();

    Lexer lexer(sourceCode);


    vector<Token>tokens=lexer.tokenize();

    printTokens(tokens);

    Parser parser(tokens);

    cout<<" token parsed"<<endl;


    shared_ptr<ASTNode> ast=parser.parse();


    if(ast){

        cout<<"going to parsing "<<endl;
        outFile<<" .text"<<endl;

        ast->generateCode(outFile);
    }
    else{

        cerr<<" parsing failed"<<endl;
    }

    outFile<<"hlt"<<endl;
    outFile.close();

    return 0;

}