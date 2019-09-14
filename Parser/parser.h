/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#include <map>
#include <vector>

struct stmt {
    int stmt_type;
    int LHS;
    int operatorn;
    int op1;
    int op2;
    stmt *next;
    stmt *startOfProcedure=nullptr;
    
};

class Parser {
private:
    LexicalAnalyzer lexer;
    void syntax_error();
    void nextSymbol(stmt *param3,Token *tPointer);
    void addToLinkedList(stmt *statement);
    void checkIfInSymbolTable(Token *l);
    void plzwork(Token *ko);
    int findAddress(Token *token);
    bool isNum(Token *t);
    
    Token expect(TokenType expected_type);
    Token peek();
    
public:
    void parse_inputs();
    void parse_primary(stmt *par1);
    void parse_operator(stmt *ads);
    void parse_expr(stmt *par1);
    struct stmt* parse_assign_statement();
    struct stmt* parse_do_statement();
    void parse_procedure_name();
    struct stmt* parse_procedure_invocation();
    struct stmt*  parse_output_statement();
    struct stmt*  parse_input_statement();
    struct stmt* parse_statement();
    struct stmt* parse_statement_list();
    struct stmt*  parse_procedure_body();
    bool parse_proc_decl();
    bool parse_proc_decl_section();
    void parse_main();
    void parse_program();
    void parse_input();
    void execute_program(struct stmt* start);
};

#endif


