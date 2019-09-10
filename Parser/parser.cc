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
        t= lexer.GetToken();
        t = peek();
        if(t.token_type == NUM){
            parse_inputs();
            return true;
        } else if (t.token_type==END_OF_FILE){
            return true;
        }
        else{
            return false;
        }
    }
    else {
        return false;
    }
   
}

bool Parser::parse_primary(){
    Token t=peek();
    if(t.token_type==ID){
        lexer.GetToken();
        return true;
    }else if(t.token_type==NUM){
        lexer.GetToken();
        return true;
    }
    else {
        lexer.GetToken();
        return false;
    }
    
    
    
}

bool Parser::parse_operator(){
    Token t=peek();
    if(t.token_type==PLUS){
        lexer.GetToken();
        return true;
    }else if(t.token_type==MINUS){
        lexer.GetToken();
        return true;
    } else if(t.token_type==DIV){
        lexer.GetToken();
        return true;
    }else if(t.token_type==MULT){
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
    Token t=peek();
    if(t.token_type==ID){
        lexer.GetToken();
        t=peek();
        if(t.token_type==EQUAL){
            lexer.GetToken();
            if(parse_expr()){
                t=peek();
                if(t.token_type==SEMICOLON){
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
    Token t=peek();
    if(t.token_type==ID){
        lexer.GetToken();
        return true;
    } else if(t.token_type==NUM){
        lexer.GetToken();
        return true;
    }
    else{
        return false;
    }
}


bool Parser::parse_procedure_invocation(){
    if(parse_procedure_name()){
        Token t=peek();
        if(t.token_type==SEMICOLON){
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
    Token t=peek();
    if(t.token_type==OUTPUT){
        lexer.GetToken();
        t=peek();
        if(t.token_type==ID){
            lexer.GetToken();
            t=peek();
            if(t.token_type==SEMICOLON){
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

bool Parser::parse_input_statement(){
    Token t=peek();
    if(t.token_type==INPUT){
        lexer.GetToken();
        t=peek();
        if(t.token_type==ID){
            lexer.GetToken();
            t=peek();
            if(t.token_type==SEMICOLON){
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


bool Parser::parse_do_statement(){
    Token t=peek();
    if(t.token_type==DO){
        lexer.GetToken();
        Token t=peek();
        if(t.token_type==ID){
            lexer.GetToken();
            Token t=peek();
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
    Token t=peek();
    if(parse_input_statement()){
        return true;
    } else if(parse_output_statement()){
        return true;
    } else if(parse_do_statement()){
        return true;
    }else if(parse_procedure_invocation()){
        return true;
    }else if(parse_assign_statement()){
        return true;
    }else {
        return false;
    }
}

bool Parser::parse_statement_list(){
    Token t=peek();
    if(parse_statement()){
        lexer.GetToken();
        if(parse_statement_list()){
            lexer.GetToken();
            return true;
        } else {
            return false;
        }
        
        
        return true;
    } else {
        return false;
    }
    
}






    
    












    






int main()
{
    /*
    LexicalAnalyzer lexer;
    Token token;
    
    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
     */
   // Parser *p=new Parser();
    //p->parse_operator();
    Parser parser;
    if(parser.parse_statement_list()) {
        cout<<"IT WORKS\n";
    } else {
        cout<<"IT DOES NOT WORK ):\n";
    }


}

