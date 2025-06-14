/* -------PROLOGUE------- */
%{
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexer.h"
#include "ast.h"

ASTNode* astRoot = NULL;

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

int yylex(void);

%}

%union {
    const char* stringValue;   /* STRING, nameNode etc */
    double      numberValue;   /* NUMBER */
    bool        boolValue;     /* TRUE, FALSE */
    ASTNode*    node;          /* nonterminals */
    int         opType;        /* operators */
    Modifier    modifier;      /* modifiers like CONST */
}

/* literals */
%token <stringValue> STRING
%token <numberValue> NUMBER
%token <boolValue> TRUE FALSE

/* identifier */
%token <stringValue> NAME

/* arithmetic ops types */
%token <opType> MINUS PLUS BANG STAR SLASH MOD UNARY_DEC UNARY_INC

/* assignment ops types */
%token <opType> EQUALS ASSIGN_ADD ASSIGN_SUB ASSIGN_DIV ASSIGN_MUL ASSIGN_MOD
%token <opType> ASSIGN_XOR ASSIGN_SHIFTL ASSIGN_SHIFTR ASSIGN_AND ASSIGN_OR

/* bitwise ops types */
%token <opType> BIT_SHIFTL BIT_SHIFTR BIT_AND BIT_OR BIT_XOR

/* conditional ops types */
%token <opType> COND_GT COND_GEQ COND_LT COND_LEQ COND_EQ COND_NEQ LOGIC_AND LOGIC_OR

/* member access op type */
%token <opType> DOT


/* braces and other delimiters */
%token LPAREN RPAREN       /* ( ) */
%token LANGLE RANGLE       /* < > */
%token LBRACE RBRACE       /* { } */
%token LSQUARE RSQUARE     /* [ ] */
%token COLON               /* : */
%token COMMA               /* , */
%token HASH                /* # for lambda */
%token DELIMITER           /* end of line */
%token ENDOFTOKENS         /* end of tokens indicator */

/* keywords */
%token FN RET IF ELSE WHL FOR SWITCH BRK DEFAULT CLASS TOSS DEL ENUM

%token <modifier> CONST VAR STATIC HIDDEN

%type <node> program

/* program statements list */
%type <node> opt_program_stmt_list
%type <node> program_stmt_list
%type <node> program_stmt

/* global statements */
%type <node> global_stmt
%type <node> global_stmt_stmt
%type <node> class_def
%type <node> fn_def
%type <node> enum_stmt

/* regular statements */
%type <node> stmt
%type <node> stmt_stmt

/* code block */
%type <node> code_block
%type <node> opt_stmt_list
%type <node> stmt_list
%type <node> code_block_or_stmt

/* del statement */
%type <node> del_stmt

/* assignment */
%type <node> assign
%type <node> assign_rhs

/* control statements */
%type <node> cntrl_cnstrct
%type <node> loop_whl
%type <node> loop_for
%type <node> loop_for_init
%type <node> loop_for_cond
%type <node> loop_for_post_iter
%type <node> ret_stmt
%type <node> switch_stmt
%type <node> opt_switch_case_list
%type <node> switch_case_list
%type <node> switch_case
%type <node> brk_stmt
%type <node> if_stmt
%type <node> opt_else
%type <node> toss_stmt

/* operators */
%type <opType> op_assign
%type <opType> op_logic
%type <opType> op_cond
%type <opType> op_bitwise_xor
%type <opType> op_bitwise_or
%type <opType> op_bitwise_and
%type <opType> op_bitwise_shift
%type <opType> op_arithm_add
%type <opType> op_arithm_mul
%type <opType> op_unary_neg
%type <opType> op_unary_logic_neg
%type <opType> op_unary_inc

/* expression grammar */
%type <node> expr
%type <node> lvl_assign
%type <node> lvl_logic
%type <node> lvl_cond
%type <node> lvl_bitwise_xor
%type <node> lvl_bitwise_or
%type <node> lvl_bitwise_and
%type <node> lvl_bitwise_shift
%type <node> lvl_arithm_add
%type <node> lvl_arithm_mul
%type <node> lvl_unary_neg
%type <node> lvl_unary_inc
%type <node> lvl_postfix
%type <node> primary

