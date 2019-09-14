
/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "parser.h"
#include <map>
#include <vector>
#include <stdlib.h>

std::map<std::string,int> symbol_Table;
std::map<std::string,int> Proc_Table;
std::vector<int> mem(1000);
int next_available=0;
stmt *program=nullptr;
bool calledPrimayOnce=false;
std::vector<int> inputsToProgram(1);
int next_input=1;
int number;

using namespace std;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}


bool Parser::isNum(Token *cse){
    
    if(cse->lexeme=="0"||cse->lexeme=="1"||cse->lexeme=="2"||cse->lexeme=="3"||cse->lexeme=="4"||cse->lexeme=="5"||cse->lexeme=="6"||cse->lexeme=="7"||cse->lexeme=="8"||cse->lexeme=="9"){
        return true;
        
    }
    
    
    
    return false;
    
}



/*void Parser::nextSymbol(stmt *param3, Token *tPointer){
    Token checkIfoperator=peek();
    //This block of code checks the next token to see weather or not to op1 or op2
    if(checkIfoperator.token_type==PLUS || checkIfoperator.token_type==MINUS || checkIfoperator.token_type==MULT || checkIfoperator.token_type==DIV){
        
        
        if(isNum(&checkIfoperator)){
            param3->op1=findAddress(&checkIfoperator);
        }
        
        
        param3->op1=symbol_Table[tPointer->lexeme];
    }else {
        
        if(isNum(&checkIfoperator)){
            param3->op1=findAddress(&checkIfoperator);
        }
        
        param3->op2=symbol_Table[tPointer->lexeme];
    }
    
    
}*/



int Parser::findAddress(Token *token){
    for(int it=0; it!=mem.size(); it++){
        if(mem.at(it)==atoi(token->lexeme.c_str())){
            return it;
        }
        
    }
    
    return -100;
    
    
}

void Parser::checkIfInSymbolTable(Token *k){
    if(symbol_Table.find(k->lexeme)==symbol_Table.end()){
        mem.at(next_available)=0;
        symbol_Table[k->lexeme]=next_available;
        next_available++;
    }
    
    
}

