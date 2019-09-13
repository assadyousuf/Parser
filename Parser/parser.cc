
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
std::vector<int> mem(10);
int next_available=0;

using namespace std;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
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
bool Parser::parse_inputs(){
    Token t=peek();
    if(t.token_type==NUM){
        lexer.GetToken();
        if(parse_inputs()){
            return true;
        }
    }else if(t.token_type==END_OF_FILE){
        return true;
    }
    return false;
    
    
    
}

bool Parser::parse_primary(){
    Token t2=peek();
    
    
    if(t2.token_type==ID){
        if(symbol_Table.find(t2.lexeme)==symbol_Table.end()){
            mem.at(next_available)=0;
            symbol_Table[t2.lexeme]=next_available;
            next_available++;
        }
        
        lexer.GetToken();
        return true;
    }else if(t2.token_type==NUM){
        lexer.GetToken();
        mem[next_available]=atoi(t2.lexeme.c_str());
        next_available++;
        return true;
    }
    else {
        lexer.GetToken();
        return false;
    }
    
    
    
}

bool Parser::parse_operator(){
    Token t3=peek();
    if(t3.token_type==PLUS){
        lexer.GetToken();
        return true;
    }else if(t3.token_type==MINUS){
        lexer.GetToken();
        return true;
    } else if(t3.token_type==DIV){
        lexer.GetToken();
        return true;
    }else if(t3.token_type==MULT){
        lexer.GetToken();
        return true;
    }
    else {
        return false;
    }
    
}

bool Parser::parse_expr(){
    if(parse_primary()){
        if(parse_operator()){
            if(parse_primary()){
                return true;
            } else {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool Parser::parse_assign_statement(){
    Token t4=peek();
    if(t4.token_type==ID){
        if(symbol_Table.find(t4.lexeme)==symbol_Table.end()){
            symbol_Table[t4.lexeme]=next_available;
            next_available++;
            
        }
        
        lexer.GetToken();
        t4=peek();
        if(t4.token_type==EQUAL){
            lexer.GetToken();
            if(parse_expr()){
                t4=peek();
                if(t4.token_type==SEMICOLON){
                    lexer.GetToken();
                    return true;
                } else {
                    return false;
                }
            } else {
                return true;
            }
        } else {
            return true;
        }
    } else {
        return true;
    }
    
}

bool Parser::parse_procedure_name(){
    Token t5=peek();
    if(t5.token_type==ID){
        lexer.GetToken();
        return true;
    } else if(t5.token_type==NUM){
        lexer.GetToken();
        return true;
    }
    else{
        return false;
    }
}


bool Parser::parse_procedure_invocation(){
    if(parse_procedure_name()){
        Token t7=peek();
        if(t7.token_type==SEMICOLON){
            lexer.GetToken();
            return true;
        } else{
            return false;
        }
    } else{
        return false;
    }
    
}

bool Parser::parse_output_statement(){
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
            lexer.GetToken();
            t8=peek();
            if(t8.token_type==SEMICOLON){
                lexer.GetToken();
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

struct stmt_type* Parser::parse_input_statement(){
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
            
            stmt_type* t=new stmt_type();
            t->op1=symbol_Table.find(t9.lexeme)->second;
            t->op2=0;
            t->LHS=0;
            t->operatorn=-20;
            t->stmt_type=1;
            
            lexer.GetToken();
            t9=peek();
            if(t9.token_type==SEMICOLON){
                return t;
            } else {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
    }
    
}


bool Parser::parse_do_statement(){
    Token z=peek();
    if(z.token_type==DO){
        lexer.GetToken();
        Token z=peek();
        if(z.token_type==ID){
            lexer.GetToken();
            Token z=peek();
            if(parse_procedure_invocation()){
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
    
    
}


bool Parser::parse_statement(){
    
    if(parse_input_statement()){
        return true;
    } else if(parse_output_statement()){
        return true;
    } else if(parse_do_statement()){
        return true;
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
        return false;
    }
    
    
    
}

bool Parser::parse_statement_list(){
    if(parse_statement()){
        if(parse_statement_list()){
            return true;
        }
        return true;
    } else {
        return false;
    }
    
}

bool Parser::parse_procedure_body(){
    if(parse_statement_list()){
        return true;
    }
    else {
        return false;
    }
}

bool Parser::parse_proc_decl(){
    Token z3=peek();
    if(z3.token_type==PROC){
        lexer.GetToken();
        z3=peek();
        if(parse_procedure_name()){
            if(parse_procedure_body()){
                z3=peek();
                if(z3.token_type==ENDPROC){
                    lexer.GetToken();
                    return true;
                } else {
                    return false;
                }
            }else {
                return false;
            }
        }else {
            return false;
        }
    }else {
        return false;
    }
    
    
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

bool Parser::parse_main(){
    Token z4=peek();
    if(z4.token_type==MAIN){
        lexer.GetToken();
        if(parse_procedure_body()){
            return true;
        } else {
            return false;
        }
    }else {
        return false;
    }
    
}


bool Parser::parse_program(){
    if(parse_proc_decl_section()){
        if(parse_main()){
            return true;
        }else{
            return false;
        }
        
    } else if(parse_main()){
        return true;
    } else {
        return false;
    }
    
}

bool Parser::parse_input(){
    if(parse_program()){
        if(parse_inputs()){
            return true;
        }else {
            syntax_error();
            return false;
        }
    }else {
        syntax_error();
        return false;
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
    if(parser.parse_input_statement()!=nullptr) {
        cout<<"IT WORKS";
    } else {
        cout<<"IT DOES NOT WORK ):";
    }
    
    
    ;
    
    
    
}



