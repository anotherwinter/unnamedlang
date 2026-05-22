// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 27 "bnf.y"

#include <stdio.h>
#include "front/bison_wrapper.h"

ASTNode* astRoot = NULL;

#line 48 "parser.cpp"


#include "front/parser.hpp"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 126 "parser.cpp"

  /// Build a parser object.
  parser::parser (Lexer& lexer_yyarg, ParserContext& ctx_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      ctx (ctx_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v)
    : Base (t)
    , value (YY_MOVE (v))
  {}


  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
  }

  // by_kind.
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YY_USE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    // User initialization code.
#line 11 "bnf.y"
{
    setLexer(&lexer);
}

#line 461 "parser.cpp"


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, lexer));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: opt_program_stmt_list
#line 234 "bnf.y"
            {
                astRoot = (yystack_[0].value.node);
            }
#line 595 "parser.cpp"
    break;

  case 7: // opt_program_stmt_list: program_stmt_list
#line 251 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 603 "parser.cpp"
    break;

  case 8: // opt_program_stmt_list: opt_delimiter_list
#line 255 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 611 "parser.cpp"
    break;

  case 9: // program_stmt_list: program_stmt_list_head program_stmt_list_tail
#line 262 "bnf.y"
            {
                astNodeLLConcat((yystack_[1].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 620 "parser.cpp"
    break;

  case 10: // program_stmt_list_head: opt_delimiter_list program_stmt
#line 270 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 628 "parser.cpp"
    break;

  case 11: // program_stmt_list_tail: delimiter_list program_stmt program_stmt_list_tail
#line 277 "bnf.y"
            {
                (yylhs.value.node) = astNodeLLPrepend((yystack_[0].value.node), (yystack_[1].value.node));
            }
#line 636 "parser.cpp"
    break;

  case 12: // program_stmt_list_tail: delimiter_list
#line 281 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 644 "parser.cpp"
    break;

  case 13: // program_stmt_list_tail: %empty
#line 285 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 652 "parser.cpp"
    break;

  case 14: // program_stmt: stmt
#line 292 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 660 "parser.cpp"
    break;

  case 15: // program_stmt: global_stmt
#line 296 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 668 "parser.cpp"
    break;

  case 16: // global_stmt: class_def
#line 304 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 676 "parser.cpp"
    break;

  case 17: // global_stmt: fn_def
#line 308 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 684 "parser.cpp"
    break;

  case 18: // global_stmt: enum_stmt
#line 312 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 692 "parser.cpp"
    break;

  case 19: // stmt: stmt_stmt
#line 320 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 700 "parser.cpp"
    break;

  case 20: // stmt_stmt: var_decl
#line 327 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 708 "parser.cpp"
    break;

  case 21: // stmt_stmt: cntrl_cnstrct
#line 331 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 716 "parser.cpp"
    break;

  case 22: // stmt_stmt: del_stmt
#line 335 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 724 "parser.cpp"
    break;

  case 23: // stmt_stmt: assign
#line 339 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 732 "parser.cpp"
    break;

  case 24: // stmt_stmt: expr
#line 343 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 740 "parser.cpp"
    break;

  case 25: // code_block: opt_delimiter_list LBRACE opt_stmt_list RBRACE
#line 351 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 748 "parser.cpp"
    break;

  case 26: // opt_stmt_list: stmt_list
#line 358 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 756 "parser.cpp"
    break;

  case 27: // opt_stmt_list: opt_delimiter_list
#line 362 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 764 "parser.cpp"
    break;

  case 28: // stmt_list: stmt_list_head stmt_list_tail
#line 369 "bnf.y"
            {
                astNodeLLConcat((yystack_[1].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 773 "parser.cpp"
    break;

  case 29: // stmt_list_head: opt_delimiter_list stmt_stmt
#line 377 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 781 "parser.cpp"
    break;

  case 30: // stmt_list_tail: delimiter_list stmt_stmt stmt_list_tail
#line 384 "bnf.y"
            {
                (yylhs.value.node) = astNodeLLPrepend((yystack_[0].value.node), (yystack_[1].value.node));
            }
#line 789 "parser.cpp"
    break;

  case 31: // stmt_list_tail: delimiter_list
#line 388 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 797 "parser.cpp"
    break;

  case 32: // stmt_list_tail: %empty
#line 392 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 805 "parser.cpp"
    break;

  case 33: // code_block_or_stmt: code_block
#line 399 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 813 "parser.cpp"
    break;

  case 34: // code_block_or_stmt: opt_delimiter_list stmt
#line 403 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 821 "parser.cpp"
    break;

  case 35: // del_stmt: DEL expr
#line 411 "bnf.y"
            {
                (yylhs.value.node) = newDelStmt((yystack_[0].value.node));
            }
#line 829 "parser.cpp"
    break;

  case 36: // cntrl_cnstrct: loop_whl
#line 419 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 837 "parser.cpp"
    break;

  case 37: // cntrl_cnstrct: loop_for
#line 423 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 845 "parser.cpp"
    break;

  case 38: // cntrl_cnstrct: switch_stmt
#line 427 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 853 "parser.cpp"
    break;

  case 39: // cntrl_cnstrct: ret_stmt
#line 431 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 861 "parser.cpp"
    break;

  case 40: // cntrl_cnstrct: brk_stmt
#line 435 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 869 "parser.cpp"
    break;

  case 41: // cntrl_cnstrct: if_stmt
#line 439 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 877 "parser.cpp"
    break;

  case 42: // cntrl_cnstrct: toss_stmt
#line 443 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 885 "parser.cpp"
    break;

  case 43: // loop_whl: WHL LPAREN expr RPAREN code_block_or_stmt
#line 450 "bnf.y"
            {
                (yylhs.value.node) = newLoopWhl((yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 893 "parser.cpp"
    break;

  case 44: // loop_for: FOR LPAREN loop_for_init COLON loop_for_cond COLON loop_for_post_iter RPAREN code_block_or_stmt
#line 457 "bnf.y"
            {
                (yylhs.value.node) = newLoopFor((yystack_[6].value.node), (yystack_[4].value.node), (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 901 "parser.cpp"
    break;

  case 45: // loop_for_init: opt_expr_list
#line 464 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 909 "parser.cpp"
    break;

  case 46: // loop_for_cond: loop_for_cond COMMA lvl_logic
#line 471 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 918 "parser.cpp"
    break;

  case 47: // loop_for_cond: lvl_logic
#line 476 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 926 "parser.cpp"
    break;

  case 48: // loop_for_post_iter: opt_expr_list
#line 483 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 934 "parser.cpp"
    break;

  case 49: // ret_stmt: RET
#line 490 "bnf.y"
            {
                (yylhs.value.node) = newRetStmt(NULL);
            }
#line 942 "parser.cpp"
    break;

  case 50: // ret_stmt: RET expr
#line 494 "bnf.y"
            {
                (yylhs.value.node) = newRetStmt((yystack_[0].value.node));
            }
#line 950 "parser.cpp"
    break;

  case 51: // switch_stmt: SWITCH LPAREN expr RPAREN LBRACE opt_switch_case_list opt_delimiter_list RBRACE
#line 501 "bnf.y"
            {
                (yylhs.value.node) = newSwitchStmt((yystack_[5].value.node), (yystack_[2].value.node));
            }
#line 958 "parser.cpp"
    break;

  case 52: // opt_switch_case_list: switch_case_list
#line 508 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 966 "parser.cpp"
    break;

  case 53: // opt_switch_case_list: %empty
#line 512 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 974 "parser.cpp"
    break;

  case 54: // switch_case_list: switch_case_list delimiter_list switch_case
#line 519 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 983 "parser.cpp"
    break;

  case 55: // switch_case_list: switch_case
#line 524 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 991 "parser.cpp"
    break;

  case 56: // switch_case: expr COLON code_block_or_stmt
#line 531 "bnf.y"
            {
                (yylhs.value.node) = newSwitchCase((yystack_[2].value.node), (yystack_[0].value.node), false);
            }
#line 999 "parser.cpp"
    break;

  case 57: // switch_case: DEFAULT COLON code_block_or_stmt
#line 535 "bnf.y"
            {
                (yylhs.value.node) = newSwitchCase(NULL, (yystack_[0].value.node), true);
            }
#line 1007 "parser.cpp"
    break;

  case 58: // brk_stmt: BRK LPAREN expr RPAREN
#line 542 "bnf.y"
            {
                (yylhs.value.node) = newBrkStmt((yystack_[1].value.node));
            }
#line 1015 "parser.cpp"
    break;

  case 59: // brk_stmt: BRK
#line 546 "bnf.y"
            {
                (yylhs.value.node) = newBrkStmt(NULL);
            }
#line 1023 "parser.cpp"
    break;

  case 60: // if_stmt: IF LPAREN expr RPAREN code_block_or_stmt opt_else
#line 553 "bnf.y"
            {
                (yylhs.value.node) = newIfStmt((yystack_[3].value.node), (yystack_[1].value.node), (yystack_[0].value.node));
            }
#line 1031 "parser.cpp"
    break;

  case 61: // opt_else: ELSE code_block_or_stmt
#line 560 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1039 "parser.cpp"
    break;

  case 62: // opt_else: %empty
#line 564 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1047 "parser.cpp"
    break;

  case 63: // toss_stmt: TOSS expr
#line 571 "bnf.y"
            {
                (yylhs.value.node) = newTossStmt((yystack_[0].value.node));
            }
#line 1055 "parser.cpp"
    break;

  case 64: // op_assign: EQUALS
#line 579 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1063 "parser.cpp"
    break;

  case 65: // op_assign: ASSIGN_ADD
#line 583 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1071 "parser.cpp"
    break;

  case 66: // op_assign: ASSIGN_SUB
#line 587 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1079 "parser.cpp"
    break;

  case 67: // op_assign: ASSIGN_DIV
#line 591 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1087 "parser.cpp"
    break;

  case 68: // op_assign: ASSIGN_MUL
#line 595 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1095 "parser.cpp"
    break;

  case 69: // op_assign: ASSIGN_MOD
#line 599 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1103 "parser.cpp"
    break;

  case 70: // op_assign: ASSIGN_XOR
#line 603 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1111 "parser.cpp"
    break;

  case 71: // op_assign: ASSIGN_SHIFTL
#line 607 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1119 "parser.cpp"
    break;

  case 72: // op_assign: ASSIGN_SHIFTR
#line 611 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1127 "parser.cpp"
    break;

  case 73: // op_assign: ASSIGN_AND
#line 615 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1135 "parser.cpp"
    break;

  case 74: // op_assign: ASSIGN_OR
#line 619 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1143 "parser.cpp"
    break;

  case 75: // op_logic: LOGIC_AND
#line 626 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1151 "parser.cpp"
    break;

  case 76: // op_logic: LOGIC_OR
#line 630 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1159 "parser.cpp"
    break;

  case 77: // op_cond: COND_GT
#line 637 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1167 "parser.cpp"
    break;

  case 78: // op_cond: COND_GEQ
#line 641 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1175 "parser.cpp"
    break;

  case 79: // op_cond: COND_LT
#line 645 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1183 "parser.cpp"
    break;

  case 80: // op_cond: COND_LEQ
#line 649 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1191 "parser.cpp"
    break;

  case 81: // op_cond: COND_EQ
#line 653 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1199 "parser.cpp"
    break;

  case 82: // op_cond: COND_NEQ
#line 657 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1207 "parser.cpp"
    break;

  case 83: // op_bitwise_xor: BIT_XOR
#line 664 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1215 "parser.cpp"
    break;

  case 84: // op_bitwise_or: BIT_OR
#line 671 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1223 "parser.cpp"
    break;

  case 85: // op_bitwise_and: BIT_AND
#line 678 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1231 "parser.cpp"
    break;

  case 86: // op_bitwise_shift: BIT_SHIFTL
#line 685 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1239 "parser.cpp"
    break;

  case 87: // op_bitwise_shift: BIT_SHIFTR
#line 689 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1247 "parser.cpp"
    break;

  case 88: // op_arithm_add: PLUS
#line 696 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1255 "parser.cpp"
    break;

  case 89: // op_arithm_add: MINUS
#line 700 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1263 "parser.cpp"
    break;

  case 90: // op_arithm_mul: STAR
#line 707 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1271 "parser.cpp"
    break;

  case 91: // op_arithm_mul: SLASH
#line 711 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1279 "parser.cpp"
    break;

  case 92: // op_unary_neg: MINUS
#line 718 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1287 "parser.cpp"
    break;

  case 93: // op_unary_logic_neg: BANG
#line 725 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1295 "parser.cpp"
    break;

  case 94: // op_unary_inc: UNARY_DEC
#line 732 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1303 "parser.cpp"
    break;

  case 95: // op_unary_inc: UNARY_INC
#line 736 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1311 "parser.cpp"
    break;

  case 96: // expr: lvl_logic
#line 744 "bnf.y"
            { 
                (yylhs.value.node) = (yystack_[0].value.node); 
            }
#line 1319 "parser.cpp"
    break;

  case 97: // lvl_logic: lvl_logic op_logic lvl_cond
#line 751 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1327 "parser.cpp"
    break;

  case 98: // lvl_logic: lvl_cond
#line 755 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1335 "parser.cpp"
    break;

  case 99: // lvl_cond: lvl_cond op_cond lvl_bitwise_xor
#line 762 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1343 "parser.cpp"
    break;

  case 100: // lvl_cond: lvl_bitwise_xor
#line 766 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1351 "parser.cpp"
    break;

  case 101: // lvl_bitwise_xor: lvl_bitwise_xor op_bitwise_xor lvl_bitwise_or
#line 773 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1359 "parser.cpp"
    break;

  case 102: // lvl_bitwise_xor: lvl_bitwise_or
#line 777 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1367 "parser.cpp"
    break;

  case 103: // lvl_bitwise_or: lvl_bitwise_or op_bitwise_or lvl_bitwise_and
#line 784 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1375 "parser.cpp"
    break;

  case 104: // lvl_bitwise_or: lvl_bitwise_and
#line 788 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1383 "parser.cpp"
    break;

  case 105: // lvl_bitwise_and: lvl_bitwise_and op_bitwise_and lvl_bitwise_shift
#line 795 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1391 "parser.cpp"
    break;

  case 106: // lvl_bitwise_and: lvl_bitwise_shift
#line 799 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1399 "parser.cpp"
    break;

  case 107: // lvl_bitwise_shift: lvl_bitwise_shift op_bitwise_shift lvl_arithm_add
#line 806 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1407 "parser.cpp"
    break;

  case 108: // lvl_bitwise_shift: lvl_arithm_add
#line 810 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1415 "parser.cpp"
    break;

  case 109: // lvl_arithm_add: lvl_arithm_add op_arithm_add lvl_arithm_mul
#line 817 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1423 "parser.cpp"
    break;

  case 110: // lvl_arithm_add: lvl_arithm_mul
#line 821 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1431 "parser.cpp"
    break;

  case 111: // lvl_arithm_mul: lvl_arithm_mul op_arithm_mul lvl_unary_neg
#line 828 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1439 "parser.cpp"
    break;

  case 112: // lvl_arithm_mul: lvl_unary_neg
#line 832 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1447 "parser.cpp"
    break;

  case 113: // lvl_unary_neg: op_unary_neg lvl_unary_inc
#line 839 "bnf.y"
            {
                (yylhs.value.node) = newUnaryPre((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1455 "parser.cpp"
    break;

  case 114: // lvl_unary_neg: op_unary_logic_neg lvl_unary_inc
#line 843 "bnf.y"
            {
                (yylhs.value.node) = newUnaryPre((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1463 "parser.cpp"
    break;

  case 115: // lvl_unary_neg: lvl_unary_inc
#line 847 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1471 "parser.cpp"
    break;

  case 116: // lvl_unary_inc: op_unary_inc lvl_postfix
#line 854 "bnf.y"
            {
                (yylhs.value.node) = newUnaryPre((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1479 "parser.cpp"
    break;

  case 117: // lvl_unary_inc: lvl_postfix
#line 858 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1487 "parser.cpp"
    break;

  case 118: // lvl_postfix: lvl_postfix DOT nameNode
#line 865 "bnf.y"
            {   
                (yylhs.value.node) = newMemberAccess((yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1495 "parser.cpp"
    break;

  case 119: // lvl_postfix: lvl_postfix LPAREN opt_expr_list RPAREN
#line 869 "bnf.y"
            {   
                (yylhs.value.node) = newFnCall((yystack_[3].value.node), (yystack_[1].value.node));   
            }
#line 1503 "parser.cpp"
    break;

  case 120: // lvl_postfix: lvl_postfix LSQUARE opt_expr RSQUARE
#line 873 "bnf.y"
            {   
                (yylhs.value.node) = newArrayAccess((yystack_[3].value.node), (yystack_[1].value.node));   
            }
#line 1511 "parser.cpp"
    break;

  case 121: // lvl_postfix: lvl_postfix op_unary_inc
#line 877 "bnf.y"
            {   
                (yylhs.value.node) = newUnaryPost((yystack_[0].value.opType), (yystack_[1].value.node)); 
            }
#line 1519 "parser.cpp"
    break;

  case 122: // lvl_postfix: primary
#line 881 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1527 "parser.cpp"
    break;

  case 123: // primary: literal
#line 888 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1535 "parser.cpp"
    break;

  case 124: // primary: nameNode
#line 892 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1543 "parser.cpp"
    break;

  case 125: // primary: LPAREN expr RPAREN
#line 896 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 1551 "parser.cpp"
    break;

  case 126: // opt_expr: expr
#line 903 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1559 "parser.cpp"
    break;

  case 127: // opt_expr: %empty
#line 907 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1567 "parser.cpp"
    break;

  case 128: // opt_expr_list: expr_list
#line 914 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1575 "parser.cpp"
    break;

  case 129: // opt_expr_list: %empty
#line 918 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1583 "parser.cpp"
    break;

  case 130: // expr_list: expr_list COMMA expr
#line 925 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1592 "parser.cpp"
    break;

  case 131: // expr_list: expr
#line 930 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1600 "parser.cpp"
    break;

  case 132: // literal: TRUE
#line 937 "bnf.y"
            { 
                (yylhs.value.node) = newBoolValue(true); 
            }
#line 1608 "parser.cpp"
    break;

  case 133: // literal: FALSE
#line 941 "bnf.y"
            {
                (yylhs.value.node) = newBoolValue(false);
            }
#line 1616 "parser.cpp"
    break;

  case 134: // literal: NUMBER
#line 945 "bnf.y"
            {
                (yylhs.value.node) = newNumberValue((yystack_[0].value.numberValue));
            }
#line 1624 "parser.cpp"
    break;

  case 135: // literal: arr_initializer
#line 949 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1632 "parser.cpp"
    break;

  case 136: // literal: STRING
#line 953 "bnf.y"
            {
                (yylhs.value.node) = newStringValue((yystack_[0].value.stringValue));
            }
#line 1640 "parser.cpp"
    break;

  case 137: // arr_initializer: LSQUARE RSQUARE
#line 961 "bnf.y"
            {
                (yylhs.value.node) = newArray(NULL);
            }
#line 1648 "parser.cpp"
    break;

  case 138: // arr_initializer: LSQUARE expr_list RSQUARE
#line 965 "bnf.y"
            {
                (yylhs.value.node) = newArray((yystack_[1].value.node));
            }
#line 1656 "parser.cpp"
    break;

  case 139: // nameNode: NAME
#line 973 "bnf.y"
            {
                (yylhs.value.node) = newName((yystack_[0].value.stringValue));
            }
#line 1664 "parser.cpp"
    break;

  case 140: // enum_stmt: ENUM nameNode LBRACE enum_opt_elements RBRACE
#line 981 "bnf.y"
            {
                (yylhs.value.node) = newEnumDef((yystack_[3].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1672 "parser.cpp"
    break;

  case 141: // enum_element: delimiter_list nameNode
#line 988 "bnf.y"
            {
                (yylhs.value.node) = newEnumElement((yystack_[0].value.node)->data.stringValue, NULL);
            }
#line 1680 "parser.cpp"
    break;

  case 142: // enum_element: delimiter_list nameNode EQUALS expr
#line 992 "bnf.y"
            {
                (yylhs.value.node) = newEnumElement((yystack_[2].value.node)->data.stringValue, (yystack_[0].value.node));
            }
#line 1688 "parser.cpp"
    break;

  case 143: // enum_opt_elements: enum_elements
#line 999 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1696 "parser.cpp"
    break;

  case 144: // enum_opt_elements: %empty
#line 1003 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1704 "parser.cpp"
    break;

  case 145: // enum_elements: enum_elements COMMA enum_element
#line 1010 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1713 "parser.cpp"
    break;

  case 146: // enum_elements: enum_element
#line 1015 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1721 "parser.cpp"
    break;

  case 147: // fn_parameter: nameNode nameNode
#line 1023 "bnf.y"
            {
                (yylhs.value.node) = newParamInfo((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1729 "parser.cpp"
    break;

  case 148: // fn_parameter: var_type nameNode
#line 1027 "bnf.y"
            {
                (yylhs.value.node) = newParamInfo((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1737 "parser.cpp"
    break;

  case 149: // fn_opt_params_list: fn_params_list
#line 1034 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1745 "parser.cpp"
    break;

  case 150: // fn_opt_params_list: %empty
#line 1038 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1753 "parser.cpp"
    break;

  case 151: // fn_params_list: fn_params_list COMMA fn_parameter
#line 1045 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1762 "parser.cpp"
    break;

  case 152: // fn_params_list: fn_parameter
#line 1050 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1770 "parser.cpp"
    break;

  case 153: // fn_def: FN nameNode LPAREN fn_opt_params_list RPAREN code_block
#line 1057 "bnf.y"
            {
                (yylhs.value.node) = newFnDef((yystack_[4].value.node)->data.stringValue, (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 1778 "parser.cpp"
    break;

  case 154: // var_decl: VAR var_decl_assign_list
#line 1065 "bnf.y"
            {
                (yylhs.value.node) = newVarDeclBatch((yystack_[0].value.node));
            }
#line 1786 "parser.cpp"
    break;

  case 155: // var_decl_assign_list: var_decl_assign_list COMMA var_decl_assign
#line 1072 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1795 "parser.cpp"
    break;

  case 156: // var_decl_assign_list: var_decl_assign
#line 1077 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1803 "parser.cpp"
    break;

  case 157: // var_decl_assign: var_decl_modifiers nameNode EQUALS expr
#line 1084 "bnf.y"
            {
                (yylhs.value.node) = newVarDecl((yystack_[2].value.node)->data.stringValue, (yystack_[3].value.modifier), (yystack_[0].value.node));
            }
#line 1811 "parser.cpp"
    break;

  case 158: // var_decl_assign: var_decl_modifiers nameNode
#line 1088 "bnf.y"
            {
                (yylhs.value.node) = newVarDecl((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.modifier), NULL);
            }
#line 1819 "parser.cpp"
    break;

  case 159: // var_decl_modifiers: var_decl_modifiers var_decl_modifier
#line 1095 "bnf.y"
            {
                (yylhs.value.modifier) = static_cast<Modifier>(to_underlying((yystack_[1].value.modifier)) | to_underlying((yystack_[0].value.modifier)));
            }
#line 1827 "parser.cpp"
    break;

  case 160: // var_decl_modifiers: %empty
#line 1099 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::None;
            }
#line 1835 "parser.cpp"
    break;

  case 161: // var_decl_modifier: CONST
#line 1106 "bnf.y"
            {
                (yylhs.value.modifier) = (yystack_[0].value.modifier);
            }
#line 1843 "parser.cpp"
    break;

  case 162: // var_decl_modifier: LOCAL
#line 1110 "bnf.y"
            {
                (yylhs.value.modifier) = (yystack_[0].value.modifier);
            }
#line 1851 "parser.cpp"
    break;

  case 163: // var_type: VAR
#line 1117 "bnf.y"
            {
                (yylhs.value.node) = newName("var");
            }
#line 1859 "parser.cpp"
    break;

  case 164: // assign: expr op_assign assign_rhs
#line 1125 "bnf.y"
            {
                (yylhs.value.node) = newVarAssign((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 1867 "parser.cpp"
    break;

  case 165: // assign_rhs: expr
#line 1132 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1875 "parser.cpp"
    break;

  case 166: // class_def: CLASS nameNode opt_delimiter_list LBRACE opt_class_member_list RBRACE
#line 1179 "bnf.y"
            {
                (yylhs.value.node) = newClassDef((yystack_[4].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1883 "parser.cpp"
    break;

  case 167: // opt_class_member_list: class_member_list
#line 1186 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1891 "parser.cpp"
    break;

  case 168: // opt_class_member_list: opt_delimiter_list
#line 1190 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1899 "parser.cpp"
    break;

  case 169: // class_member_list: class_member_list_head class_member_list_tail
#line 1196 "bnf.y"
            {
                astNodeLLConcat((yystack_[1].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 1908 "parser.cpp"
    break;

  case 170: // class_member_list_head: opt_delimiter_list class_member
#line 1204 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1916 "parser.cpp"
    break;

  case 171: // class_member_list_tail: delimiter_list class_member class_member_list_tail
#line 1211 "bnf.y"
            {
                (yylhs.value.node) = astNodeLLPrepend((yystack_[0].value.node), (yystack_[1].value.node));
            }
#line 1924 "parser.cpp"
    break;

  case 172: // class_member_list_tail: delimiter_list
#line 1215 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1932 "parser.cpp"
    break;

  case 173: // class_member_list_tail: %empty
#line 1219 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1940 "parser.cpp"
    break;

  case 174: // class_member: class_member_mod_list var_type nameNode
#line 1226 "bnf.y"
            {
                (yylhs.value.node) = newClassFieldDef((yystack_[0].value.node)->data.stringValue, (yystack_[2].value.modifier), (yystack_[1].value.node));
            }
#line 1948 "parser.cpp"
    break;

  case 175: // class_member: class_member_mod_list fn_def
#line 1230 "bnf.y"
            {
                (yylhs.value.node) = newClassMethodDef((yystack_[1].value.modifier), (yystack_[0].value.node));
            }
#line 1956 "parser.cpp"
    break;

  case 176: // class_member_mod_list: class_member_mod_list class_member_mod
#line 1272 "bnf.y"
            {
                (yylhs.value.modifier) = static_cast<Modifier>(static_cast<uint16_t>((yystack_[1].value.modifier)) | static_cast<uint16_t>((yystack_[0].value.modifier)));
            }
#line 1964 "parser.cpp"
    break;

  case 177: // class_member_mod_list: %empty
#line 1276 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::None;
            }
#line 1972 "parser.cpp"
    break;

  case 178: // class_member_mod: HIDDEN
#line 1283 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Hidden;
            }
#line 1980 "parser.cpp"
    break;

  case 179: // class_member_mod: CONST
#line 1287 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Const;
            }
#line 1988 "parser.cpp"
    break;

  case 180: // class_member_mod: STATIC
#line 1291 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Static;
            }
#line 1996 "parser.cpp"
    break;


#line 2000 "parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const short parser::yypact_ninf_ = -220;

  const short parser::yytable_ninf_ = -173;

  const short
  parser::yypact_[] =
  {
     -34,  -220,    21,   268,   -13,  -220,  -220,   -34,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,   142,    30,
      36,   142,     6,    13,    17,    23,    39,    36,   142,   142,
      36,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,   387,   387,   117,   420,    44,
      -8,    31,    54,    62,    61,    99,   101,  -220,  -220,    16,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
     200,  -220,    52,  -220,  -220,   -31,    58,  -220,   142,   142,
     142,   142,   142,   -34,  -220,  -220,    57,    64,  -220,    -2,
    -220,  -220,    16,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,   142,  -220,  -220,   142,  -220,  -220,
    -220,  -220,  -220,  -220,   142,  -220,   142,  -220,   142,  -220,
     142,  -220,  -220,   142,  -220,  -220,   142,  -220,  -220,   142,
      36,   142,   142,  -220,   -34,  -220,  -220,   142,    -1,    76,
      88,    89,  -220,    91,   100,   111,   109,   -34,  -220,  -220,
    -220,   143,  -220,  -220,  -220,    -8,    31,    54,    62,    61,
      99,   101,  -220,  -220,   118,  -220,   113,  -220,  -220,  -220,
      36,  -220,   121,   115,    36,   -34,   -34,   142,   122,  -220,
     -34,     9,  -220,   120,   119,  -220,   142,  -220,  -220,  -220,
     -34,    -1,  -220,    45,  -220,   114,  -220,    67,    44,   373,
     127,   128,  -220,   -34,   152,  -220,   -34,  -220,   130,  -220,
    -220,   -34,  -220,   -34,  -220,   142,   142,   135,   -34,   -34,
    -220,   136,  -220,    25,  -220,   -32,  -220,   142,  -220,   357,
     131,  -220,   -34,  -220,   137,  -220,    44,   -34,   132,   129,
     -34,  -220,  -220,  -220,  -220,    36,  -220,   -34,  -220,  -220,
    -220,   289,  -220,   -34,  -220,  -220,  -220,  -220,  -220,  -220,
     -34,  -220,  -220
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,     6,     0,     8,     3,     2,     7,    13,     1,   136,
     134,   132,   133,   139,    92,    93,    94,    95,     0,     0,
       0,    49,     0,     0,     0,     0,    59,     0,     0,     0,
       0,   160,    10,    15,    14,    19,    22,    21,    36,    37,
      39,    38,    40,    41,    42,     0,     0,     0,    24,    96,
      98,   100,   102,   104,   106,   108,   110,   112,   115,   117,
     122,   123,   135,   124,    18,    17,    20,    23,    16,     5,
      12,     9,     0,   137,   131,     0,     0,    50,     0,     0,
     129,     0,     0,     4,    63,    35,     0,   154,   156,     0,
     113,   114,   116,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,     0,    75,    76,     0,    77,    78,
      79,    80,    81,    82,     0,    83,     0,    84,     0,    85,
       0,    86,    87,     0,    89,    88,     0,    90,    91,     0,
       0,   129,   127,   121,    13,   125,   138,     0,   150,     0,
       0,     0,    45,   128,     0,     0,     0,   144,   160,   161,
     162,   158,   159,   165,   164,    97,    99,   101,   103,   105,
     107,   109,   111,   118,     0,   126,     0,    11,   130,   163,
       0,   152,     0,   149,     0,     4,     4,     0,     0,    58,
       4,     0,   146,     0,   143,   155,     0,   119,   120,   147,
       4,     0,   148,     0,    33,    62,    43,     0,    47,    53,
     177,     0,   167,   173,   141,   140,     0,   157,     0,   153,
     151,     4,    34,     4,    60,   129,     0,     0,     4,    52,
      55,     0,   170,     0,   166,   177,   169,     0,   145,    27,
       0,    26,    32,    61,     0,    48,    46,     4,     0,     0,
       4,   179,   180,   178,   175,     0,   176,   173,   142,    29,
      25,    31,    28,     4,    57,    51,    54,    56,   174,   171,
      32,    44,    30
  };

  const short
  parser::yypgoto_[] =
  {
    -220,  -220,     2,    -7,  -220,  -220,  -220,    53,   116,  -220,
      -5,  -219,     0,  -220,  -220,  -220,   -66,  -172,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,   -44,
    -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,  -220,
    -220,  -220,  -220,  -220,  -220,   -50,    -6,  -170,    90,    84,
      85,    82,    96,    79,    92,    93,    83,   164,  -220,  -220,
    -128,   198,  -220,  -220,   -19,  -220,    18,  -220,  -220,    28,
    -220,  -220,     4,  -220,  -220,    75,  -220,  -220,     5,  -220,
    -220,  -220,  -220,  -220,  -220,   -18,     7,  -220,  -220
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     2,   193,     4,     5,     6,     7,    71,    32,    33,
      34,    35,   194,   230,   231,   232,   252,   195,    36,    37,
      38,    39,   141,   197,   234,    40,    41,   218,   219,   220,
      42,    43,   214,    44,   104,   107,   114,   116,   118,   120,
     123,   126,   129,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,   166,
     142,   143,    61,    62,    63,    64,   182,   183,   184,   171,
     172,   173,    65,    66,    87,    88,    89,   152,   174,    67,
     154,    68,   201,   202,   203,   226,   222,   223,   246
  };

  const short
  parser::yytable_[] =
  {
      70,    76,     3,   164,   196,    13,    13,   198,    83,   133,
     249,    86,    72,    74,  -172,    77,    13,   136,     1,   137,
      69,     8,    84,    85,   108,   109,   110,   111,   112,   113,
      16,    17,   260,     9,    10,    11,    12,    13,    14,    69,
      15,   233,   133,    13,    16,    17,   236,    78,     9,    10,
      11,    12,    13,    14,    79,    15,   130,   131,    80,    16,
      17,    69,   115,   132,    81,   254,   149,   150,   257,   169,
     151,    18,   139,   140,    74,   144,   145,    19,    73,    20,
      82,   261,   105,   106,   117,   146,    18,   235,   121,   122,
     211,   119,    19,   241,   135,   169,   242,   243,   153,   138,
      21,    22,   147,    23,    24,    25,    26,   124,   125,    28,
      29,   163,   127,   128,   148,    31,   215,   216,   175,   170,
       9,    10,    11,    12,    13,    74,   165,    70,    90,    91,
     176,   168,     9,    10,    11,    12,    13,    14,   177,    15,
     181,   137,   178,    16,    17,     9,    10,    11,    12,    13,
      14,   189,    15,   179,   180,   192,    16,    17,    18,   186,
     187,   188,   204,   190,    19,   191,   205,   199,   227,   206,
      18,   213,   170,  -168,   224,   211,    19,   250,   255,   253,
     207,    69,   200,    18,   237,   240,   134,   167,   212,    19,
     209,   217,   208,   221,   262,   256,   225,   155,   156,   181,
     158,   157,   160,     9,    10,    11,    12,    13,    14,    74,
      15,    92,   239,   229,    16,    17,   159,    75,   161,   210,
     238,   248,   162,   185,   228,   251,   258,   244,   245,   259,
       0,     0,   247,   221,     0,     0,     0,     0,     0,     0,
     225,    18,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,    69,   251,    20,    21,    22,     0,    23,    24,
      25,    26,     0,    27,    28,    29,    30,     0,     0,     0,
      31,     9,    10,    11,    12,    13,    14,     0,    15,     0,
       0,     0,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     9,    10,    11,    12,    13,    14,     0,    15,
       0,     0,     0,    16,    17,     0,     0,     0,     0,    18,
       0,     0,     0,     0,     0,    19,     0,     0,     0,     0,
       0,     0,    20,    21,    22,     0,    23,    24,    25,    26,
      18,    27,    28,    29,    30,     0,    19,     0,    31,     0,
       0,    69,     0,     0,    21,    22,     0,    23,    24,    25,
      26,     0,     0,    28,    29,     0,     0,     0,     0,    31,
       9,    10,    11,    12,    13,    14,     0,    15,     0,     0,
       0,    16,    17,     0,     0,     0,     9,    10,    11,    12,
      13,    14,     0,    15,     0,     0,     0,    16,    17,     0,
       9,    10,    11,    12,    13,     0,     0,     0,    18,     0,
       0,    16,    17,     0,    19,     0,     0,     0,     0,     0,
       0,     0,    21,    22,    18,    23,    24,    25,    26,     0,
      19,    28,    29,     0,     0,     0,     0,    31,    18,     0,
       0,     0,     0,     0,    19,   217,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103
  };

  const short
  parser::yycheck_[] =
  {
       7,    20,     0,   131,   176,     7,     7,   177,    27,    59,
     229,    30,    18,    19,    46,    21,     7,    48,    52,    50,
      52,     0,    28,    29,    32,    33,    34,    35,    36,    37,
      14,    15,   251,     3,     4,     5,     6,     7,     8,    52,
      10,   213,    92,     7,    14,    15,   216,    41,     3,     4,
       5,     6,     7,     8,    41,    10,    40,    41,    41,    14,
      15,    52,    31,    47,    41,   237,    68,    69,   240,    70,
      89,    41,    78,    79,    80,    81,    82,    47,    48,    54,
      41,   253,    38,    39,    30,    83,    41,   215,    27,    28,
      45,    29,    47,    68,    42,    70,    71,    72,   104,    41,
      55,    56,    45,    58,    59,    60,    61,     8,     9,    64,
      65,   130,    11,    12,    50,    70,    49,    50,    42,   138,
       3,     4,     5,     6,     7,   131,   132,   134,    45,    46,
      42,   137,     3,     4,     5,     6,     7,     8,    49,    10,
     147,    50,    42,    14,    15,     3,     4,     5,     6,     7,
       8,   170,    10,    42,    45,   174,    14,    15,    41,    16,
      42,    48,   181,    42,    47,    50,    46,    45,    16,    50,
      41,    57,   191,    46,    46,    45,    47,    46,    46,    42,
     186,    52,   180,    41,    49,    49,    70,   134,   193,    47,
     190,    62,   190,   199,   260,   239,   203,   107,   114,   206,
     118,   116,   123,     3,     4,     5,     6,     7,     8,   215,
      10,    47,   219,   211,    14,    15,   120,    19,   126,   191,
     218,   227,   129,   148,   206,   232,   245,   223,   223,   247,
      -1,    -1,   225,   239,    -1,    -1,    -1,    -1,    -1,    -1,
     247,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    52,   260,    54,    55,    56,    -1,    58,    59,
      60,    61,    -1,    63,    64,    65,    66,    -1,    -1,    -1,
      70,     3,     4,     5,     6,     7,     8,    -1,    10,    -1,
      -1,    -1,    14,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    -1,    -1,    14,    15,    -1,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    -1,    58,    59,    60,    61,
      41,    63,    64,    65,    66,    -1,    47,    -1,    70,    -1,
      -1,    52,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    -1,
      -1,    14,    15,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    10,    -1,    -1,    -1,    14,    15,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    41,    -1,
      -1,    14,    15,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    56,    41,    58,    59,    60,    61,    -1,
      47,    64,    65,    -1,    -1,    -1,    -1,    70,    41,    -1,
      -1,    -1,    -1,    -1,    47,    62,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    52,    76,    77,    78,    79,    80,    81,     0,     3,
       4,     5,     6,     7,     8,    10,    14,    15,    41,    47,
      54,    55,    56,    58,    59,    60,    61,    63,    64,    65,
      66,    70,    83,    84,    85,    86,    93,    94,    95,    96,
     100,   101,   105,   106,   108,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   137,   138,   139,   140,   147,   148,   154,   156,    52,
      78,    82,   121,    48,   121,   136,   139,   121,    41,    41,
      41,    41,    41,   139,   121,   121,   139,   149,   150,   151,
     131,   131,   132,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   109,    38,    39,   110,    32,    33,
      34,    35,    36,    37,   111,    31,   112,    30,   113,    29,
     114,    27,    28,   115,     8,     9,   116,    11,    12,   117,
      40,    41,    47,   120,    83,    42,    48,    50,    41,   121,
     121,    97,   135,   136,   121,   121,    77,    45,    50,    68,
      69,   139,   152,   121,   155,   123,   124,   125,   126,   127,
     128,   129,   130,   139,   135,   121,   134,    82,   121,    70,
     139,   144,   145,   146,   153,    42,    42,    49,    42,    42,
      45,    78,   141,   142,   143,   150,    16,    42,    48,   139,
      42,    50,   139,    77,    87,    92,    92,    98,   122,    45,
      77,   157,   158,   159,   139,    46,    50,   121,    77,    87,
     144,    45,    85,    57,   107,    49,    50,    62,   102,   103,
     104,   121,   161,   162,    46,    78,   160,    16,   141,    77,
      88,    89,    90,    92,    99,   135,   122,    49,    77,    78,
      49,    68,    71,    72,   147,   153,   163,   161,   121,    86,
      46,    78,    91,    42,    92,    46,   104,    92,   139,   160,
      86,    92,    91
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    75,    76,    77,    77,    78,    78,    79,    79,    80,
      81,    82,    82,    82,    83,    83,    84,    84,    84,    85,
      86,    86,    86,    86,    86,    87,    88,    88,    89,    90,
      91,    91,    91,    92,    92,    93,    94,    94,    94,    94,
      94,    94,    94,    95,    96,    97,    98,    98,    99,   100,
     100,   101,   102,   102,   103,   103,   104,   104,   105,   105,
     106,   107,   107,   108,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   110,   110,   111,   111,   111,
     111,   111,   111,   112,   113,   114,   115,   115,   116,   116,
     117,   117,   118,   119,   120,   120,   121,   122,   122,   123,
     123,   124,   124,   125,   125,   126,   126,   127,   127,   128,
     128,   129,   129,   130,   130,   130,   131,   131,   132,   132,
     132,   132,   132,   133,   133,   133,   134,   134,   135,   135,
     136,   136,   137,   137,   137,   137,   137,   138,   138,   139,
     140,   141,   141,   142,   142,   143,   143,   144,   144,   145,
     145,   146,   146,   147,   148,   149,   149,   150,   150,   151,
     151,   152,   152,   153,   154,   155,   156,   157,   157,   158,
     159,   160,   160,   160,   161,   161,   162,   162,   163,   163,
     163
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     0,     2,     1,     1,     1,     2,
       2,     3,     1,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     1,     1,     2,     2,
       3,     1,     0,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     5,     9,     1,     3,     1,     1,     1,
       2,     8,     1,     0,     3,     1,     3,     3,     4,     1,
       6,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     2,     2,     1,     2,     1,     3,     4,
       4,     2,     1,     1,     1,     3,     1,     0,     1,     0,
       3,     1,     1,     1,     1,     1,     1,     2,     3,     1,
       5,     2,     4,     1,     0,     3,     1,     2,     2,     1,
       0,     3,     1,     6,     2,     3,     1,     4,     2,     2,
       0,     1,     1,     1,     3,     1,     6,     1,     1,     2,
       2,     3,     1,     0,     3,     2,     2,     0,     1,     1,
       1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "STRING", "NUMBER",
  "TRUE", "FALSE", "NAME", "MINUS", "PLUS", "BANG", "STAR", "SLASH", "MOD",
  "UNARY_DEC", "UNARY_INC", "EQUALS", "ASSIGN_ADD", "ASSIGN_SUB",
  "ASSIGN_DIV", "ASSIGN_MUL", "ASSIGN_MOD", "ASSIGN_XOR", "ASSIGN_SHIFTL",
  "ASSIGN_SHIFTR", "ASSIGN_AND", "ASSIGN_OR", "BIT_SHIFTL", "BIT_SHIFTR",
  "BIT_AND", "BIT_OR", "BIT_XOR", "COND_GT", "COND_GEQ", "COND_LT",
  "COND_LEQ", "COND_EQ", "COND_NEQ", "LOGIC_AND", "LOGIC_OR", "DOT",
  "LPAREN", "RPAREN", "LANGLE", "RANGLE", "LBRACE", "RBRACE", "LSQUARE",
  "RSQUARE", "COLON", "COMMA", "ERROR", "DELIMITER", "ENDOFTOKENS", "FN",
  "RET", "IF", "ELSE", "WHL", "FOR", "SWITCH", "BRK", "DEFAULT", "CLASS",
  "TOSS", "DEL", "ENUM", "OP", "CONST", "LOCAL", "VAR", "STATIC", "HIDDEN",
  "LOWER_THAN_ELSE", "UMINUS", "$accept", "program", "opt_delimiter_list",
  "delimiter_list", "opt_program_stmt_list", "program_stmt_list",
  "program_stmt_list_head", "program_stmt_list_tail", "program_stmt",
  "global_stmt", "stmt", "stmt_stmt", "code_block", "opt_stmt_list",
  "stmt_list", "stmt_list_head", "stmt_list_tail", "code_block_or_stmt",
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
  "var_decl_modifier", "var_type", "assign", "assign_rhs", "class_def",
  "opt_class_member_list", "class_member_list", "class_member_list_head",
  "class_member_list_tail", "class_member", "class_member_mod_list",
  "class_member_mod", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   233,   233,   240,   241,   245,   246,   250,   254,   261,
     269,   276,   280,   285,   291,   295,   303,   307,   311,   319,
     326,   330,   334,   338,   342,   350,   357,   361,   368,   376,
     383,   387,   392,   398,   402,   410,   418,   422,   426,   430,
     434,   438,   442,   449,   456,   463,   470,   475,   482,   489,
     493,   500,   507,   512,   518,   523,   530,   534,   541,   545,
     552,   559,   563,   570,   578,   582,   586,   590,   594,   598,
     602,   606,   610,   614,   618,   625,   629,   636,   640,   644,
     648,   652,   656,   663,   670,   677,   684,   688,   695,   699,
     706,   710,   717,   724,   731,   735,   743,   750,   754,   761,
     765,   772,   776,   783,   787,   794,   798,   805,   809,   816,
     820,   827,   831,   838,   842,   846,   853,   857,   864,   868,
     872,   876,   880,   887,   891,   895,   902,   907,   913,   918,
     924,   929,   936,   940,   944,   948,   952,   960,   964,   972,
     980,   987,   991,   998,  1003,  1009,  1014,  1022,  1026,  1033,
    1038,  1044,  1049,  1056,  1064,  1071,  1076,  1083,  1087,  1094,
    1099,  1105,  1109,  1116,  1124,  1131,  1178,  1185,  1189,  1195,
    1203,  1210,  1214,  1219,  1225,  1229,  1271,  1276,  1282,  1286,
    1290
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74
    };
    // Last valid token kind.
    const int code_max = 329;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 2617 "parser.cpp"

#line 1296 "bnf.y"
