
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
std::vector<int> mem(1000);
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
void Parser::parse_inputs(){
    Token t=peek();
    if(t.token_type==NUM){
        lexer.GetToken();
        parse_inputs();
        return;
        }
    else if(t.token_type==END_OF_FILE){
        return;
    }
    syntax_error();
    
    
}
    
    
    
    


void Parser::parse_primary(){
    Token t2=peek();
    
    
    if(t2.token_type==ID){
        if(symbol_Table.find(t2.lexeme)==symbol_Table.end()){
            mem.at(next_available)=0;
            symbol_Table[t2.lexeme]=next_available;
            next_available++;
        }
        
        lexer.GetToken();
        return;
    }else if(t2.token_type==NUM){
        lexer.GetToken();
        mem[next_available]=atoi(t2.lexeme.c_str());
        next_available++;
        return;
    }
    else {
        lexer.GetToken();
        syntax_error();
    }
    
    
    
}



void Parser::parse_operator(){
    Token t3=peek();
    if(t3.token_type==PLUS){
        lexer.GetToken();
        return;
    }else if(t3.token_type==MINUS){
        lexer.GetToken();
        return;
    } else if(t3.token_type==DIV){
        lexer.GetToken();
        return;
    }else if(t3.token_type==MULT){
        lexer.GetToken();
        return;
    }
    else {
        syntax_error();
    }
    
}



void Parser::parse_expr(){
    
    parse_primary();
    Token v=peek();
    
    if(v.token_type==PLUS || v.token_type==MINUS || v.token_type==MULT || v.token_type==DIV){
    parse_operator();
    parse_primary();
    }
}



void Parser::parse_assign_statement(){
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
            parse_expr();
            t4=peek();
                if(t4.token_type==SEMICOLON){
                    lexer.GetToken();
                    return;
                } else {
                    syntax_error();
                }
            } else {
                syntax_error();
            }
        } else {
            syntax_error();
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




void Parser::parse_procedure_invocation(){
    parse_procedure_name();
    Token t7=peek();
        if(t7.token_type==SEMICOLON){
            lexer.GetToken();
            return;
        } else{
            syntax_error();
        }
    
}



void Parser::parse_output_statement(){
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
                return;
            } else {
                syntax_error();
            }
        } else {
            syntax_error();
        }
    } else {
        syntax_error();
    }
}





void Parser::parse_input_statement(){
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
            
            stmt_type* a=new stmt_type();
            a->op1=symbol_Table.find(t9.lexeme)->second;
            a->op2=0;
            a->LHS=0;
            a->operatorn=-20;
            a->stmt_type=1;
            
            lexer.GetToken();
            t9=peek();
            if(t9.token_type==SEMICOLON){
                lexer.GetToken();
                return;
            } else {
                syntax_error();
            }
        } else {
            syntax_error();
        }
    } else {
        syntax_error();
    }
    
}






void Parser::parse_do_statement(){
    Token z=peek();
    if(z.token_type==DO){
        lexer.GetToken();
        Token z=peek();
        if(z.token_type==ID){
            lexer.GetToken();
            Token z=peek();
            parse_procedure_invocation();
            return;
            } else {
                syntax_error();
            }
        } else {
            syntax_error();
        }
    
    
    
}



bool Parser::parse_statement(){
    
    Token y=peek();
    if(y.token_type==INPUT){
        parse_input_statement();
        return true;
    } else if(y.token_type==OUTPUT){
        parse_output_statement();
        return true;
    } else if(y.token_type==DO){
        parse_do_statement();
        return true;
    }
    
    Token z1=lexer.GetToken();
    Token z2=lexer.GetToken();
    lexer.UngetToken(z2);
    lexer.UngetToken(z1);
    
    
    if(z1.token_type==ID && z2.token_type==SEMICOLON){
        parse_procedure_invocation();
        return true;
    }else if(z1.token_type==ID && z2.token_type==EQUAL){
        parse_assign_statement();
        return true;
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
    
    




void Parser::parse_procedure_body(){
    if(parse_statement_list()){
        return;
    }
    else {
        syntax_error();
    }
}




bool Parser::parse_proc_decl(){
    Token z3=peek();
    if(z3.token_type==PROC){
        lexer.GetToken();
        z3=peek();
        parse_procedure_name();
        parse_procedure_body();
                z3=peek();
                if(z3.token_type==ENDPROC){
                    lexer.GetToken();
                    return true;
                } else {
                    return false;
                }
    }
                else {
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



void Parser::parse_main(){
    Token z4=peek();
    if(z4.token_type==MAIN){
        lexer.GetToken();
        parse_procedure_body();
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



