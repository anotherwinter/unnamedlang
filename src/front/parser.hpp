// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


/**
 ** \file parser.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
// "%code requires" blocks.
#line 10 "bnf.y"

#include "ast.h"
#include "lexer.h"
#include "shared.h"
#include <stack>

struct ParserContext
{
    std::stack<ASTNode*> nodeLLStack;
};

#line 61 "parser.hpp"


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
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

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

namespace yy {
#line 196 "parser.hpp"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef YYSTYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define YYSTYPE in C++, use %define api.value.type"
# endif
    typedef YYSTYPE value_type;
#else
    /// Symbol semantic values.
    union value_type
    {
#line 29 "bnf.y"

    const char* stringValue;   /* STRING, nameNode etc */
    double      numberValue;   /* NUMBER */
    bool        boolValue;     /* TRUE, FALSE */
    ASTNode*    node;          /* nonterminals */
    TokenType   opType;        /* operators */
    Modifier    modifier;      /* modifiers like CONST */

#line 223 "parser.hpp"

    };
#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;


    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const std::string& m)
        : std::runtime_error (m)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        YYEMPTY = -2,
    YYEOF = 0,                     // "end of file"
    YYerror = 256,                 // error
    YYUNDEF = 257,                 // "invalid token"
    STRING = 258,                  // STRING
    NUMBER = 259,                  // NUMBER
    TRUE = 260,                    // TRUE
    FALSE = 261,                   // FALSE
    NAME = 262,                    // NAME
    MINUS = 263,                   // MINUS
    PLUS = 264,                    // PLUS
    BANG = 265,                    // BANG
    STAR = 266,                    // STAR
    SLASH = 267,                   // SLASH
    MOD = 268,                     // MOD
    UNARY_DEC = 269,               // UNARY_DEC
    UNARY_INC = 270,               // UNARY_INC
    EQUALS = 271,                  // EQUALS
    ASSIGN_ADD = 272,              // ASSIGN_ADD
    ASSIGN_SUB = 273,              // ASSIGN_SUB
    ASSIGN_DIV = 274,              // ASSIGN_DIV
    ASSIGN_MUL = 275,              // ASSIGN_MUL
    ASSIGN_MOD = 276,              // ASSIGN_MOD
    ASSIGN_XOR = 277,              // ASSIGN_XOR
    ASSIGN_SHIFTL = 278,           // ASSIGN_SHIFTL
    ASSIGN_SHIFTR = 279,           // ASSIGN_SHIFTR
    ASSIGN_AND = 280,              // ASSIGN_AND
    ASSIGN_OR = 281,               // ASSIGN_OR
    BIT_SHIFTL = 282,              // BIT_SHIFTL
    BIT_SHIFTR = 283,              // BIT_SHIFTR
    BIT_AND = 284,                 // BIT_AND
    BIT_OR = 285,                  // BIT_OR
    BIT_XOR = 286,                 // BIT_XOR
    COND_GT = 287,                 // COND_GT
    COND_GEQ = 288,                // COND_GEQ
    COND_LT = 289,                 // COND_LT
    COND_LEQ = 290,                // COND_LEQ
    COND_EQ = 291,                 // COND_EQ
    COND_NEQ = 292,                // COND_NEQ
    LOGIC_AND = 293,               // LOGIC_AND
    LOGIC_OR = 294,                // LOGIC_OR
    DOT = 295,                     // DOT
    LPAREN = 296,                  // LPAREN
    RPAREN = 297,                  // RPAREN
    LANGLE = 298,                  // LANGLE
    RANGLE = 299,                  // RANGLE
    LBRACE = 300,                  // LBRACE
    RBRACE = 301,                  // RBRACE
    LSQUARE = 302,                 // LSQUARE
    RSQUARE = 303,                 // RSQUARE
    COLON = 304,                   // COLON
    COMMA = 305,                   // COMMA
    ERROR = 306,                   // ERROR
    DELIMITER = 307,               // DELIMITER
    ENDOFTOKENS = 308,             // ENDOFTOKENS
    FN = 309,                      // FN
    RET = 310,                     // RET
    IF = 311,                      // IF
    ELSE = 312,                    // ELSE
    WHL = 313,                     // WHL
    FOR = 314,                     // FOR
    SWITCH = 315,                  // SWITCH
    BRK = 316,                     // BRK
    DEFAULT = 317,                 // DEFAULT
    CLASS = 318,                   // CLASS
    TOSS = 319,                    // TOSS
    DEL = 320,                     // DEL
    ENUM = 321,                    // ENUM
    OP = 322,                      // OP
    CONST = 323,                   // CONST
    LOCAL = 324,                   // LOCAL
    VAR = 325,                     // VAR
    STATIC = 326,                  // STATIC
    HIDDEN = 327,                  // HIDDEN
    LOWER_THAN_ELSE = 328,         // LOWER_THAN_ELSE
    UMINUS = 329                   // UMINUS
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 75, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_STRING = 3,                            // STRING
        S_NUMBER = 4,                            // NUMBER
        S_TRUE = 5,                              // TRUE
        S_FALSE = 6,                             // FALSE
        S_NAME = 7,                              // NAME
        S_MINUS = 8,                             // MINUS
        S_PLUS = 9,                              // PLUS
        S_BANG = 10,                             // BANG
        S_STAR = 11,                             // STAR
        S_SLASH = 12,                            // SLASH
        S_MOD = 13,                              // MOD
        S_UNARY_DEC = 14,                        // UNARY_DEC
        S_UNARY_INC = 15,                        // UNARY_INC
        S_EQUALS = 16,                           // EQUALS
        S_ASSIGN_ADD = 17,                       // ASSIGN_ADD
        S_ASSIGN_SUB = 18,                       // ASSIGN_SUB
        S_ASSIGN_DIV = 19,                       // ASSIGN_DIV
        S_ASSIGN_MUL = 20,                       // ASSIGN_MUL
        S_ASSIGN_MOD = 21,                       // ASSIGN_MOD
        S_ASSIGN_XOR = 22,                       // ASSIGN_XOR
        S_ASSIGN_SHIFTL = 23,                    // ASSIGN_SHIFTL
        S_ASSIGN_SHIFTR = 24,                    // ASSIGN_SHIFTR
        S_ASSIGN_AND = 25,                       // ASSIGN_AND
        S_ASSIGN_OR = 26,                        // ASSIGN_OR
        S_BIT_SHIFTL = 27,                       // BIT_SHIFTL
        S_BIT_SHIFTR = 28,                       // BIT_SHIFTR
        S_BIT_AND = 29,                          // BIT_AND
        S_BIT_OR = 30,                           // BIT_OR
        S_BIT_XOR = 31,                          // BIT_XOR
        S_COND_GT = 32,                          // COND_GT
        S_COND_GEQ = 33,                         // COND_GEQ
        S_COND_LT = 34,                          // COND_LT
        S_COND_LEQ = 35,                         // COND_LEQ
        S_COND_EQ = 36,                          // COND_EQ
        S_COND_NEQ = 37,                         // COND_NEQ
        S_LOGIC_AND = 38,                        // LOGIC_AND
        S_LOGIC_OR = 39,                         // LOGIC_OR
        S_DOT = 40,                              // DOT
        S_LPAREN = 41,                           // LPAREN
        S_RPAREN = 42,                           // RPAREN
        S_LANGLE = 43,                           // LANGLE
        S_RANGLE = 44,                           // RANGLE
        S_LBRACE = 45,                           // LBRACE
        S_RBRACE = 46,                           // RBRACE
        S_LSQUARE = 47,                          // LSQUARE
        S_RSQUARE = 48,                          // RSQUARE
        S_COLON = 49,                            // COLON
        S_COMMA = 50,                            // COMMA
        S_ERROR = 51,                            // ERROR
        S_DELIMITER = 52,                        // DELIMITER
        S_ENDOFTOKENS = 53,                      // ENDOFTOKENS
        S_FN = 54,                               // FN
        S_RET = 55,                              // RET
        S_IF = 56,                               // IF
        S_ELSE = 57,                             // ELSE
        S_WHL = 58,                              // WHL
        S_FOR = 59,                              // FOR
        S_SWITCH = 60,                           // SWITCH
        S_BRK = 61,                              // BRK
        S_DEFAULT = 62,                          // DEFAULT
        S_CLASS = 63,                            // CLASS
        S_TOSS = 64,                             // TOSS
        S_DEL = 65,                              // DEL
        S_ENUM = 66,                             // ENUM
        S_OP = 67,                               // OP
        S_CONST = 68,                            // CONST
        S_LOCAL = 69,                            // LOCAL
        S_VAR = 70,                              // VAR
        S_STATIC = 71,                           // STATIC
        S_HIDDEN = 72,                           // HIDDEN
        S_LOWER_THAN_ELSE = 73,                  // LOWER_THAN_ELSE
        S_UMINUS = 74,                           // UMINUS
        S_YYACCEPT = 75,                         // $accept
        S_program = 76,                          // program
        S_opt_delimiter_list = 77,               // opt_delimiter_list
        S_delimiter_list = 78,                   // delimiter_list
        S_opt_program_stmt_list = 79,            // opt_program_stmt_list
        S_program_stmt_list = 80,                // program_stmt_list
        S_program_stmt = 81,                     // program_stmt
        S_global_stmt = 82,                      // global_stmt
        S_global_stmt_stmt = 83,                 // global_stmt_stmt
        S_stmt = 84,                             // stmt
        S_stmt_stmt = 85,                        // stmt_stmt
        S_code_block = 86,                       // code_block
        S_opt_stmt_list = 87,                    // opt_stmt_list
        S_stmt_list = 88,                        // stmt_list
        S_89_1 = 89,                             // $@1
        S_stmt_list_tail = 90,                   // stmt_list_tail
        S_code_block_or_stmt = 91,               // code_block_or_stmt
        S_del_stmt = 92,                         // del_stmt
        S_cntrl_cnstrct = 93,                    // cntrl_cnstrct
        S_loop_whl = 94,                         // loop_whl
        S_loop_for = 95,                         // loop_for
        S_loop_for_init = 96,                    // loop_for_init
        S_loop_for_cond = 97,                    // loop_for_cond
        S_loop_for_post_iter = 98,               // loop_for_post_iter
        S_ret_stmt = 99,                         // ret_stmt
        S_switch_stmt = 100,                     // switch_stmt
        S_opt_switch_case_list = 101,            // opt_switch_case_list
        S_switch_case_list = 102,                // switch_case_list
        S_switch_case = 103,                     // switch_case
        S_brk_stmt = 104,                        // brk_stmt
        S_if_stmt = 105,                         // if_stmt
        S_opt_else = 106,                        // opt_else
        S_toss_stmt = 107,                       // toss_stmt
        S_op_assign = 108,                       // op_assign
        S_op_logic = 109,                        // op_logic
        S_op_cond = 110,                         // op_cond
        S_op_bitwise_xor = 111,                  // op_bitwise_xor
        S_op_bitwise_or = 112,                   // op_bitwise_or
        S_op_bitwise_and = 113,                  // op_bitwise_and
        S_op_bitwise_shift = 114,                // op_bitwise_shift
        S_op_arithm_add = 115,                   // op_arithm_add
        S_op_arithm_mul = 116,                   // op_arithm_mul
        S_op_unary_neg = 117,                    // op_unary_neg
        S_op_unary_logic_neg = 118,              // op_unary_logic_neg
        S_op_unary_inc = 119,                    // op_unary_inc
        S_expr = 120,                            // expr
        S_lvl_logic = 121,                       // lvl_logic
        S_lvl_cond = 122,                        // lvl_cond
        S_lvl_bitwise_xor = 123,                 // lvl_bitwise_xor
        S_lvl_bitwise_or = 124,                  // lvl_bitwise_or
        S_lvl_bitwise_and = 125,                 // lvl_bitwise_and
        S_lvl_bitwise_shift = 126,               // lvl_bitwise_shift
        S_lvl_arithm_add = 127,                  // lvl_arithm_add
        S_lvl_arithm_mul = 128,                  // lvl_arithm_mul
        S_lvl_unary_neg = 129,                   // lvl_unary_neg
        S_lvl_unary_inc = 130,                   // lvl_unary_inc
        S_lvl_postfix = 131,                     // lvl_postfix
        S_primary = 132,                         // primary
        S_opt_expr = 133,                        // opt_expr
        S_opt_expr_list = 134,                   // opt_expr_list
        S_expr_list = 135,                       // expr_list
        S_literal = 136,                         // literal
        S_arr_initializer = 137,                 // arr_initializer
        S_nameNode = 138,                        // nameNode
        S_enum_stmt = 139,                       // enum_stmt
        S_enum_element = 140,                    // enum_element
        S_enum_opt_elements = 141,               // enum_opt_elements
        S_enum_elements = 142,                   // enum_elements
        S_fn_parameter = 143,                    // fn_parameter
        S_fn_opt_params_list = 144,              // fn_opt_params_list
        S_fn_params_list = 145,                  // fn_params_list
        S_fn_def = 146,                          // fn_def
        S_var_decl = 147,                        // var_decl
        S_var_decl_assign_list = 148,            // var_decl_assign_list
        S_var_decl_assign = 149,                 // var_decl_assign
        S_var_decl_modifiers = 150,              // var_decl_modifiers
        S_var_decl_modifier = 151,               // var_decl_modifier
        S_var_type = 152,                        // var_type
        S_assign = 153,                          // assign
        S_assign_rhs = 154,                      // assign_rhs
        S_class_def = 155,                       // class_def
        S_opt_class_member_list = 156,           // opt_class_member_list
        S_class_member_list = 157,               // class_member_list
        S_class_member = 158,                    // class_member
        S_class_member_mod_list = 159,           // class_member_mod_list
        S_class_member_mod = 160                 // class_member_mod
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value (std::move (that.value))
      {}
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);
      /// Constructor for valueless symbols.
      basic_symbol (typename Base::kind_type t);

      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    YY_RVREF (value_type) v);

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        Base::clear ();
      }

#if YYDEBUG || 0
      /// The user-facing name of this symbol.
      const char *name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }
#endif // #if YYDEBUG || 0


      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {};

    /// Build a parser object.
    parser (Lexer& lexer_yyarg, ParserContext& ctx_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param msg    a description of the syntax error.
    virtual void error (const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

#if YYDEBUG || 0
    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static const char *symbol_name (symbol_kind_type yysymbol);
#endif // #if YYDEBUG || 0




  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef short state_type;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const short yypact_ninf_;
    static const short yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

#if YYDEBUG || 0
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif // #if YYDEBUG || 0


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const unsigned char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const short yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const unsigned char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const unsigned char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 462,     ///< Last index in yytable_.
      yynnts_ = 86,  ///< Number of nonterminal symbols.
      yyfinal_ = 86 ///< Termination state number.
    };


    // User arguments.
    Lexer& lexer;
    ParserContext& ctx;

  };


} // yy
#line 1002 "parser.hpp"




#endif // !YY_YY_PARSER_HPP_INCLUDED