/* expressions list */
%type <node> expr_list
%type <node> opt_expr_list
%type <node> opt_expr

/* literals and arrays */
%type <node> literal
%type <node> arr_initializer

/* node for identifier */
%type <node> nameNode

/* enum definition */
%type <node> enum_element
%type <node> enum_opt_elements
%type <node> enum_elements

/* function definition */
%type <node> fn_parameter
%type <node> fn_opt_params_list
%type <node> fn_params_list

/* variable declaration */
%type <node> var_decl
%type <node> var_decl_assign_list
%type <node> var_decl_assign
%type <modifier> var_decl_modifiers
%type <modifier> var_decl_modifier
%type <node> var_type

/* lambda */
%type <node> lambda_expr
%type <node> lambda_opt_capture_list
%type <node> lambda_capture_list

/* class definition */
%type <node> opt_class_member_list
%type <node> class_member_list
%type <node> class_member
%type <modifier> class_member_mod_list
%type <modifier> class_member_mod

/* set assign operators as non-associative */
%precedence EQUALS ASSIGN_ADD ASSIGN_SUB ASSIGN_DIV ASSIGN_MUL ASSIGN_MOD
%precedence ASSIGN_XOR ASSIGN_SHIFTL ASSIGN_SHIFTR ASSIGN_AND ASSIGN_OR

%precedence LOWER_THAN_ELSE
%precedence ELSE

/* operator priorities */
%left LOGIC_AND LOGIC_OR
%left COND_GT COND_GEQ COND_LT COND_LEQ COND_EQ COND_NEQ
%left BIT_XOR
%left BIT_OR
%left BIT_AND
%left BIT_SHIFTL BIT_SHIFTR
%left PLUS MINUS
%left STAR SLASH
%right UMINUS BANG
%right UNARY_DEC UNARY_INC
%left DOT

%%
/* -------GRAMMAR RULES------- */
    program
        : opt_program_stmt_list
            {
                astRoot = $1;
            }
    ;

    opt_delimiter_list
        : delimiter_list
        |
    ;

    delimiter_list
        : delimiter_list DELIMITER
        | DELIMITER
    ;

    opt_program_stmt_list
        : program_stmt_list opt_delimiter_list
            {
                $$ = $1;
            }
        | opt_delimiter_list
            {
                $$ = NULL;
            }
    ;

    program_stmt_list
        : program_stmt_list delimiter_list program_stmt
            {
                astNodeLLAppend($1->data.nodeList.list, $3);
                $$ = $1;
            }
        | program_stmt
            {
                $$ = newNodeList($1);
            }
    ;

    program_stmt
        : stmt
            {
                $$ = $1;
            }
        | global_stmt
            {
                $$ = $1;
            }
    ;

/* global statement */
    global_stmt
        : global_stmt_stmt
            {
                $$ = $1;
            }
    ;

    global_stmt_stmt
        : class_def
            {
                $$ = $1;
            }
        | fn_def
            {
                $$ = $1;
            }
        | enum_stmt
            {
                $$ = $1;
            }
    ;

/* regular statement */
    stmt
        : stmt_stmt
            {
                $$ = $1;
            }
    ;

    stmt_stmt
        : var_decl
            {
                $$ = $1;
            }
        | cntrl_cnstrct
            {
                $$ = $1;
            }
        | del_stmt
            {
                $$ = $1;
            }
        | assign
            {
                $$ = $1;
            }
        | expr
            {
                $$ = $1;
            }
    ;

