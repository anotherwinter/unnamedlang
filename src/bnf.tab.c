/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "bnf.y"

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


#line 88 "bnf.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "bnf.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_STRING = 3,                     /* STRING  */
  YYSYMBOL_NUMBER = 4,                     /* NUMBER  */
  YYSYMBOL_TRUE = 5,                       /* TRUE  */
  YYSYMBOL_FALSE = 6,                      /* FALSE  */
  YYSYMBOL_NAME = 7,                       /* NAME  */
  YYSYMBOL_MINUS = 8,                      /* MINUS  */
  YYSYMBOL_PLUS = 9,                       /* PLUS  */
  YYSYMBOL_BANG = 10,                      /* BANG  */
  YYSYMBOL_STAR = 11,                      /* STAR  */
  YYSYMBOL_SLASH = 12,                     /* SLASH  */
  YYSYMBOL_MOD = 13,                       /* MOD  */
  YYSYMBOL_UNARY_DEC = 14,                 /* UNARY_DEC  */
  YYSYMBOL_UNARY_INC = 15,                 /* UNARY_INC  */
  YYSYMBOL_EQUALS = 16,                    /* EQUALS  */
  YYSYMBOL_ASSIGN_ADD = 17,                /* ASSIGN_ADD  */
  YYSYMBOL_ASSIGN_SUB = 18,                /* ASSIGN_SUB  */
  YYSYMBOL_ASSIGN_DIV = 19,                /* ASSIGN_DIV  */
  YYSYMBOL_ASSIGN_MUL = 20,                /* ASSIGN_MUL  */
  YYSYMBOL_ASSIGN_MOD = 21,                /* ASSIGN_MOD  */
  YYSYMBOL_ASSIGN_XOR = 22,                /* ASSIGN_XOR  */
  YYSYMBOL_ASSIGN_SHIFTL = 23,             /* ASSIGN_SHIFTL  */
  YYSYMBOL_ASSIGN_SHIFTR = 24,             /* ASSIGN_SHIFTR  */
  YYSYMBOL_ASSIGN_AND = 25,                /* ASSIGN_AND  */
  YYSYMBOL_ASSIGN_OR = 26,                 /* ASSIGN_OR  */
  YYSYMBOL_BIT_SHIFTL = 27,                /* BIT_SHIFTL  */
  YYSYMBOL_BIT_SHIFTR = 28,                /* BIT_SHIFTR  */
  YYSYMBOL_BIT_AND = 29,                   /* BIT_AND  */
  YYSYMBOL_BIT_OR = 30,                    /* BIT_OR  */
  YYSYMBOL_BIT_XOR = 31,                   /* BIT_XOR  */
  YYSYMBOL_COND_GT = 32,                   /* COND_GT  */
  YYSYMBOL_COND_GEQ = 33,                  /* COND_GEQ  */
  YYSYMBOL_COND_LT = 34,                   /* COND_LT  */
  YYSYMBOL_COND_LEQ = 35,                  /* COND_LEQ  */
  YYSYMBOL_COND_EQ = 36,                   /* COND_EQ  */
  YYSYMBOL_COND_NEQ = 37,                  /* COND_NEQ  */
  YYSYMBOL_LOGIC_AND = 38,                 /* LOGIC_AND  */
  YYSYMBOL_LOGIC_OR = 39,                  /* LOGIC_OR  */
  YYSYMBOL_DOT = 40,                       /* DOT  */
  YYSYMBOL_LPAREN = 41,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 42,                    /* RPAREN  */
  YYSYMBOL_LANGLE = 43,                    /* LANGLE  */
  YYSYMBOL_RANGLE = 44,                    /* RANGLE  */
  YYSYMBOL_LBRACE = 45,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 46,                    /* RBRACE  */
  YYSYMBOL_LSQUARE = 47,                   /* LSQUARE  */
  YYSYMBOL_RSQUARE = 48,                   /* RSQUARE  */
  YYSYMBOL_COLON = 49,                     /* COLON  */
  YYSYMBOL_COMMA = 50,                     /* COMMA  */
  YYSYMBOL_HASH = 51,                      /* HASH  */
  YYSYMBOL_DELIMITER = 52,                 /* DELIMITER  */
  YYSYMBOL_ENDOFTOKENS = 53,               /* ENDOFTOKENS  */
  YYSYMBOL_FN = 54,                        /* FN  */
  YYSYMBOL_RET = 55,                       /* RET  */
  YYSYMBOL_IF = 56,                        /* IF  */
  YYSYMBOL_ELSE = 57,                      /* ELSE  */
  YYSYMBOL_WHL = 58,                       /* WHL  */
  YYSYMBOL_FOR = 59,                       /* FOR  */
  YYSYMBOL_SWITCH = 60,                    /* SWITCH  */
  YYSYMBOL_BRK = 61,                       /* BRK  */
  YYSYMBOL_DEFAULT = 62,                   /* DEFAULT  */
  YYSYMBOL_CLASS = 63,                     /* CLASS  */
  YYSYMBOL_TOSS = 64,                      /* TOSS  */
  YYSYMBOL_DEL = 65,                       /* DEL  */
  YYSYMBOL_ENUM = 66,                      /* ENUM  */
  YYSYMBOL_CONST = 67,                     /* CONST  */
  YYSYMBOL_VAR = 68,                       /* VAR  */
  YYSYMBOL_STATIC = 69,                    /* STATIC  */
  YYSYMBOL_HIDDEN = 70,                    /* HIDDEN  */
  YYSYMBOL_LOWER_THAN_ELSE = 71,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_UMINUS = 72,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 73,                  /* $accept  */
  YYSYMBOL_program = 74,                   /* program  */
  YYSYMBOL_opt_delimiter_list = 75,        /* opt_delimiter_list  */
  YYSYMBOL_delimiter_list = 76,            /* delimiter_list  */
  YYSYMBOL_opt_program_stmt_list = 77,     /* opt_program_stmt_list  */
  YYSYMBOL_program_stmt_list = 78,         /* program_stmt_list  */
  YYSYMBOL_program_stmt = 79,              /* program_stmt  */
  YYSYMBOL_global_stmt = 80,               /* global_stmt  */
  YYSYMBOL_global_stmt_stmt = 81,          /* global_stmt_stmt  */
  YYSYMBOL_stmt = 82,                      /* stmt  */
  YYSYMBOL_stmt_stmt = 83,                 /* stmt_stmt  */
  YYSYMBOL_code_block = 84,                /* code_block  */
  YYSYMBOL_opt_stmt_list = 85,             /* opt_stmt_list  */
  YYSYMBOL_stmt_list = 86,                 /* stmt_list  */
  YYSYMBOL_code_block_or_stmt = 87,        /* code_block_or_stmt  */
  YYSYMBOL_del_stmt = 88,                  /* del_stmt  */
  YYSYMBOL_cntrl_cnstrct = 89,             /* cntrl_cnstrct  */
  YYSYMBOL_loop_whl = 90,                  /* loop_whl  */
  YYSYMBOL_loop_for = 91,                  /* loop_for  */
  YYSYMBOL_loop_for_init = 92,             /* loop_for_init  */
  YYSYMBOL_loop_for_cond = 93,             /* loop_for_cond  */
  YYSYMBOL_loop_for_post_iter = 94,        /* loop_for_post_iter  */
  YYSYMBOL_ret_stmt = 95,                  /* ret_stmt  */
  YYSYMBOL_switch_stmt = 96,               /* switch_stmt  */
  YYSYMBOL_opt_switch_case_list = 97,      /* opt_switch_case_list  */
  YYSYMBOL_switch_case_list = 98,          /* switch_case_list  */
  YYSYMBOL_switch_case = 99,               /* switch_case  */
  YYSYMBOL_brk_stmt = 100,                 /* brk_stmt  */
  YYSYMBOL_if_stmt = 101,                  /* if_stmt  */
  YYSYMBOL_opt_else = 102,                 /* opt_else  */
  YYSYMBOL_toss_stmt = 103,                /* toss_stmt  */
  YYSYMBOL_op_assign = 104,                /* op_assign  */
  YYSYMBOL_op_logic = 105,                 /* op_logic  */
  YYSYMBOL_op_cond = 106,                  /* op_cond  */
  YYSYMBOL_op_bitwise_xor = 107,           /* op_bitwise_xor  */
  YYSYMBOL_op_bitwise_or = 108,            /* op_bitwise_or  */
  YYSYMBOL_op_bitwise_and = 109,           /* op_bitwise_and  */
  YYSYMBOL_op_bitwise_shift = 110,         /* op_bitwise_shift  */
  YYSYMBOL_op_arithm_add = 111,            /* op_arithm_add  */
  YYSYMBOL_op_arithm_mul = 112,            /* op_arithm_mul  */
  YYSYMBOL_op_unary_neg = 113,             /* op_unary_neg  */
  YYSYMBOL_op_unary_logic_neg = 114,       /* op_unary_logic_neg  */
  YYSYMBOL_op_unary_inc = 115,             /* op_unary_inc  */
  YYSYMBOL_expr = 116,                     /* expr  */
  YYSYMBOL_lvl_logic = 117,                /* lvl_logic  */
  YYSYMBOL_lvl_cond = 118,                 /* lvl_cond  */
  YYSYMBOL_lvl_bitwise_xor = 119,          /* lvl_bitwise_xor  */
  YYSYMBOL_lvl_bitwise_or = 120,           /* lvl_bitwise_or  */
  YYSYMBOL_lvl_bitwise_and = 121,          /* lvl_bitwise_and  */
  YYSYMBOL_lvl_bitwise_shift = 122,        /* lvl_bitwise_shift  */
  YYSYMBOL_lvl_arithm_add = 123,           /* lvl_arithm_add  */
  YYSYMBOL_lvl_arithm_mul = 124,           /* lvl_arithm_mul  */
  YYSYMBOL_lvl_unary_neg = 125,            /* lvl_unary_neg  */
  YYSYMBOL_lvl_unary_inc = 126,            /* lvl_unary_inc  */
  YYSYMBOL_lvl_postfix = 127,              /* lvl_postfix  */
  YYSYMBOL_primary = 128,                  /* primary  */
  YYSYMBOL_opt_expr = 129,                 /* opt_expr  */
  YYSYMBOL_opt_expr_list = 130,            /* opt_expr_list  */
  YYSYMBOL_expr_list = 131,                /* expr_list  */
  YYSYMBOL_literal = 132,                  /* literal  */
  YYSYMBOL_arr_initializer = 133,          /* arr_initializer  */
  YYSYMBOL_nameNode = 134,                 /* nameNode  */
  YYSYMBOL_enum_stmt = 135,                /* enum_stmt  */
  YYSYMBOL_enum_element = 136,             /* enum_element  */
  YYSYMBOL_enum_opt_elements = 137,        /* enum_opt_elements  */
  YYSYMBOL_enum_elements = 138,            /* enum_elements  */
  YYSYMBOL_fn_parameter = 139,             /* fn_parameter  */
  YYSYMBOL_fn_opt_params_list = 140,       /* fn_opt_params_list  */
  YYSYMBOL_fn_params_list = 141,           /* fn_params_list  */
  YYSYMBOL_fn_def = 142,                   /* fn_def  */
  YYSYMBOL_var_decl = 143,                 /* var_decl  */
  YYSYMBOL_var_decl_assign_list = 144,     /* var_decl_assign_list  */
  YYSYMBOL_var_decl_assign = 145,          /* var_decl_assign  */
  YYSYMBOL_var_decl_modifiers = 146,       /* var_decl_modifiers  */
  YYSYMBOL_var_decl_modifier = 147,        /* var_decl_modifier  */
  YYSYMBOL_var_type = 148,                 /* var_type  */
  YYSYMBOL_assign = 149,                   /* assign  */
  YYSYMBOL_assign_rhs = 150,               /* assign_rhs  */
  YYSYMBOL_lambda_expr = 151,              /* lambda_expr  */
  YYSYMBOL_lambda_opt_capture_list = 152,  /* lambda_opt_capture_list  */
  YYSYMBOL_lambda_capture_list = 153,      /* lambda_capture_list  */
  YYSYMBOL_class_def = 154,                /* class_def  */
  YYSYMBOL_opt_class_member_list = 155,    /* opt_class_member_list  */
  YYSYMBOL_class_member_list = 156,        /* class_member_list  */
  YYSYMBOL_class_member = 157,             /* class_member  */
  YYSYMBOL_class_member_mod_list = 158,    /* class_member_mod_list  */
  YYSYMBOL_class_member_mod = 159          /* class_member_mod  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  86
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   524

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  87
/* YYNRULES -- Number of rules.  */
#define YYNRULES  176
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  267

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   327


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   212,   212,   219,   220,   224,   225,   229,   233,   240,
     245,   252,   256,   264,   271,   275,   279,   287,   294,   298,
     302,   306,   310,   318,   325,   330,   336,   341,   348,   352,
     360,   368,   372,   376,   380,   384,   388,   392,   399,   406,
     413,   420,   425,   432,   439,   443,   450,   457,   462,   468,
     473,   480,   484,   491,   495,   502,   509,   513,   520,   528,
     532,   536,   540,   544,   548,   552,   556,   560,   564,   568,
     575,   579,   586,   590,   594,   598,   602,   606,   613,   620,
     627,   634,   638,   645,   649,   656,   660,   667,   674,   681,
     685,   693,   711,   715,   722,   726,   733,   737,   744,   748,
     755,   759,   766,   770,   777,   781,   788,   792,   799,   803,
     807,   814,   818,   825,   829,   833,   837,   844,   848,   852,
     859,   864,   870,   875,   881,   886,   893,   897,   901,   905,
     909,   917,   921,   929,   937,   944,   948,   955,   960,   966,
     971,   979,   983,   990,   995,  1001,  1006,  1013,  1021,  1028,
    1033,  1040,  1044,  1051,  1056,  1062,  1069,  1077,  1084,  1088,
    1096,  1103,  1108,  1114,  1119,  1127,  1134,  1139,  1145,  1150,
    1157,  1161,  1168,  1173,  1179,  1183,  1187
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "STRING", "NUMBER",
  "TRUE", "FALSE", "NAME", "MINUS", "PLUS", "BANG", "STAR", "SLASH", "MOD",
  "UNARY_DEC", "UNARY_INC", "EQUALS", "ASSIGN_ADD", "ASSIGN_SUB",
  "ASSIGN_DIV", "ASSIGN_MUL", "ASSIGN_MOD", "ASSIGN_XOR", "ASSIGN_SHIFTL",
  "ASSIGN_SHIFTR", "ASSIGN_AND", "ASSIGN_OR", "BIT_SHIFTL", "BIT_SHIFTR",
  "BIT_AND", "BIT_OR", "BIT_XOR", "COND_GT", "COND_GEQ", "COND_LT",
  "COND_LEQ", "COND_EQ", "COND_NEQ", "LOGIC_AND", "LOGIC_OR", "DOT",
  "LPAREN", "RPAREN", "LANGLE", "RANGLE", "LBRACE", "RBRACE", "LSQUARE",
  "RSQUARE", "COLON", "COMMA", "HASH", "DELIMITER", "ENDOFTOKENS", "FN",
  "RET", "IF", "ELSE", "WHL", "FOR", "SWITCH", "BRK", "DEFAULT", "CLASS",
  "TOSS", "DEL", "ENUM", "CONST", "VAR", "STATIC", "HIDDEN",
  "LOWER_THAN_ELSE", "UMINUS", "$accept", "program", "opt_delimiter_list",
  "delimiter_list", "opt_program_stmt_list", "program_stmt_list",
  "program_stmt", "global_stmt", "global_stmt_stmt", "stmt", "stmt_stmt",
  "code_block", "opt_stmt_list", "stmt_list", "code_block_or_stmt",
  "del_stmt", "cntrl_cnstrct", "loop_whl", "loop_for", "loop_for_init",
  "loop_for_cond", "loop_for_post_iter", "ret_stmt", "switch_stmt",
  "opt_switch_case_list", "switch_case_list", "switch_case", "brk_stmt",
  "if_stmt", "opt_else", "toss_stmt", "op_assign", "op_logic", "op_cond",
  "op_bitwise_xor", "op_bitwise_or", "op_bitwise_and", "op_bitwise_shift",
  "op_arithm_add", "op_arithm_mul", "op_unary_neg", "op_unary_logic_neg",
  "op_unary_inc", "expr", "lvl_logic", "lvl_cond", "lvl_bitwise_xor",
  "lvl_bitwise_or", "lvl_bitwise_and", "lvl_bitwise_shift",
  "lvl_arithm_add", "lvl_arithm_mul", "lvl_unary_neg", "lvl_unary_inc",
  "lvl_postfix", "primary", "opt_expr", "opt_expr_list", "expr_list",
  "literal", "arr_initializer", "nameNode", "enum_stmt", "enum_element",
  "enum_opt_elements", "enum_elements", "fn_parameter",
  "fn_opt_params_list", "fn_params_list", "fn_def", "var_decl",
  "var_decl_assign_list", "var_decl_assign", "var_decl_modifiers",
  "var_decl_modifier", "var_type", "assign", "assign_rhs", "lambda_expr",
  "lambda_opt_capture_list", "lambda_capture_list", "class_def",
  "opt_class_member_list", "class_member_list", "class_member",
  "class_member_mod_list", "class_member_mod", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-225)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-168)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     226,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
     477,   431,  -225,    20,   477,   -34,   -17,     1,    41,    47,
      20,   477,   477,    20,  -225,    48,  -225,    16,  -225,    40,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,    11,    11,   122,   478,     7,    27,
      19,    69,    72,    44,    66,    91,  -225,  -225,   -19,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,    68,  -225,
    -225,    17,    70,  -225,   477,   477,   477,   477,   477,    40,
    -225,  -225,    67,    65,  -225,     2,  -225,  -225,  -225,   255,
    -225,  -225,   -19,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,   416,  -225,  -225,   477,  -225,  -225,
    -225,  -225,  -225,  -225,   477,  -225,   477,  -225,   477,  -225,
     477,  -225,  -225,   477,  -225,  -225,   477,  -225,  -225,   477,
      20,   477,   477,  -225,  -225,   477,    -2,    74,    75,    71,
    -225,    73,    79,    80,    85,    40,  -225,  -225,    98,  -225,
    -225,    86,  -225,  -225,  -225,    27,    19,    69,    72,    44,
      66,    91,  -225,  -225,    92,  -225,    84,  -225,  -225,    20,
    -225,    94,    88,    20,    40,    40,   477,    97,  -225,    -5,
       5,  -225,    93,   102,  -225,   477,    20,  -225,  -225,  -225,
      40,    -2,  -225,    90,  -225,    96,  -225,    57,     7,   403,
      40,    40,  -225,   -14,   127,  -225,    40,  -225,  -225,    99,
     106,   114,  -225,  -225,    40,  -225,    40,  -225,   477,   477,
     111,    40,    40,  -225,   113,   118,    16,  -225,  -225,  -225,
    -225,    20,  -225,   477,  -225,   124,    20,   337,  -225,   125,
    -225,     7,    40,   120,    29,    40,  -225,  -225,  -225,  -225,
      -2,  -225,  -225,   126,    40,    40,  -225,  -225,  -225,  -225,
     128,  -225,   321,  -225,    40,  -225,  -225
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,   130,   128,   126,   127,   133,    87,    88,    89,    90,
       0,     0,     6,     0,    44,     0,     0,     0,     0,    54,
       0,     0,     0,     0,   154,     0,     8,     3,     2,     4,
      10,    12,    13,    11,    17,    20,    19,    31,    32,    34,
      33,    35,    36,    37,     0,     0,     0,    22,    91,    93,
      95,    97,    99,   101,   103,   105,   107,   110,   112,   116,
     117,   129,   118,    16,    15,    18,    21,    14,     0,   131,
     125,     0,     0,    45,     0,     0,   123,     0,     0,     4,
      58,    30,     0,   148,   150,     0,     1,     5,     7,     3,
     108,   109,   111,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,     0,    70,    71,     0,    72,    73,
      74,    75,    76,    77,     0,    78,     0,    79,     0,    80,
       0,    81,    82,     0,    84,    83,     0,    85,    86,     0,
       0,   123,   121,   119,   132,     0,   144,     0,     0,     0,
      40,   122,     0,     0,     0,   138,   154,   155,   152,   153,
       9,     0,   158,   157,   159,    92,    94,    96,    98,   100,
     102,   104,   106,   113,     0,   120,     0,   124,   156,     0,
     146,     0,   143,     0,     4,     4,     0,     0,    53,   173,
       0,   140,     0,   137,   149,     0,   162,   114,   115,   141,
       4,     0,   142,     0,    28,    57,    38,     0,    42,    48,
       4,   166,   169,     0,   135,   134,     0,   151,   164,     0,
     161,     0,   147,   145,     4,    29,     4,    55,   123,     0,
       0,     4,    47,    50,     0,     0,   173,   175,   176,   174,
     171,     0,   172,     0,   139,     0,     0,    25,    56,     0,
      43,    41,     4,     0,     0,     4,   165,   168,   170,   136,
     144,   163,    27,     0,    24,     4,    52,    46,    49,    51,
       0,    23,     0,    39,     4,    26,   160
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -225,  -225,     0,   -26,  -225,  -225,    82,  -225,  -225,   -20,
    -224,  -186,  -225,  -225,  -165,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,   -68,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,     9,  -170,    76,    63,    62,    77,    61,
      59,    58,    56,    64,   140,  -225,  -225,  -129,   176,  -225,
    -225,   -12,  -225,   -18,  -225,  -225,     6,   -61,  -225,   -11,
    -225,  -225,    45,  -225,  -225,   -10,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,   -28,  -225,  -225
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    25,   193,    27,    28,    29,    30,    31,    32,    33,
      34,   194,   253,   254,   195,    35,    36,    37,    38,   139,
     197,   239,    39,    40,   221,   222,   223,    41,    42,   217,
      43,   104,   107,   114,   116,   118,   120,   123,   126,   129,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,   166,   140,   141,    60,
      61,    62,    63,   181,   182,   183,   170,   171,   172,    64,
      65,    83,    84,    85,   149,   173,    66,   153,   154,   209,
     210,    67,   200,   201,   202,   203,   232
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      26,    72,   164,    89,   212,     5,   198,    74,    79,     5,
     196,    82,     5,   252,     1,     2,     3,     4,     5,    68,
      70,   130,   131,    73,    75,     8,     9,     5,   132,    88,
      80,    81,     1,     2,     3,     4,     5,     6,   265,     7,
      13,  -167,    76,     8,     9,   105,   106,  -167,    86,   241,
     115,   238,    10,   227,   168,   228,   229,    87,    11,   108,
     109,   110,   111,   112,   113,   134,   168,   135,    87,   147,
      10,   121,   122,   148,   124,   125,    11,   256,   266,   144,
     259,    87,    77,   137,   138,    70,   142,   143,    78,   240,
     263,   220,    12,     1,     2,     3,     4,     5,     6,   117,
       7,   119,   127,   128,     8,     9,   218,   219,    90,    91,
     133,   136,   145,   152,   185,   146,   174,   175,   163,   180,
     176,   177,   178,   135,   169,     1,     2,     3,     4,     5,
     179,    10,   188,   186,   187,   214,   190,    11,   191,   205,
      70,   165,   199,   233,   167,    14,    15,   235,    16,    17,
      18,    19,   206,   216,    21,    22,   236,   189,    24,   214,
     242,   192,   245,    10,   246,   250,   257,   255,   204,    11,
     264,   150,   261,   215,   208,   226,   258,   156,   157,   169,
     180,   159,   160,   155,   161,   162,    92,    71,   234,   260,
     211,   184,   230,   231,   207,   158,   244,   213,   247,     0,
     225,     0,     0,     0,     0,     0,     0,     0,   224,     0,
       0,     0,     0,     0,   237,     0,     0,     0,     0,   248,
       0,   243,     0,     0,   251,     0,     0,    70,   262,     1,
       2,     3,     4,     5,     6,     0,     7,     0,   169,     0,
       8,     9,   249,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   224,     0,     0,     0,     0,     1,     2,
       3,     4,     5,     6,   211,     7,     0,    10,     0,     8,
       9,     0,     0,    11,     0,     0,     0,     0,    12,     0,
      13,    14,    15,     0,    16,    17,    18,    19,     0,    20,
      21,    22,    23,     0,    24,     0,    10,     0,     0,     0,
       0,     0,    11,     0,     0,     0,     0,    87,     0,    13,
      14,    15,     0,    16,    17,    18,    19,     0,    20,    21,
      22,    23,     0,    24,     1,     2,     3,     4,     5,     6,
       0,     7,     0,     0,     0,     8,     9,     0,     0,     0,
       1,     2,     3,     4,     5,     6,     0,     7,     0,     0,
       0,     8,     9,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    10,     0,     0,     0,     0,     0,    11,     0,
       0,     0,     0,    87,     0,     0,    14,    15,    10,    16,
      17,    18,    19,     0,    11,    21,    22,     0,     0,    24,
       0,     0,    14,    15,     0,    16,    17,    18,    19,     0,
       0,    21,    22,     0,     0,    24,     1,     2,     3,     4,
       5,     6,     0,     7,     0,     0,     0,     8,     9,     1,
       2,     3,     4,     5,     6,     0,     7,     0,     0,     0,
       8,     9,     0,     0,     1,     2,     3,     4,     5,     6,
       0,     7,     0,     0,    10,     8,     9,     0,     0,     0,
      11,     0,     0,     0,     0,     0,     0,    10,     0,     0,
       0,     0,     0,    11,     0,   220,     0,   151,     0,     0,
       0,     0,    10,     0,     0,     0,     0,     0,    11,    69,
       1,     2,     3,     4,     5,     6,     0,     7,     0,     0,
       0,     8,     9,     0,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    10,     0,
       0,     0,     0,     0,    11
};

