/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"

class Parser {
private:
    LexicalAnalyzer lexer;
    
    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();
    
public:
    bool parse_inputs();
    bool parse_primary();
    bool parse_operator();
    bool parse_expr();
    bool parse_assign_statement();
    bool parse_do_statement();
    bool parse_procedure_name();
    bool parse_procedure_invocation();
    bool parse_output_statement();
    bool parse_input_statement();
    bool parse_statement();
    bool parse_statement_list();
};

#endif