/* code block with braces */
    code_block
        : opt_delimiter_list LBRACE opt_delimiter_list opt_stmt_list RBRACE
            {
                $$ = $4;
            }
    ;

    opt_stmt_list
        : stmt_list
            {
                $$ = $1;
            }
        |
            {
                $$ = NULL;
            }
    ;

    stmt_list
        : stmt_list delimiter_list stmt_stmt
            {
                astNodeLLAppend($1->data.nodeList.list, $3);
                $$ = $1;
            }
        | stmt_stmt
            {
                $$ = newNodeList($1);
            }
    ;

    code_block_or_stmt
        : code_block
            {
                $$ = $1;
            }
        | opt_delimiter_list stmt
            {
                $$ = $2;
            }
    ;

/* del statement */
    del_stmt
        : DEL expr
            {
                $$ = newDelStmt($2);
            }
    ;

/* control constructs */
    cntrl_cnstrct
        : loop_whl
            {
                $$ = $1;
            }
        | loop_for
            {
                $$ = $1;
            }
        | switch_stmt
            {
                $$ = $1;
            }
        | ret_stmt
            {
                $$ = $1;
            }
        | brk_stmt
            {
                $$ = $1;
            }
        | if_stmt
            {
                $$ = $1;
            }
        | toss_stmt
            {
                $$ = $1;
            }
    ;

    loop_whl
        : WHL LPAREN expr RPAREN code_block_or_stmt
            {
                $$ = newLoopWhl($3, $5);
            }
    ;

    loop_for
        : FOR LPAREN loop_for_init COLON loop_for_cond COLON loop_for_post_iter RPAREN code_block_or_stmt
            {
                $$ = newLoopFor($3, $5, $7, $9);
            }
    ;

    loop_for_init
        : opt_expr_list
            {
                $$ = $1;
            }
    ;

    loop_for_cond
        : loop_for_cond COMMA lvl_logic
            {
                astNodeLLAppend($1->data.nodeList.list, $3);
                $$ = $1;
            }
        | lvl_logic
            {
                $$ = newNodeList($1);
            }
    ;

    loop_for_post_iter
        : opt_expr_list
            {
                $$ = $1;
            }
    ;

    ret_stmt
        : RET
            {
                $$ = newRetStmt(NULL);
            }
        | RET expr
            {
                $$ = newRetStmt($2);
            }
    ;

    switch_stmt
        : SWITCH LPAREN expr RPAREN LBRACE opt_switch_case_list opt_delimiter_list RBRACE 
            {
                $$ = newSwitchStmt($3, $6);
            }
    ;

    opt_switch_case_list
        : switch_case_list
            {
                $$ = $1;
            }
        |
            {
                $$ = NULL;
            }
    ;

    switch_case_list
        : switch_case_list delimiter_list switch_case
            {
                astNodeLLAppend($1->data.nodeList.list, $3);
                $$ = $1;
            }
        | switch_case
            {
                $$ = newNodeList($1);
            }
    ;

    switch_case
        : expr COLON code_block_or_stmt
            {
                $$ = newSwitchCase($1, $3, false);
            }
        | DEFAULT COLON code_block_or_stmt
            {
                $$ = newSwitchCase(NULL, $3, true);
            }
    ;

    brk_stmt
        : BRK LPAREN expr RPAREN
            {
                $$ = newBrkStmt($3);
            }
        | BRK
            {
                $$ = newBrkStmt(NULL);
            }
    ;

    if_stmt
        : IF LPAREN expr RPAREN code_block_or_stmt opt_else
            {
                $$ = newIfStmt($3, $5, $6);
            }
    ;

    opt_else
        : ELSE code_block_or_stmt
            {
                $$ = $2;
            }
        |                           %prec LOWER_THAN_ELSE
            {
                $$ = NULL;
            }
    ;

    toss_stmt
        : TOSS expr
            {
                $$ = newTossStmt($2);
            }
    ;