void Parser::addToLinkedList(stmt *statement){
    stmt *iterator;
    
    if(program==nullptr){
        program=statement;
        statement->next=nullptr;
    }
    else if(program!=nullptr){
        iterator=program;
        
        while(iterator->next!=nullptr){//traverse down to end of linked list
            iterator=iterator->next;
        }
        iterator->next=statement;
        statement->next=nullptr;
    
    }
    
    
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// this function simply checks the next token without
// consuming the input
// Written by Mohsen Zohrevandi
Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Parsing
void Parser::parse_inputs(){
    Token t=peek();
    if(t.token_type==NUM){
        lexer.GetToken();
        int yay=atoi(t.lexeme.c_str());
        //vector<int>::iterator it=inputsToProgram.begin();
        //inputsToProgram.insert(it,yay );
        inputsToProgram.push_back(yay);
        parse_inputs();
        return;
        }
    else if(t.token_type==END_OF_FILE){
        return;
    }
    syntax_error();
    
    
}
    
    
    
    


void Parser::parse_primary(stmt *par2){
    Token t2=peek();
    Token *forFunction=&t2;
    
    
    if(t2.token_type==ID){
        //check if t2 is in symboltable
        checkIfInSymbolTable(forFunction);
    
        lexer.GetToken();
        if(calledPrimayOnce==false){
            
            par2->op1=symbol_Table[t2.lexeme];
           
        }
        else {  par2->op2=symbol_Table[t2.lexeme]; }
        
       
        
        return;
    }else if(t2.token_type==NUM){
        lexer.GetToken();
        mem[next_available]=atoi(t2.lexeme.c_str());
        next_available++;
        
        if(calledPrimayOnce==false){
            
            par2->op1=findAddress(&t2);
            
        }
        else {  par2->op2=findAddress(&t2); }
        
        
        return;
    }
    else {
        lexer.GetToken();
        syntax_error();
    }
    
    
    
}



void Parser::parse_operator(stmt *arg1){
    Token t3=peek();
    if(t3.token_type==PLUS){
        lexer.GetToken();
        arg1->operatorn=7;
        return;
    }else if(t3.token_type==MINUS){
        lexer.GetToken();
        arg1->operatorn=8;
        return;
    } else if(t3.token_type==DIV){
        lexer.GetToken();
        arg1->operatorn=9;
        return;
    }else if(t3.token_type==MULT){
        lexer.GetToken();
        arg1->operatorn=10;
        return;
    }
    else {
        arg1->operatorn=-1;
    }
    
}



void Parser::parse_expr(stmt *par1){
   calledPrimayOnce=false;//initalize
    
    parse_primary(par1);
    calledPrimayOnce=true;
    
    Token v=peek();
    
    if(v.token_type==PLUS || v.token_type==MINUS || v.token_type==MULT || v.token_type==DIV){
    parse_operator(par1);
    parse_primary(par1);
    }
}



struct stmt* Parser::parse_assign_statement(){
    Token t4=peek();
    stmt *p=new stmt();
    p->stmt_type=3;
    
    
    
    if(t4.token_type==ID){
        if(symbol_Table.find(t4.lexeme)==symbol_Table.end()){
            symbol_Table[t4.lexeme]=next_available;
            next_available++;
            
            
        }
        p->LHS=symbol_Table[t4.lexeme];//assigning left hand sign to memory adr of lhs
        
        lexer.GetToken();
        t4=peek();
        if(t4.token_type==EQUAL){
            lexer.GetToken();
            parse_expr(p);
            t4=peek();
                if(t4.token_type==SEMICOLON){
                    lexer.GetToken();
                    //addToLinkedList(p);
                    return p;
                } else {
                    syntax_error();
                    return nullptr;
                }
            } else {
                syntax_error();
                return nullptr;
            }
        } else {
            syntax_error();
            return nullptr;
        }
    }




void Parser::parse_procedure_name(){
    Token t5=peek();
    if(t5.token_type==ID){
        lexer.GetToken();
        return;
    } else if(t5.token_type==NUM){
        lexer.GetToken();
        return;
    }
    else{
        syntax_error();
    }
}




struct stmt* Parser::parse_procedure_invocation(){
    parse_procedure_name();
    
    stmt* nextNode=new stmt();
    nextNode->stmt_type=5;
    
    Token t7=peek();
        if(t7.token_type==SEMICOLON){
            lexer.GetToken();
            return nextNode;
        } else{
            syntax_error();
        }
    
    return nullptr;
    
}



struct stmt* Parser::parse_output_statement(){
    Token t8=peek();
    if(t8.token_type==OUTPUT){
        lexer.GetToken();
        t8=peek();
        if(t8.token_type==ID){// If not found in symbol table allocate mem
            if(symbol_Table.find(t8.lexeme)==symbol_Table.end()){
                mem.at(next_available)=0;
                symbol_Table[t8.lexeme]=next_available;
                next_available++;
            }
            
            stmt* node=new stmt();
            node->op1=symbol_Table.find(t8.lexeme)->second;
            node->stmt_type=2;
            
            
            lexer.GetToken();
            t8=peek();
            if(t8.token_type==SEMICOLON){
                lexer.GetToken();
                return node;
            } else {
                syntax_error();
            }
        } else {
            syntax_error();
        }
    } else {
        syntax_error();
    }
    
    return nullptr;
}





struct stmt*  Parser::parse_input_statement(){
    Token t9=peek();
    if(t9.token_type==INPUT){
        lexer.GetToken();
        t9=peek();
        if(t9.token_type==ID){// If not found in symbol table
            if(symbol_Table.find(t9.lexeme)==symbol_Table.end()){
                mem.at(next_available)=0;
                symbol_Table[t9.lexeme]=next_available;
                next_available++;
            }
            
            stmt* a=new stmt();
            a->op1=symbol_Table.find(t9.lexeme)->second;
            a->stmt_type=1;
            
            lexer.GetToken();
            t9=peek();
            if(t9.token_type==SEMICOLON){
                lexer.GetToken();
                return a;
            } else {
                syntax_error();
            }
        } else {
            syntax_error();
        }
    } else {
        syntax_error();
    }
    return nullptr;
    
}






struct stmt* Parser::parse_do_statement(){
    Token z=peek();
    
    stmt* newNode=new stmt();
    newNode->op1=0;
    newNode->op2=0;
    newNode->LHS=0;
    newNode->operatorn=-20;
    newNode->stmt_type=4;
    
    if(z.token_type==DO){
        lexer.GetToken();
        Token z=peek();
        if(z.token_type==ID){
            lexer.GetToken();
            Token z=peek();
            parse_procedure_invocation();
            return newNode;
            } else {
                syntax_error();
            }
        } else {
            syntax_error();
        }
    
    return nullptr;
    
    
}



struct stmt* Parser::parse_statement(){
    
    Token y=peek();
    if(y.token_type==INPUT){
        return parse_input_statement();
    } else if(y.token_type==OUTPUT){
        return parse_output_statement();
    } else if(y.token_type==DO){
        return parse_do_statement();
        
    }
    
    Token z1=lexer.GetToken();
    Token z2=lexer.GetToken();
    lexer.UngetToken(z2);
    lexer.UngetToken(z1);
    
    
    if(z1.token_type==ID && z2.token_type==SEMICOLON){
        return parse_procedure_invocation();
    }else if(z1.token_type==ID && z2.token_type==EQUAL){
        return parse_assign_statement();
    }
    
    else{
        return nullptr;
    }
    
    
    
}




struct stmt* Parser::parse_statement_list(){
    struct stmt* st;
    struct stmt* stl;
    
    st=parse_statement();
    
    if(st==nullptr){
        return st;
    }
    
    if(st!=nullptr){
        stl=parse_statement_list();
        st->next=stl;
        return st;
    }
    
    return nullptr;
    
    
}
    
    




struct stmt* Parser::parse_procedure_body(){
    stmt* lel=new stmt();
    lel=parse_statement_list();
    if(lel!=nullptr){
        return lel;
    }
    else {
        return nullptr;
    }
}




bool Parser::parse_proc_decl(){
    Token z3=peek();
    if(z3.token_type==PROC){
        lexer.GetToken();
        z3=peek();
        parse_procedure_name();
        if(parse_procedure_body()!=nullptr){
                z3=peek();
                if(z3.token_type==ENDPROC){
                    lexer.GetToken();
                    return true;
                } else {
                    return false;
                }
    }
    }
    return false;
    
    
    
}






bool Parser::parse_proc_decl_section(){
    if(parse_proc_decl()){
        if(parse_proc_decl_section()){
            return true;
        }
        return true;
    } else {
        return false;
    }
    

    
}



void Parser::parse_main(){
    Token z4=peek();
    if(z4.token_type==MAIN){
        lexer.GetToken();
        program=parse_procedure_body();
        return;
        }
    else {
        syntax_error();
    }
    
}




void Parser::parse_program(){
    if(parse_proc_decl_section()){
           parse_main();
           return;
        }
    
    else{
    parse_main();
        return;
    }
    

}



void Parser::parse_input(){
    parse_program();
    parse_inputs();
    execute_program(program);
 
}
 




void Parser::execute_program(struct stmt* start){
    struct stmt* pc;
    pc = start;
    while(pc!=NULL){
        switch(pc->stmt_type){
                
            case 1://case input
                mem[pc->op1]=inputsToProgram[next_input];
                next_input++;
                break;
                
            case 2:
                cout << mem[pc->op1] << " ";
                break;
                
                
                
            case 3: //case assign
                switch(pc->operatorn){
            case 7:
                mem[pc->LHS]=mem[pc->op1]+mem[pc->op2];
                break;
                
            case 8:
                mem[pc->LHS]=mem[pc->op1]-mem[pc->op2];
                break;
                
            case 9:
                mem[pc->LHS]=mem[pc->op1]*mem[pc->op2];
                break;
                }
                
                
        
                
            
                
        }
        pc=pc->next;
    }
    
}
























int main()
{
    /*
     LexicalAnalyzer lexer;
     Token token;
     
     token = lexer.GetToken();
     printf("\n");
     token.Print();
     while (token.token_type != END_OF_FILE)
     {
     token = lexer.GetToken();
     token.Print();
     }*/
    
    
    //Parser *p=new Parser();
    //p->parse_operator();
    
    Parser parser;
    parser.parse_input();
    
    
    ;
    
    
    
}