static const yytype_int16 yycheck[] =
{
       0,    13,   131,    29,   190,     7,   176,    41,    20,     7,
     175,    23,     7,   237,     3,     4,     5,     6,     7,    10,
      11,    40,    41,    14,    41,    14,    15,     7,    47,    29,
      21,    22,     3,     4,     5,     6,     7,     8,   262,    10,
      54,    46,    41,    14,    15,    38,    39,    52,     0,   219,
      31,   216,    41,    67,    68,    69,    70,    52,    47,    32,
      33,    34,    35,    36,    37,    48,    68,    50,    52,    67,
      41,    27,    28,    85,     8,     9,    47,   242,   264,    79,
     245,    52,    41,    74,    75,    76,    77,    78,    41,   218,
     255,    62,    52,     3,     4,     5,     6,     7,     8,    30,
      10,    29,    11,    12,    14,    15,    49,    50,    44,    45,
      42,    41,    45,   104,    16,    50,    42,    42,   130,   145,
      49,    42,    42,    50,   136,     3,     4,     5,     6,     7,
      45,    41,    48,    47,    42,    45,    42,    47,    50,    46,
     131,   132,    45,    16,   135,    55,    56,    48,    58,    59,
      60,    61,    50,    57,    64,    65,    50,   169,    68,    45,
      49,   173,    49,    41,    46,    41,    46,    42,   180,    47,
      42,    89,    46,   193,   186,   201,   244,   114,   116,   191,
     206,   120,   123,   107,   126,   129,    46,    11,   206,   250,
     190,   146,   203,   203,   185,   118,   222,   191,   226,    -1,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   199,    -1,
      -1,    -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,   231,
      -1,   221,    -1,    -1,   236,    -1,    -1,   218,   254,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,   250,    -1,
      14,    15,   233,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   244,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,     8,   264,    10,    -1,    41,    -1,    14,
      15,    -1,    -1,    47,    -1,    -1,    -1,    -1,    52,    -1,
      54,    55,    56,    -1,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    -1,    68,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    -1,    -1,    -1,    52,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    -1,    63,    64,
      65,    66,    -1,    68,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    -1,    -1,    14,    15,    -1,    -1,    -1,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    -1,
      -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    55,    56,    41,    58,
      59,    60,    61,    -1,    47,    64,    65,    -1,    -1,    68,
      -1,    -1,    55,    56,    -1,    58,    59,    60,    61,    -1,
      -1,    64,    65,    -1,    -1,    68,     3,     4,     5,     6,
       7,     8,    -1,    10,    -1,    -1,    -1,    14,    15,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    -1,    -1,
      14,    15,    -1,    -1,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    -1,    41,    14,    15,    -1,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    47,    -1,    62,    -1,    51,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,    48,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    -1,
      -1,    14,    15,    -1,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    47
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    10,    14,    15,
      41,    47,    52,    54,    55,    56,    58,    59,    60,    61,
      63,    64,    65,    66,    68,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    88,    89,    90,    91,    95,
      96,   100,   101,   103,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     132,   133,   134,   135,   142,   143,   149,   154,   116,    48,
     116,   131,   134,   116,    41,    41,    41,    41,    41,   134,
     116,   116,   134,   144,   145,   146,     0,    52,    75,    76,
     126,   126,   127,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   104,    38,    39,   105,    32,    33,
      34,    35,    36,    37,   106,    31,   107,    30,   108,    29,
     109,    27,    28,   110,     8,     9,   111,    11,    12,   112,
      40,    41,    47,    42,    48,    50,    41,   116,   116,    92,
     130,   131,   116,   116,    75,    45,    50,    67,   134,   147,
      79,    51,   116,   150,   151,   118,   119,   120,   121,   122,
     123,   124,   125,   134,   130,   116,   129,   116,    68,   134,
     139,   140,   141,   148,    42,    42,    49,    42,    42,    45,
      76,   136,   137,   138,   145,    16,    47,    42,    48,   134,
      42,    50,   134,    75,    84,    87,    87,    93,   117,    45,
     155,   156,   157,   158,   134,    46,    50,   116,   134,   152,
     153,    75,    84,   139,    45,    82,    57,   102,    49,    50,
      62,    97,    98,    99,   116,    75,    76,    67,    69,    70,
     142,   148,   159,    16,   136,    48,    50,    75,    87,    94,
     130,   117,    49,    75,    76,    49,    46,   157,   134,   116,
      41,   134,    83,    85,    86,    42,    87,    46,    99,    87,
     140,    46,    76,    87,    42,    83,    84
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    73,    74,    75,    75,    76,    76,    77,    77,    78,
      78,    79,    79,    80,    81,    81,    81,    82,    83,    83,
      83,    83,    83,    84,    85,    85,    86,    86,    87,    87,
      88,    89,    89,    89,    89,    89,    89,    89,    90,    91,
      92,    93,    93,    94,    95,    95,    96,    97,    97,    98,
      98,    99,    99,   100,   100,   101,   102,   102,   103,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     105,   105,   106,   106,   106,   106,   106,   106,   107,   108,
     109,   110,   110,   111,   111,   112,   112,   113,   114,   115,
     115,   116,   117,   117,   118,   118,   119,   119,   120,   120,
     121,   121,   122,   122,   123,   123,   124,   124,   125,   125,
     125,   126,   126,   127,   127,   127,   127,   128,   128,   128,
     129,   129,   130,   130,   131,   131,   132,   132,   132,   132,
     132,   133,   133,   134,   135,   136,   136,   137,   137,   138,
     138,   139,   139,   140,   140,   141,   141,   142,   143,   144,
     144,   145,   145,   146,   146,   147,   148,   149,   150,   150,
     151,   152,   152,   153,   153,   154,   155,   155,   156,   156,
     157,   157,   158,   158,   159,   159,   159
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     0,     2,     1,     2,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     1,     0,     3,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     5,     9,
       1,     3,     1,     1,     1,     2,     8,     1,     0,     3,
       1,     3,     3,     4,     1,     6,     2,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     2,     2,
       1,     2,     1,     3,     4,     4,     1,     1,     1,     3,
       1,     0,     1,     0,     3,     1,     1,     1,     1,     1,
       1,     2,     3,     1,     5,     2,     4,     1,     0,     3,
       1,     2,     2,     1,     0,     3,     1,     6,     2,     3,
       1,     4,     2,     2,     0,     1,     1,     3,     1,     1,
       8,     1,     0,     3,     1,     7,     1,     0,     3,     1,
       3,     2,     2,     0,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: opt_program_stmt_list  */
#line 213 "bnf.y"
            {
                astRoot = (yyvsp[0].node);
            }
#line 1509 "bnf.tab.c"
    break;

  case 7: /* opt_program_stmt_list: program_stmt_list opt_delimiter_list  */
#line 230 "bnf.y"
            {
                (yyval.node) = (yyvsp[-1].node);
            }
#line 1517 "bnf.tab.c"
    break;

  case 8: /* opt_program_stmt_list: opt_delimiter_list  */
#line 234 "bnf.y"
            {
                (yyval.node) = NULL;
            }
#line 1525 "bnf.tab.c"
    break;

  case 9: /* program_stmt_list: program_stmt_list delimiter_list program_stmt  */
#line 241 "bnf.y"
            {
                astNodeLLAppend((yyvsp[-2].node)->data.nodeList.list, (yyvsp[0].node));
                (yyval.node) = (yyvsp[-2].node);
            }
#line 1534 "bnf.tab.c"
    break;

  case 10: /* program_stmt_list: program_stmt  */
#line 246 "bnf.y"
            {
                (yyval.node) = newNodeList((yyvsp[0].node));
            }
#line 1542 "bnf.tab.c"
    break;

  case 11: /* program_stmt: stmt  */
#line 253 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1550 "bnf.tab.c"
    break;

  case 12: /* program_stmt: global_stmt  */
#line 257 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1558 "bnf.tab.c"
    break;

  case 13: /* global_stmt: global_stmt_stmt  */
#line 265 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1566 "bnf.tab.c"
    break;

  case 14: /* global_stmt_stmt: class_def  */
#line 272 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1574 "bnf.tab.c"
    break;

  case 15: /* global_stmt_stmt: fn_def  */
#line 276 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1582 "bnf.tab.c"
    break;

  case 16: /* global_stmt_stmt: enum_stmt  */
#line 280 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1590 "bnf.tab.c"
    break;

  case 17: /* stmt: stmt_stmt  */
#line 288 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1598 "bnf.tab.c"
    break;

  case 18: /* stmt_stmt: var_decl  */
#line 295 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1606 "bnf.tab.c"
    break;

  case 19: /* stmt_stmt: cntrl_cnstrct  */
#line 299 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1614 "bnf.tab.c"
    break;

  case 20: /* stmt_stmt: del_stmt  */
#line 303 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1622 "bnf.tab.c"
    break;

  case 21: /* stmt_stmt: assign  */
#line 307 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1630 "bnf.tab.c"
    break;

  case 22: /* stmt_stmt: expr  */
#line 311 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1638 "bnf.tab.c"
    break;

  case 23: /* code_block: opt_delimiter_list LBRACE opt_delimiter_list opt_stmt_list RBRACE  */
#line 319 "bnf.y"
            {
                (yyval.node) = (yyvsp[-1].node);
            }
#line 1646 "bnf.tab.c"
    break;

  case 24: /* opt_stmt_list: stmt_list  */
#line 326 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1654 "bnf.tab.c"
    break;

  case 25: /* opt_stmt_list: %empty  */
#line 330 "bnf.y"
            {
                (yyval.node) = NULL;
            }
#line 1662 "bnf.tab.c"
    break;

  case 26: /* stmt_list: stmt_list delimiter_list stmt_stmt  */
#line 337 "bnf.y"
            {
                astNodeLLAppend((yyvsp[-2].node)->data.nodeList.list, (yyvsp[0].node));
                (yyval.node) = (yyvsp[-2].node);
            }
#line 1671 "bnf.tab.c"
    break;

  case 27: /* stmt_list: stmt_stmt  */
#line 342 "bnf.y"
            {
                (yyval.node) = newNodeList((yyvsp[0].node));
            }
#line 1679 "bnf.tab.c"
    break;

  case 28: /* code_block_or_stmt: code_block  */
#line 349 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1687 "bnf.tab.c"
    break;

  case 29: /* code_block_or_stmt: opt_delimiter_list stmt  */
#line 353 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1695 "bnf.tab.c"
    break;

  case 30: /* del_stmt: DEL expr  */
#line 361 "bnf.y"
            {
                (yyval.node) = newDelStmt((yyvsp[0].node));
            }
#line 1703 "bnf.tab.c"
    break;

  case 31: /* cntrl_cnstrct: loop_whl  */
#line 369 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1711 "bnf.tab.c"
    break;

  case 32: /* cntrl_cnstrct: loop_for  */
#line 373 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1719 "bnf.tab.c"
    break;

  case 33: /* cntrl_cnstrct: switch_stmt  */
#line 377 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1727 "bnf.tab.c"
    break;

  case 34: /* cntrl_cnstrct: ret_stmt  */
#line 381 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1735 "bnf.tab.c"
    break;

  case 35: /* cntrl_cnstrct: brk_stmt  */
#line 385 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1743 "bnf.tab.c"
    break;

  case 36: /* cntrl_cnstrct: if_stmt  */
#line 389 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1751 "bnf.tab.c"
    break;

  case 37: /* cntrl_cnstrct: toss_stmt  */
#line 393 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1759 "bnf.tab.c"
    break;

  case 38: /* loop_whl: WHL LPAREN expr RPAREN code_block_or_stmt  */
#line 400 "bnf.y"
            {
                (yyval.node) = newLoopWhl((yyvsp[-2].node), (yyvsp[0].node));
            }
#line 1767 "bnf.tab.c"
    break;

  case 39: /* loop_for: FOR LPAREN loop_for_init COLON loop_for_cond COLON loop_for_post_iter RPAREN code_block_or_stmt  */
#line 407 "bnf.y"
            {
                (yyval.node) = newLoopFor((yyvsp[-6].node), (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node));
            }
#line 1775 "bnf.tab.c"
    break;

  case 40: /* loop_for_init: opt_expr_list  */
#line 414 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1783 "bnf.tab.c"
    break;

  case 41: /* loop_for_cond: loop_for_cond COMMA lvl_logic  */
#line 421 "bnf.y"
            {
                astNodeLLAppend((yyvsp[-2].node)->data.nodeList.list, (yyvsp[0].node));
                (yyval.node) = (yyvsp[-2].node);
            }
#line 1792 "bnf.tab.c"
    break;

  case 42: /* loop_for_cond: lvl_logic  */
#line 426 "bnf.y"
            {
                (yyval.node) = newNodeList((yyvsp[0].node));
            }
#line 1800 "bnf.tab.c"
    break;

  case 43: /* loop_for_post_iter: opt_expr_list  */
#line 433 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1808 "bnf.tab.c"
    break;

  case 44: /* ret_stmt: RET  */
#line 440 "bnf.y"
            {
                (yyval.node) = newRetStmt(NULL);
            }
#line 1816 "bnf.tab.c"
    break;

  case 45: /* ret_stmt: RET expr  */
#line 444 "bnf.y"
            {
                (yyval.node) = newRetStmt((yyvsp[0].node));
            }
#line 1824 "bnf.tab.c"
    break;

  case 46: /* switch_stmt: SWITCH LPAREN expr RPAREN LBRACE opt_switch_case_list opt_delimiter_list RBRACE  */
#line 451 "bnf.y"
            {
                (yyval.node) = newSwitchStmt((yyvsp[-5].node), (yyvsp[-2].node));
            }
#line 1832 "bnf.tab.c"
    break;

  case 47: /* opt_switch_case_list: switch_case_list  */
#line 458 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1840 "bnf.tab.c"
    break;

  case 48: /* opt_switch_case_list: %empty  */
#line 462 "bnf.y"
            {
                (yyval.node) = NULL;
            }
#line 1848 "bnf.tab.c"
    break;

  case 49: /* switch_case_list: switch_case_list delimiter_list switch_case  */
#line 469 "bnf.y"
            {
                astNodeLLAppend((yyvsp[-2].node)->data.nodeList.list, (yyvsp[0].node));
                (yyval.node) = (yyvsp[-2].node);
            }
#line 1857 "bnf.tab.c"
    break;

  case 50: /* switch_case_list: switch_case  */
#line 474 "bnf.y"
            {
                (yyval.node) = newNodeList((yyvsp[0].node));
            }
#line 1865 "bnf.tab.c"
    break;

  case 51: /* switch_case: expr COLON code_block_or_stmt  */
#line 481 "bnf.y"
            {
                (yyval.node) = newSwitchCase((yyvsp[-2].node), (yyvsp[0].node), false);
            }
#line 1873 "bnf.tab.c"
    break;

  case 52: /* switch_case: DEFAULT COLON code_block_or_stmt  */
#line 485 "bnf.y"
            {
                (yyval.node) = newSwitchCase(NULL, (yyvsp[0].node), true);
            }
#line 1881 "bnf.tab.c"
    break;

  case 53: /* brk_stmt: BRK LPAREN expr RPAREN  */
#line 492 "bnf.y"
            {
                (yyval.node) = newBrkStmt((yyvsp[-1].node));
            }
#line 1889 "bnf.tab.c"
    break;

  case 54: /* brk_stmt: BRK  */
#line 496 "bnf.y"
            {
                (yyval.node) = newBrkStmt(NULL);
            }
#line 1897 "bnf.tab.c"
    break;

  case 55: /* if_stmt: IF LPAREN expr RPAREN code_block_or_stmt opt_else  */
#line 503 "bnf.y"
            {
                (yyval.node) = newIfStmt((yyvsp[-3].node), (yyvsp[-1].node), (yyvsp[0].node));
            }
#line 1905 "bnf.tab.c"
    break;

  case 56: /* opt_else: ELSE code_block_or_stmt  */
#line 510 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 1913 "bnf.tab.c"
    break;

  case 57: /* opt_else: %empty  */
#line 514 "bnf.y"
            {
                (yyval.node) = NULL;
            }
#line 1921 "bnf.tab.c"
    break;

  case 58: /* toss_stmt: TOSS expr  */
#line 521 "bnf.y"
            {
                (yyval.node) = newTossStmt((yyvsp[0].node));
            }
#line 1929 "bnf.tab.c"
    break;

  case 59: /* op_assign: EQUALS  */
#line 529 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 1937 "bnf.tab.c"
    break;

  case 60: /* op_assign: ASSIGN_ADD  */
#line 533 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 1945 "bnf.tab.c"
    break;

  case 61: /* op_assign: ASSIGN_SUB  */
#line 537 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 1953 "bnf.tab.c"
    break;

  case 62: /* op_assign: ASSIGN_DIV  */
#line 541 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 1961 "bnf.tab.c"
    break;

  case 63: /* op_assign: ASSIGN_MUL  */
#line 545 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 1969 "bnf.tab.c"
    break;

  case 64: /* op_assign: ASSIGN_MOD  */
#line 549 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 1977 "bnf.tab.c"
    break;

  case 65: /* op_assign: ASSIGN_XOR  */
#line 553 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 1985 "bnf.tab.c"
    break;

  case 66: /* op_assign: ASSIGN_SHIFTL  */
#line 557 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 1993 "bnf.tab.c"
    break;

  case 67: /* op_assign: ASSIGN_SHIFTR  */
#line 561 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2001 "bnf.tab.c"
    break;

  case 68: /* op_assign: ASSIGN_AND  */
#line 565 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2009 "bnf.tab.c"
    break;

  case 69: /* op_assign: ASSIGN_OR  */
#line 569 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2017 "bnf.tab.c"
    break;

  case 70: /* op_logic: LOGIC_AND  */
#line 576 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2025 "bnf.tab.c"
    break;

  case 71: /* op_logic: LOGIC_OR  */
#line 580 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2033 "bnf.tab.c"
    break;

  case 72: /* op_cond: COND_GT  */
#line 587 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2041 "bnf.tab.c"
    break;

  case 73: /* op_cond: COND_GEQ  */
#line 591 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2049 "bnf.tab.c"
    break;

  case 74: /* op_cond: COND_LT  */
#line 595 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2057 "bnf.tab.c"
    break;

  case 75: /* op_cond: COND_LEQ  */
#line 599 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2065 "bnf.tab.c"
    break;

  case 76: /* op_cond: COND_EQ  */
#line 603 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2073 "bnf.tab.c"
    break;

  case 77: /* op_cond: COND_NEQ  */
#line 607 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2081 "bnf.tab.c"
    break;

  case 78: /* op_bitwise_xor: BIT_XOR  */
#line 614 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2089 "bnf.tab.c"
    break;

  case 79: /* op_bitwise_or: BIT_OR  */
#line 621 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2097 "bnf.tab.c"
    break;

  case 80: /* op_bitwise_and: BIT_AND  */
#line 628 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2105 "bnf.tab.c"
    break;

  case 81: /* op_bitwise_shift: BIT_SHIFTL  */
#line 635 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2113 "bnf.tab.c"
    break;

  case 82: /* op_bitwise_shift: BIT_SHIFTR  */
#line 639 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2121 "bnf.tab.c"
    break;

  case 83: /* op_arithm_add: PLUS  */
#line 646 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2129 "bnf.tab.c"
    break;

  case 84: /* op_arithm_add: MINUS  */
#line 650 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2137 "bnf.tab.c"
    break;

  case 85: /* op_arithm_mul: STAR  */
#line 657 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2145 "bnf.tab.c"
    break;

  case 86: /* op_arithm_mul: SLASH  */
#line 661 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2153 "bnf.tab.c"
    break;

  case 87: /* op_unary_neg: MINUS  */
#line 668 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2161 "bnf.tab.c"
    break;

  case 88: /* op_unary_logic_neg: BANG  */
#line 675 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2169 "bnf.tab.c"
    break;

  case 89: /* op_unary_inc: UNARY_DEC  */
#line 682 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2177 "bnf.tab.c"
    break;

  case 90: /* op_unary_inc: UNARY_INC  */
#line 686 "bnf.y"
            {
                (yyval.opType) = (yyvsp[0].opType);
            }
#line 2185 "bnf.tab.c"
    break;

  case 91: /* expr: lvl_logic  */
#line 694 "bnf.y"
            { 
                (yyval.node) = (yyvsp[0].node); 
            }
#line 2193 "bnf.tab.c"
    break;

  case 92: /* lvl_logic: lvl_logic op_logic lvl_cond  */
#line 712 "bnf.y"
            {   
                (yyval.node) = newBinaryOp((yyvsp[-1].opType), (yyvsp[-2].node), (yyvsp[0].node));   
            }
#line 2201 "bnf.tab.c"
    break;

  case 93: /* lvl_logic: lvl_cond  */
#line 716 "bnf.y"
            {   
                (yyval.node) = (yyvsp[0].node);    
            }
#line 2209 "bnf.tab.c"
    break;

  case 94: /* lvl_cond: lvl_cond op_cond lvl_bitwise_xor  */
#line 723 "bnf.y"
            {   
                (yyval.node) = newBinaryOp((yyvsp[-1].opType), (yyvsp[-2].node), (yyvsp[0].node));   
            }
#line 2217 "bnf.tab.c"
    break;

  case 95: /* lvl_cond: lvl_bitwise_xor  */
#line 727 "bnf.y"
            {   
                (yyval.node) = (yyvsp[0].node);    
            }
#line 2225 "bnf.tab.c"
    break;

  case 96: /* lvl_bitwise_xor: lvl_bitwise_xor op_bitwise_xor lvl_bitwise_or  */
#line 734 "bnf.y"
            {   
                (yyval.node) = newBinaryOp((yyvsp[-1].opType), (yyvsp[-2].node), (yyvsp[0].node));   
            }
#line 2233 "bnf.tab.c"
    break;

  case 97: /* lvl_bitwise_xor: lvl_bitwise_or  */
#line 738 "bnf.y"
            {   
                (yyval.node) = (yyvsp[0].node);    
            }
#line 2241 "bnf.tab.c"
    break;

  case 98: /* lvl_bitwise_or: lvl_bitwise_or op_bitwise_or lvl_bitwise_and  */
#line 745 "bnf.y"
            {   
                (yyval.node) = newBinaryOp((yyvsp[-1].opType), (yyvsp[-2].node), (yyvsp[0].node));   
            }
#line 2249 "bnf.tab.c"
    break;

  case 99: /* lvl_bitwise_or: lvl_bitwise_and  */
#line 749 "bnf.y"
            {   
                (yyval.node) = (yyvsp[0].node);    
            }
#line 2257 "bnf.tab.c"
    break;

  case 100: /* lvl_bitwise_and: lvl_bitwise_and op_bitwise_and lvl_bitwise_shift  */
#line 756 "bnf.y"
            {   
                (yyval.node) = newBinaryOp((yyvsp[-1].opType), (yyvsp[-2].node), (yyvsp[0].node));   
            }
#line 2265 "bnf.tab.c"
    break;

  case 101: /* lvl_bitwise_and: lvl_bitwise_shift  */
#line 760 "bnf.y"
            {   
                (yyval.node) = (yyvsp[0].node);
            }
#line 2273 "bnf.tab.c"
    break;

  case 102: /* lvl_bitwise_shift: lvl_bitwise_shift op_bitwise_shift lvl_arithm_add  */
#line 767 "bnf.y"
            {   
                (yyval.node) = newBinaryOp((yyvsp[-1].opType), (yyvsp[-2].node), (yyvsp[0].node));   
            }
#line 2281 "bnf.tab.c"
    break;

  case 103: /* lvl_bitwise_shift: lvl_arithm_add  */
#line 771 "bnf.y"
            {   
                (yyval.node) = (yyvsp[0].node);    
            }
#line 2289 "bnf.tab.c"
    break;

  case 104: /* lvl_arithm_add: lvl_arithm_add op_arithm_add lvl_arithm_mul  */
#line 778 "bnf.y"
            {   
                (yyval.node) = newBinaryOp((yyvsp[-1].opType), (yyvsp[-2].node), (yyvsp[0].node));   
            }
#line 2297 "bnf.tab.c"
    break;

  case 105: /* lvl_arithm_add: lvl_arithm_mul  */
#line 782 "bnf.y"
            {   
                (yyval.node) = (yyvsp[0].node);    
            }
#line 2305 "bnf.tab.c"
    break;

  case 106: /* lvl_arithm_mul: lvl_arithm_mul op_arithm_mul lvl_unary_neg  */
#line 789 "bnf.y"
            {   
                (yyval.node) = newBinaryOp((yyvsp[-1].opType), (yyvsp[-2].node), (yyvsp[0].node));   
            }
#line 2313 "bnf.tab.c"
    break;

  case 107: /* lvl_arithm_mul: lvl_unary_neg  */
#line 793 "bnf.y"
            {   
                (yyval.node) = (yyvsp[0].node);    
            }
#line 2321 "bnf.tab.c"
    break;

  case 108: /* lvl_unary_neg: op_unary_neg lvl_unary_inc  */
#line 800 "bnf.y"
            {
                (yyval.node) = newUnaryOp((yyvsp[-1].opType), (yyvsp[0].node));
            }
#line 2329 "bnf.tab.c"
    break;

  case 109: /* lvl_unary_neg: op_unary_logic_neg lvl_unary_inc  */
#line 804 "bnf.y"
            {
                (yyval.node) = newUnaryOp((yyvsp[-1].opType), (yyvsp[0].node));
            }
#line 2337 "bnf.tab.c"
    break;

  case 110: /* lvl_unary_neg: lvl_unary_inc  */
#line 808 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2345 "bnf.tab.c"
    break;

  case 111: /* lvl_unary_inc: op_unary_inc lvl_postfix  */
#line 815 "bnf.y"
            {
                (yyval.node) = newUnaryOp((yyvsp[-1].opType), (yyvsp[0].node));
            }
#line 2353 "bnf.tab.c"
    break;

  case 112: /* lvl_unary_inc: lvl_postfix  */
#line 819 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2361 "bnf.tab.c"
    break;

  case 113: /* lvl_postfix: lvl_postfix DOT nameNode  */
#line 826 "bnf.y"
            {   
                (yyval.node) = newMemberAccess((yyvsp[-2].node), (yyvsp[0].node));   
            }
#line 2369 "bnf.tab.c"
    break;

  case 114: /* lvl_postfix: lvl_postfix LPAREN opt_expr_list RPAREN  */
#line 830 "bnf.y"
            {   
                (yyval.node) = newFnCall((yyvsp[-3].node), (yyvsp[-1].node));   
            }
#line 2377 "bnf.tab.c"
    break;

  case 115: /* lvl_postfix: lvl_postfix LSQUARE opt_expr RSQUARE  */
#line 834 "bnf.y"
            {   
                (yyval.node) = newArrayAccess((yyvsp[-3].node), (yyvsp[-1].node));   
            }
#line 2385 "bnf.tab.c"
    break;

  case 116: /* lvl_postfix: primary  */
#line 838 "bnf.y"
            {   
                (yyval.node) = (yyvsp[0].node);    
            }
#line 2393 "bnf.tab.c"
    break;

  case 117: /* primary: literal  */
#line 845 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2401 "bnf.tab.c"
    break;

  case 118: /* primary: nameNode  */
#line 849 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2409 "bnf.tab.c"
    break;

  case 119: /* primary: LPAREN expr RPAREN  */
#line 853 "bnf.y"
            {
                (yyval.node) = (yyvsp[-1].node);
            }
#line 2417 "bnf.tab.c"
    break;

  case 120: /* opt_expr: expr  */
#line 860 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2425 "bnf.tab.c"
    break;

  case 121: /* opt_expr: %empty  */
#line 864 "bnf.y"
            {
                (yyval.node) = NULL;
            }
#line 2433 "bnf.tab.c"
    break;

  case 122: /* opt_expr_list: expr_list  */
#line 871 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2441 "bnf.tab.c"
    break;

  case 123: /* opt_expr_list: %empty  */
#line 875 "bnf.y"
            {
                (yyval.node) = NULL;
            }
#line 2449 "bnf.tab.c"
    break;

  case 124: /* expr_list: expr_list COMMA expr  */
#line 882 "bnf.y"
            {
                astNodeLLAppend((yyvsp[-2].node)->data.nodeList.list, (yyvsp[0].node));
                (yyval.node) = (yyvsp[-2].node);
            }
#line 2458 "bnf.tab.c"
    break;

  case 125: /* expr_list: expr  */
#line 887 "bnf.y"
            {
                (yyval.node) = newNodeList((yyvsp[0].node));
            }
#line 2466 "bnf.tab.c"
    break;

  case 126: /* literal: TRUE  */
#line 894 "bnf.y"
            { 
                (yyval.node) = newBoolValue(true); 
            }
#line 2474 "bnf.tab.c"
    break;

  case 127: /* literal: FALSE  */
#line 898 "bnf.y"
            {
                (yyval.node) = newBoolValue(false);
            }
#line 2482 "bnf.tab.c"
    break;

  case 128: /* literal: NUMBER  */
#line 902 "bnf.y"
            {
                (yyval.node) = newNumberValue((yyvsp[0].numberValue));
            }
#line 2490 "bnf.tab.c"
    break;

  case 129: /* literal: arr_initializer  */
#line 906 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2498 "bnf.tab.c"
    break;

  case 130: /* literal: STRING  */
#line 910 "bnf.y"
            {
                (yyval.node) = newStringValue((yyvsp[0].stringValue));
            }
#line 2506 "bnf.tab.c"
    break;

  case 131: /* arr_initializer: LSQUARE RSQUARE  */
#line 918 "bnf.y"
            {
                (yyval.node) = newArray(NULL);
            }
#line 2514 "bnf.tab.c"
    break;

  case 132: /* arr_initializer: LSQUARE expr_list RSQUARE  */
#line 922 "bnf.y"
            {
                (yyval.node) = newArray((yyvsp[-1].node));
            }
#line 2522 "bnf.tab.c"
    break;

  case 133: /* nameNode: NAME  */
#line 930 "bnf.y"
            {
                (yyval.node) = newName((yyvsp[0].stringValue));
            }
#line 2530 "bnf.tab.c"
    break;

  case 134: /* enum_stmt: ENUM nameNode LBRACE enum_opt_elements RBRACE  */
#line 938 "bnf.y"
            {
                (yyval.node) = newEnumDef((yyvsp[-3].node)->data.stringValue, (yyvsp[-1].node));
            }
#line 2538 "bnf.tab.c"
    break;

  case 135: /* enum_element: delimiter_list nameNode  */
#line 945 "bnf.y"
            {
                (yyval.node) = newEnumElement((yyvsp[0].node)->data.stringValue, NULL);
            }
#line 2546 "bnf.tab.c"
    break;

  case 136: /* enum_element: delimiter_list nameNode EQUALS expr  */
#line 949 "bnf.y"
            {
                (yyval.node) = newEnumElement((yyvsp[-2].node)->data.stringValue, (yyvsp[0].node));
            }
#line 2554 "bnf.tab.c"
    break;

  case 137: /* enum_opt_elements: enum_elements  */
#line 956 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2562 "bnf.tab.c"
    break;

  case 138: /* enum_opt_elements: %empty  */
#line 960 "bnf.y"
            {
                (yyval.node) = NULL;
            }
#line 2570 "bnf.tab.c"
    break;

  case 139: /* enum_elements: enum_elements COMMA enum_element  */
#line 967 "bnf.y"
            {
                astNodeLLAppend((yyvsp[-2].node)->data.nodeList.list, (yyvsp[0].node));
                (yyval.node) = (yyvsp[-2].node);
            }
#line 2579 "bnf.tab.c"
    break;

  case 140: /* enum_elements: enum_element  */
#line 972 "bnf.y"
            {
                (yyval.node) = newNodeList((yyvsp[0].node));
            }
#line 2587 "bnf.tab.c"
    break;

  case 141: /* fn_parameter: nameNode nameNode  */
#line 980 "bnf.y"
            {
                (yyval.node) = newParamInfo((yyvsp[0].node)->data.stringValue, (yyvsp[-1].node));
            }
#line 2595 "bnf.tab.c"
    break;

  case 142: /* fn_parameter: var_type nameNode  */
#line 984 "bnf.y"
            {
                (yyval.node) = newParamInfo((yyvsp[0].node)->data.stringValue, (yyvsp[-1].node));
            }
#line 2603 "bnf.tab.c"
    break;

  case 143: /* fn_opt_params_list: fn_params_list  */
#line 991 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2611 "bnf.tab.c"
    break;

  case 144: /* fn_opt_params_list: %empty  */
#line 995 "bnf.y"
            {
                (yyval.node) = NULL;
            }
#line 2619 "bnf.tab.c"
    break;

  case 145: /* fn_params_list: fn_params_list COMMA fn_parameter  */
#line 1002 "bnf.y"
            {
                astNodeLLAppend((yyvsp[-2].node)->data.nodeList.list, (yyvsp[0].node));
                (yyval.node) = (yyvsp[-2].node);
            }
#line 2628 "bnf.tab.c"
    break;

  case 146: /* fn_params_list: fn_parameter  */
#line 1007 "bnf.y"
            {
                (yyval.node) = newNodeList((yyvsp[0].node));
            }
#line 2636 "bnf.tab.c"
    break;

  case 147: /* fn_def: FN nameNode LPAREN fn_opt_params_list RPAREN code_block  */
#line 1014 "bnf.y"
            {
                (yyval.node) = newFnDef((yyvsp[-4].node)->data.stringValue, (yyvsp[-2].node), (yyvsp[0].node));
            }
#line 2644 "bnf.tab.c"
    break;

  case 148: /* var_decl: VAR var_decl_assign_list  */
#line 1022 "bnf.y"
            {
                (yyval.node) = newVarDeclBatch((yyvsp[0].node));
            }
#line 2652 "bnf.tab.c"
    break;

  case 149: /* var_decl_assign_list: var_decl_assign_list COMMA var_decl_assign  */
#line 1029 "bnf.y"
            {
                astNodeLLAppend((yyvsp[-2].node)->data.nodeList.list, (yyvsp[0].node));
                (yyval.node) = (yyvsp[-2].node);
            }
#line 2661 "bnf.tab.c"
    break;

  case 150: /* var_decl_assign_list: var_decl_assign  */
#line 1034 "bnf.y"
            {
                (yyval.node) = newNodeList((yyvsp[0].node));
            }
#line 2669 "bnf.tab.c"
    break;

  case 151: /* var_decl_assign: var_decl_modifiers nameNode EQUALS expr  */
#line 1041 "bnf.y"
            {
                (yyval.node) = newVarDecl((yyvsp[-2].node)->data.stringValue, (yyvsp[-3].modifier), (yyvsp[0].node));
            }
#line 2677 "bnf.tab.c"
    break;

  case 152: /* var_decl_assign: var_decl_modifiers nameNode  */
#line 1045 "bnf.y"
            {
                (yyval.node) = newVarDecl((yyvsp[0].node)->data.stringValue, (yyvsp[-1].modifier), NULL);
            }
#line 2685 "bnf.tab.c"
    break;

  case 153: /* var_decl_modifiers: var_decl_modifiers var_decl_modifier  */
#line 1052 "bnf.y"
            {
                (yyval.modifier) = (Modifier)((int)(yyvsp[-1].modifier) | (int)(yyvsp[0].modifier));
            }
#line 2693 "bnf.tab.c"
    break;

  case 154: /* var_decl_modifiers: %empty  */
#line 1056 "bnf.y"
            {
                (yyval.modifier) = MODIFIER_NONE;
            }
#line 2701 "bnf.tab.c"
    break;

  case 155: /* var_decl_modifier: CONST  */
#line 1063 "bnf.y"
            {
                (yyval.modifier) = (yyvsp[0].modifier);
            }
#line 2709 "bnf.tab.c"
    break;

  case 156: /* var_type: VAR  */
#line 1070 "bnf.y"
            {
                (yyval.node) = newName("var");
            }
#line 2717 "bnf.tab.c"
    break;

  case 157: /* assign: expr op_assign assign_rhs  */
#line 1078 "bnf.y"
            {
                (yyval.node) = newVarAssign((yyvsp[-1].opType), (yyvsp[-2].node), (yyvsp[0].node));
            }
#line 2725 "bnf.tab.c"
    break;

  case 158: /* assign_rhs: expr  */
#line 1085 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2733 "bnf.tab.c"
    break;

  case 159: /* assign_rhs: lambda_expr  */
#line 1089 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2741 "bnf.tab.c"
    break;

  case 160: /* lambda_expr: HASH LSQUARE lambda_opt_capture_list RSQUARE LPAREN fn_opt_params_list RPAREN code_block  */
#line 1097 "bnf.y"
            {
                (yyval.node) = newLambda((yyvsp[-5].node), (yyvsp[-2].node), (yyvsp[0].node));
            }
#line 2749 "bnf.tab.c"
    break;

  case 161: /* lambda_opt_capture_list: lambda_capture_list  */
#line 1104 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2757 "bnf.tab.c"
    break;

  case 162: /* lambda_opt_capture_list: %empty  */
#line 1108 "bnf.y"
            {
                (yyval.node) = NULL;
            }
#line 2765 "bnf.tab.c"
    break;

  case 163: /* lambda_capture_list: lambda_capture_list COMMA nameNode  */
#line 1115 "bnf.y"
            {
                astNodeLLAppend((yyvsp[-2].node)->data.nodeList.list, (yyvsp[0].node));
                (yyval.node) = (yyvsp[-2].node);
            }
#line 2774 "bnf.tab.c"
    break;

  case 164: /* lambda_capture_list: nameNode  */
#line 1120 "bnf.y"
            {
                (yyval.node) = newNodeList((yyvsp[0].node));
            }
#line 2782 "bnf.tab.c"
    break;

  case 165: /* class_def: CLASS nameNode opt_delimiter_list LBRACE opt_class_member_list opt_delimiter_list RBRACE  */
#line 1128 "bnf.y"
            {
                (yyval.node) = newClassDef((yyvsp[-5].node)->data.stringValue, (yyvsp[-2].node));
            }
#line 2790 "bnf.tab.c"
    break;

  case 166: /* opt_class_member_list: class_member_list  */
#line 1135 "bnf.y"
            {
                (yyval.node) = (yyvsp[0].node);
            }
#line 2798 "bnf.tab.c"
    break;

  case 167: /* opt_class_member_list: %empty  */
#line 1139 "bnf.y"
            {
                (yyval.node) = NULL;
            }
#line 2806 "bnf.tab.c"
    break;

  case 168: /* class_member_list: class_member_list delimiter_list class_member  */
#line 1146 "bnf.y"
            {
                astNodeLLAppend((yyvsp[-2].node)->data.nodeList.list, (yyvsp[0].node));
                (yyval.node) = (yyvsp[-2].node);
            }
#line 2815 "bnf.tab.c"
    break;

  case 169: /* class_member_list: class_member  */
#line 1151 "bnf.y"
            {
                (yyval.node) = newNodeList((yyvsp[0].node));
            }
#line 2823 "bnf.tab.c"
    break;

  case 170: /* class_member: class_member_mod_list var_type nameNode  */
#line 1158 "bnf.y"
            {
                (yyval.node) = newClassFieldDef((yyvsp[0].node)->data.stringValue, (yyvsp[-2].modifier), (yyvsp[-1].node));
            }
#line 2831 "bnf.tab.c"
    break;

  case 171: /* class_member: class_member_mod_list fn_def  */
#line 1162 "bnf.y"
            {
                (yyval.node) = newClassMethodDef((yyvsp[-1].modifier), (yyvsp[0].node));
            }
#line 2839 "bnf.tab.c"
    break;

  case 172: /* class_member_mod_list: class_member_mod_list class_member_mod  */
#line 1169 "bnf.y"
            {
                (yyval.modifier) = (Modifier)((int)(yyvsp[-1].modifier) | (int)(yyvsp[0].modifier));
            }
#line 2847 "bnf.tab.c"
    break;

  case 173: /* class_member_mod_list: %empty  */
#line 1173 "bnf.y"
            {
                (yyval.modifier) = MODIFIER_NONE;
            }
#line 2855 "bnf.tab.c"
    break;

  case 174: /* class_member_mod: HIDDEN  */
#line 1180 "bnf.y"
            {
                (yyval.modifier) = (yyvsp[0].modifier);
            }
#line 2863 "bnf.tab.c"
    break;

  case 175: /* class_member_mod: CONST  */
#line 1184 "bnf.y"
            {
                (yyval.modifier) = (yyvsp[0].modifier);
            }
#line 2871 "bnf.tab.c"
    break;

  case 176: /* class_member_mod: STATIC  */
#line 1188 "bnf.y"
            {
                (yyval.modifier) = (yyvsp[0].modifier);
            }
#line 2879 "bnf.tab.c"
    break;


#line 2883 "bnf.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1193 "bnf.y"