/* operators */
    op_assign
        : EQUALS
            {
                $$ = $1;
            }
        | ASSIGN_ADD
            {
                $$ = $1;
            }
        | ASSIGN_SUB
            {
                $$ = $1;
            } 
        | ASSIGN_DIV
            {
                $$ = $1;
            }
        | ASSIGN_MUL
            {
                $$ = $1;
            }
        | ASSIGN_MOD
            {
                $$ = $1;
            }
        | ASSIGN_XOR
            {
                $$ = $1;
            }
        | ASSIGN_SHIFTL
            {
                $$ = $1;
            }
        | ASSIGN_SHIFTR
            {
                $$ = $1;
            }
        | ASSIGN_AND
            {
                $$ = $1;
            }
        | ASSIGN_OR
            {
                $$ = $1;
            }
    ;

    op_logic
        : LOGIC_AND
            {
                $$ = $1;
            }
        | LOGIC_OR
            {
                $$ = $1;
            }
    ;

    op_cond
        : COND_GT
            {
                $$ = $1;
            }
        | COND_GEQ
            {
                $$ = $1;
            }
        | COND_LT
            {
                $$ = $1;
            }
        | COND_LEQ
            {
                $$ = $1;
            }
        | COND_EQ
            {
                $$ = $1;
            }
        | COND_NEQ
            {
                $$ = $1;
            }
    ;

    op_bitwise_xor
        : BIT_XOR
            {
                $$ = $1;
            }
    ;

    op_bitwise_or
        : BIT_OR
            {
                $$ = $1;
            }
    ;

    op_bitwise_and
        : BIT_AND
            {
                $$ = $1;
            }
    ;

    op_bitwise_shift
        : BIT_SHIFTL
            {
                $$ = $1;
            }
        | BIT_SHIFTR
            {
                $$ = $1;
            }
    ;

    op_arithm_add
        : PLUS
            {
                $$ = $1;
            }
        | MINUS
            {
                $$ = $1;
            }
    ;

    op_arithm_mul
        : STAR
            {
                $$ = $1;
            }
        | SLASH
            {
                $$ = $1;
            }
    ;

    op_unary_neg
        : MINUS
            {
                $$ = $1;
            }
    ;

    op_unary_logic_neg
        : BANG
            {
                $$ = $1;
            }
    ;

    op_unary_inc
        : UNARY_DEC
            {
                $$ = $1;
            }
        | UNARY_INC
            {
                $$ = $1;
            }
    ;

