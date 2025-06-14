/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_BNF_TAB_H_INCLUDED
# define YY_YY_BNF_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    STRING = 258,                  /* STRING  */
    NUMBER = 259,                  /* NUMBER  */
    TRUE = 260,                    /* TRUE  */
    FALSE = 261,                   /* FALSE  */
    NAME = 262,                    /* NAME  */
    MINUS = 263,                   /* MINUS  */
    PLUS = 264,                    /* PLUS  */
    BANG = 265,                    /* BANG  */
    STAR = 266,                    /* STAR  */
    SLASH = 267,                   /* SLASH  */
    MOD = 268,                     /* MOD  */
    UNARY_DEC = 269,               /* UNARY_DEC  */
    UNARY_INC = 270,               /* UNARY_INC  */
    EQUALS = 271,                  /* EQUALS  */
    ASSIGN_ADD = 272,              /* ASSIGN_ADD  */
    ASSIGN_SUB = 273,              /* ASSIGN_SUB  */
    ASSIGN_DIV = 274,              /* ASSIGN_DIV  */
    ASSIGN_MUL = 275,              /* ASSIGN_MUL  */
    ASSIGN_MOD = 276,              /* ASSIGN_MOD  */
    ASSIGN_XOR = 277,              /* ASSIGN_XOR  */
    ASSIGN_SHIFTL = 278,           /* ASSIGN_SHIFTL  */
    ASSIGN_SHIFTR = 279,           /* ASSIGN_SHIFTR  */
    ASSIGN_AND = 280,              /* ASSIGN_AND  */
    ASSIGN_OR = 281,               /* ASSIGN_OR  */
    BIT_SHIFTL = 282,              /* BIT_SHIFTL  */
    BIT_SHIFTR = 283,              /* BIT_SHIFTR  */
    BIT_AND = 284,                 /* BIT_AND  */
    BIT_OR = 285,                  /* BIT_OR  */
    BIT_XOR = 286,                 /* BIT_XOR  */
    COND_GT = 287,                 /* COND_GT  */
    COND_GEQ = 288,                /* COND_GEQ  */
    COND_LT = 289,                 /* COND_LT  */
    COND_LEQ = 290,                /* COND_LEQ  */
    COND_EQ = 291,                 /* COND_EQ  */
    COND_NEQ = 292,                /* COND_NEQ  */
    LOGIC_AND = 293,               /* LOGIC_AND  */
    LOGIC_OR = 294,                /* LOGIC_OR  */
    DOT = 295,                     /* DOT  */
    LPAREN = 296,                  /* LPAREN  */
    RPAREN = 297,                  /* RPAREN  */
    LANGLE = 298,                  /* LANGLE  */
    RANGLE = 299,                  /* RANGLE  */
    LBRACE = 300,                  /* LBRACE  */
    RBRACE = 301,                  /* RBRACE  */
    LSQUARE = 302,                 /* LSQUARE  */
    RSQUARE = 303,                 /* RSQUARE  */
    COLON = 304,                   /* COLON  */
    COMMA = 305,                   /* COMMA  */
    HASH = 306,                    /* HASH  */
    DELIMITER = 307,               /* DELIMITER  */
    ENDOFTOKENS = 308,             /* ENDOFTOKENS  */
    FN = 309,                      /* FN  */
    RET = 310,                     /* RET  */
    IF = 311,                      /* IF  */
    ELSE = 312,                    /* ELSE  */
    WHL = 313,                     /* WHL  */
    FOR = 314,                     /* FOR  */
    SWITCH = 315,                  /* SWITCH  */
    BRK = 316,                     /* BRK  */
    DEFAULT = 317,                 /* DEFAULT  */
    CLASS = 318,                   /* CLASS  */
    TOSS = 319,                    /* TOSS  */
    DEL = 320,                     /* DEL  */
    ENUM = 321,                    /* ENUM  */
    CONST = 322,                   /* CONST  */
    VAR = 323,                     /* VAR  */
    STATIC = 324,                  /* STATIC  */
    HIDDEN = 325,                  /* HIDDEN  */
    LOWER_THAN_ELSE = 326,         /* LOWER_THAN_ELSE  */
    UMINUS = 327                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "bnf.y"

    const char* stringValue;   /* STRING, nameNode etc */
    double      numberValue;   /* NUMBER */
    bool        boolValue;     /* TRUE, FALSE */
    ASTNode*    node;          /* nonterminals */
    int         opType;        /* operators */
    Modifier    modifier;      /* modifiers like CONST */

#line 145 "bnf.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_BNF_TAB_H_INCLUDED  */
