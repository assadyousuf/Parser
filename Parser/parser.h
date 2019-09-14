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

struct stmt_type {
    int stmt_type;
    int LHS;
    int operatorn;
    int op1;
    int op2;
    struct stmt_type *next;
    
    
};

class Parser {
private:
    LexicalAnalyzer lexer;
    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();
    
public:
    void parse_inputs();
    void parse_primary();
    void parse_operator();
    void parse_expr();
    void parse_assign_statement();
    void parse_do_statement();
    void parse_procedure_name();
    void parse_procedure_invocation();
    void parse_output_statement();
    void parse_input_statement();
    bool parse_statement();
    bool parse_statement_list();
    void parse_procedure_body();
    bool parse_proc_decl();
    bool parse_proc_decl_section();
    void parse_main();
    void parse_program();
    void parse_input();
};

#endif