/* expression grammar */
    expr
        : lvl_logic         
            { 
                $$ = $1; 
            }
    ;

    lvl_assign
        : lvl_assign op_assign lvl_logic
            {   
                $$ = newBinaryOp($2, $1, $3);   
            }
        | lvl_logic
            {   
                $$ = $1;    
            }
    ;

    lvl_logic
        : lvl_logic op_logic lvl_cond         
            {   
                $$ = newBinaryOp($2, $1, $3);   
            }
        | lvl_cond
            {   
                $$ = $1;    
            }
    ;

    lvl_cond
        : lvl_cond op_cond lvl_bitwise_xor
            {   
                $$ = newBinaryOp($2, $1, $3);   
            }
        | lvl_bitwise_xor
            {   
                $$ = $1;    
            }
    ;

    lvl_bitwise_xor
        : lvl_bitwise_xor op_bitwise_xor lvl_bitwise_or
            {   
                $$ = newBinaryOp($2, $1, $3);   
            }
        | lvl_bitwise_or
            {   
                $$ = $1;    
            }
    ;

    lvl_bitwise_or
        : lvl_bitwise_or op_bitwise_or lvl_bitwise_and
            {   
                $$ = newBinaryOp($2, $1, $3);   
            }
        | lvl_bitwise_and
            {   
                $$ = $1;    
            }
    ;

    lvl_bitwise_and
        : lvl_bitwise_and op_bitwise_and lvl_bitwise_shift
            {   
                $$ = newBinaryOp($2, $1, $3);   
            }
        | lvl_bitwise_shift
            {   
                $$ = $1;
            }
    ;

    lvl_bitwise_shift
        : lvl_bitwise_shift op_bitwise_shift lvl_arithm_add
            {   
                $$ = newBinaryOp($2, $1, $3);   
            }
        | lvl_arithm_add
            {   
                $$ = $1;    
            }
    ;

    lvl_arithm_add
        : lvl_arithm_add op_arithm_add lvl_arithm_mul
            {   
                $$ = newBinaryOp($2, $1, $3);   
            }
        | lvl_arithm_mul
            {   
                $$ = $1;    
            }
    ;

    lvl_arithm_mul
        : lvl_arithm_mul op_arithm_mul lvl_unary_neg
            {   
                $$ = newBinaryOp($2, $1, $3);   
            }
        | lvl_unary_neg
            {   
                $$ = $1;    
            }
    ;

    lvl_unary_neg
        : op_unary_neg lvl_unary_inc %prec UMINUS
            {
                $$ = newUnaryOp($1, $2);
            }
        | op_unary_logic_neg lvl_unary_inc
            {
                $$ = newUnaryOp($1, $2);
            }
        | lvl_unary_inc
            {
                $$ = $1;
            }
    ;

    lvl_unary_inc
        : op_unary_inc lvl_postfix
            {
                $$ = newUnaryOp($1, $2);
            }
        | lvl_postfix
            {
                $$ = $1;
            }
    ;

    lvl_postfix
        : lvl_postfix DOT nameNode
            {   
                $$ = newMemberAccess($1, $3);   
            }
        | lvl_postfix LPAREN opt_expr_list RPAREN
            {   
                $$ = newFnCall($1, $3);   
            }
        | lvl_postfix LSQUARE opt_expr RSQUARE
            {   
                $$ = newArrayAccess($1, $3);   
            }
        | primary
            {   
                $$ = $1;    
            }
    ;

    primary
        : literal
            {
                $$ = $1;
            }
        | nameNode
            {
                $$ = $1;
            }
        | LPAREN expr RPAREN
            {
                $$ = $2;
            }
    ;

    opt_expr 
        : expr
            {
                $$ = $1;
            }
        |
            {
                $$ = NULL;
            }
    ;

    opt_expr_list
        : expr_list
            {
                $$ = $1;
            }
        |
            {
                $$ = NULL;
            }
    ;

    expr_list
        : expr_list COMMA expr
            {
                astNodeLLAppend($1->data.nodeList.list, $3);
                $$ = $1;
            }
        | expr
            {
                $$ = newNodeList($1);
            }
    ;

    literal
        : TRUE
            { 
                $$ = newBoolValue(true); 
            }
        | FALSE
            {
                $$ = newBoolValue(false);
            }
        | NUMBER    
            {
                $$ = newNumberValue($1);
            }
        | arr_initializer
            {
                $$ = $1;
            }
        | STRING
            {
                $$ = newStringValue($1);
            }
    ;

/* array initializer */
    arr_initializer
        : LSQUARE RSQUARE
            {
                $$ = newArray(NULL);
            }
        | LSQUARE expr_list RSQUARE
            {
                $$ = newArray($2);
            }
    ;

/* name/identifier */
    nameNode
        : NAME
            {
                $$ = newName($1);
            }
    ;

/* enumeration */
    enum_stmt
        : ENUM nameNode LBRACE enum_opt_elements RBRACE
            {
                $$ = newEnumDef($2->data.stringValue, $4);
            }
    ;

    enum_element
        : delimiter_list nameNode
            {
                $$ = newEnumElement($2->data.stringValue, NULL);
            }
        | delimiter_list nameNode EQUALS expr
            {
                $$ = newEnumElement($2->data.stringValue, $4);
            }
    ;

    enum_opt_elements
        : enum_elements
            {
                $$ = $1;
            }
        |
            {
                $$ = NULL;
            }
    ;

    enum_elements
        : enum_elements COMMA enum_element
            {
                astNodeLLAppend($1->data.nodeList.list, $3);
                $$ = $1;
            }
        | enum_element
            {
                $$ = newNodeList($1);
            }
    ;

/* function definition */
    fn_parameter
        : nameNode nameNode
            {
                $$ = newParamInfo($2->data.stringValue, $1);
            }
        | var_type nameNode
            {
                $$ = newParamInfo($2->data.stringValue, $1);
            }
    ;

    fn_opt_params_list
        : fn_params_list
            {
                $$ = $1;
            }
        |
            {
                $$ = NULL;
            }
    ;

    fn_params_list
        : fn_params_list COMMA fn_parameter
            {
                astNodeLLAppend($1->data.nodeList.list, $3);
                $$ = $1;
            }
        | fn_parameter
            {
                $$ = newNodeList($1);
            }
    ;

    fn_def
        : FN nameNode LPAREN fn_opt_params_list RPAREN code_block
            {
                $$ = newFnDef($2->data.stringValue, $4, $6);
            }
    ;
        
/* variable declaration */
    var_decl
        : VAR var_decl_assign_list
            {
                $$ = newVarDeclBatch($2);
            }
    ;

    var_decl_assign_list
        : var_decl_assign_list COMMA var_decl_assign
            {
                astNodeLLAppend($1->data.nodeList.list, $3);
                $$ = $1;
            }
        | var_decl_assign
            {
                $$ = newNodeList($1);
            }
    ;

    var_decl_assign
        : var_decl_modifiers nameNode EQUALS expr
            {
                $$ = newVarDecl($2->data.stringValue, $1, $4);
            }
        | var_decl_modifiers nameNode
            {
                $$ = newVarDecl($2->data.stringValue, $1, NULL);
            }
    ;

    var_decl_modifiers
        : var_decl_modifiers var_decl_modifier
            {
                $$ = (Modifier)((int)$1 | (int)$2);
            }
        |
            {
                $$ = MODIFIER_NONE;
            }
    ;

    var_decl_modifier
        : CONST
            {
                $$ = $1;
            }
    ;

    var_type
        : VAR
            {
                $$ = newName("var");
            }
    ;

/* assign statement */
    assign
        : expr op_assign assign_rhs
            {
                $$ = newVarAssign($2, $1, $3);
            }
    ;

    assign_rhs
        : expr
            {
                $$ = $1;
            }
        | lambda_expr
            {
                $$ = $1;
            }
    ;

/* lambda expression */
    lambda_expr
        : HASH LSQUARE lambda_opt_capture_list RSQUARE LPAREN fn_opt_params_list RPAREN code_block
            {
                $$ = newLambda($3, $6, $8);
            }
    ;

    lambda_opt_capture_list
        : lambda_capture_list
            {
                $$ = $1;
            }
        |
            {
                $$ = NULL;
            }
    ;

    lambda_capture_list
        : lambda_capture_list COMMA nameNode
            {
                astNodeLLAppend($1->data.nodeList.list, $3);
                $$ = $1;
            }
        | nameNode
            {
                $$ = newNodeList($1);
            }
    ;

/* class definition */
    class_def
        : CLASS nameNode opt_delimiter_list LBRACE opt_class_member_list opt_delimiter_list RBRACE
            {
                $$ = newClassDef($2->data.stringValue, $5);
            }
    ;

    opt_class_member_list
        : class_member_list
            {
                $$ = $1;
            }
        |
            {
                $$ = NULL;
            }


    class_member_list
        : class_member_list delimiter_list class_member
            {
                astNodeLLAppend($1->data.nodeList.list, $3);
                $$ = $1;
            }
        | class_member
            {
                $$ = newNodeList($1);
            }
    ;

    class_member
        : class_member_mod_list var_type nameNode
            {
                $$ = newClassFieldDef($3->data.stringValue, $1, $2);
            }
        | class_member_mod_list fn_def
            {
                $$ = newClassMethodDef($1, $2);
            }
    ;

    class_member_mod_list
        : class_member_mod_list class_member_mod
            {
                $$ = (Modifier)((int)$1 | (int)$2);
            }
        |
            {
                $$ = MODIFIER_NONE;
            }
    ;

    class_member_mod
        : HIDDEN
            {
                $$ = $1;
            }
        | CONST
            {
                $$ = $1;
            }
        | STATIC
            {
                $$ = $1;
            }
    ;

%%