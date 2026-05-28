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
  case 2: // program: opt_delimiter_list opt_program_stmt_list
#line 231 "bnf.y"
            {
                astRoot = (yystack_[0].value.node);
            }
#line 595 "parser.cpp"
    break;

  case 7: // opt_program_stmt_list: program_stmt_list
#line 248 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 603 "parser.cpp"
    break;

  case 8: // opt_program_stmt_list: %empty
#line 252 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 611 "parser.cpp"
    break;

  case 9: // program_stmt_list: program_stmt program_stmt_list_tail
#line 259 "bnf.y"
            {
                (yylhs.value.node) = astNodeLLConcat(newNodeList((yystack_[1].value.node)), (yystack_[0].value.node));
            }
#line 619 "parser.cpp"
    break;

  case 10: // program_stmt_list_tail: delimiter_list program_stmt program_stmt_list_tail
#line 266 "bnf.y"
            {
                (yylhs.value.node) = astNodeLLPrepend((yystack_[0].value.node), (yystack_[1].value.node));
            }
#line 627 "parser.cpp"
    break;

  case 11: // program_stmt_list_tail: delimiter_list
#line 270 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 635 "parser.cpp"
    break;

  case 12: // program_stmt_list_tail: %empty
#line 274 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 643 "parser.cpp"
    break;

  case 13: // program_stmt: stmt
#line 281 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 651 "parser.cpp"
    break;

  case 14: // program_stmt: global_stmt
#line 285 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 659 "parser.cpp"
    break;

  case 15: // global_stmt: class_def
#line 293 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 667 "parser.cpp"
    break;

  case 16: // global_stmt: fn_def
#line 297 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 675 "parser.cpp"
    break;

  case 17: // global_stmt: enum_stmt
#line 301 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 683 "parser.cpp"
    break;

  case 18: // stmt: stmt_stmt
#line 309 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 691 "parser.cpp"
    break;

  case 19: // stmt_stmt: var_decl
#line 316 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 699 "parser.cpp"
    break;

  case 20: // stmt_stmt: cntrl_cnstrct
#line 320 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 707 "parser.cpp"
    break;

  case 21: // stmt_stmt: del_stmt
#line 324 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 715 "parser.cpp"
    break;

  case 22: // stmt_stmt: assign
#line 328 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 723 "parser.cpp"
    break;

  case 23: // stmt_stmt: expr
#line 332 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 731 "parser.cpp"
    break;

  case 24: // code_block: opt_delimiter_list LBRACE opt_stmt_list RBRACE
#line 340 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 739 "parser.cpp"
    break;

  case 25: // opt_stmt_list: stmt_list
#line 347 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 747 "parser.cpp"
    break;

  case 26: // opt_stmt_list: opt_delimiter_list
#line 351 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 755 "parser.cpp"
    break;

  case 27: // stmt_list: stmt_stmt stmt_list_tail
#line 358 "bnf.y"
            {
                (yylhs.value.node) = astNodeLLConcat(newNodeList((yystack_[1].value.node)), (yystack_[0].value.node));
            }
#line 763 "parser.cpp"
    break;

  case 28: // stmt_list_tail: delimiter_list stmt_stmt stmt_list_tail
#line 365 "bnf.y"
            {
                (yylhs.value.node) = astNodeLLPrepend((yystack_[0].value.node), (yystack_[1].value.node));
            }
#line 771 "parser.cpp"
    break;

  case 29: // stmt_list_tail: %empty
#line 369 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 779 "parser.cpp"
    break;

  case 30: // code_block_or_stmt: code_block
#line 376 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 787 "parser.cpp"
    break;

  case 31: // code_block_or_stmt: opt_delimiter_list stmt
#line 380 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 795 "parser.cpp"
    break;

  case 32: // del_stmt: DEL expr
#line 388 "bnf.y"
            {
                (yylhs.value.node) = newDelStmt((yystack_[0].value.node));
            }
#line 803 "parser.cpp"
    break;

  case 33: // cntrl_cnstrct: loop_whl
#line 396 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 811 "parser.cpp"
    break;

  case 34: // cntrl_cnstrct: loop_for
#line 400 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 819 "parser.cpp"
    break;

  case 35: // cntrl_cnstrct: switch_stmt
#line 404 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 827 "parser.cpp"
    break;

  case 36: // cntrl_cnstrct: ret_stmt
#line 408 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 835 "parser.cpp"
    break;

  case 37: // cntrl_cnstrct: brk_stmt
#line 412 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 843 "parser.cpp"
    break;

  case 38: // cntrl_cnstrct: if_stmt
#line 416 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 851 "parser.cpp"
    break;

  case 39: // cntrl_cnstrct: toss_stmt
#line 420 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 859 "parser.cpp"
    break;

  case 40: // loop_whl: WHL LPAREN expr RPAREN code_block_or_stmt
#line 427 "bnf.y"
            {
                (yylhs.value.node) = newLoopWhl((yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 867 "parser.cpp"
    break;

  case 41: // loop_for: FOR LPAREN loop_for_init COLON loop_for_cond COLON loop_for_post_iter RPAREN code_block_or_stmt
#line 434 "bnf.y"
            {
                (yylhs.value.node) = newLoopFor((yystack_[6].value.node), (yystack_[4].value.node), (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 875 "parser.cpp"
    break;

  case 42: // loop_for_init: opt_expr_list
#line 441 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 883 "parser.cpp"
    break;

  case 43: // loop_for_cond: loop_for_cond COMMA lvl_logic
#line 448 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 892 "parser.cpp"
    break;

  case 44: // loop_for_cond: lvl_logic
#line 453 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 900 "parser.cpp"
    break;

  case 45: // loop_for_post_iter: opt_expr_list
#line 460 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 908 "parser.cpp"
    break;

  case 46: // ret_stmt: RET
#line 467 "bnf.y"
            {
                (yylhs.value.node) = newRetStmt(NULL);
            }
#line 916 "parser.cpp"
    break;

  case 47: // ret_stmt: RET expr
#line 471 "bnf.y"
            {
                (yylhs.value.node) = newRetStmt((yystack_[0].value.node));
            }
#line 924 "parser.cpp"
    break;

  case 48: // switch_stmt: SWITCH LPAREN expr RPAREN LBRACE opt_switch_case_list RBRACE
#line 478 "bnf.y"
            {
                (yylhs.value.node) = newSwitchStmt((yystack_[4].value.node), (yystack_[1].value.node));
            }
#line 932 "parser.cpp"
    break;

  case 49: // opt_switch_case_list: switch_case_list
#line 485 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 940 "parser.cpp"
    break;

  case 50: // opt_switch_case_list: %empty
#line 489 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 948 "parser.cpp"
    break;

  case 51: // switch_case_list: switch_case_list delimiter_list switch_case
#line 496 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 957 "parser.cpp"
    break;

  case 52: // switch_case_list: switch_case
#line 501 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 965 "parser.cpp"
    break;

  case 53: // switch_case: expr COLON code_block_or_stmt
#line 508 "bnf.y"
            {
                (yylhs.value.node) = newSwitchCase((yystack_[2].value.node), (yystack_[0].value.node), false);
            }
#line 973 "parser.cpp"
    break;

  case 54: // switch_case: DEFAULT COLON code_block_or_stmt
#line 512 "bnf.y"
            {
                (yylhs.value.node) = newSwitchCase(NULL, (yystack_[0].value.node), true);
            }
#line 981 "parser.cpp"
    break;

  case 55: // brk_stmt: BRK LPAREN expr RPAREN
#line 519 "bnf.y"
            {
                (yylhs.value.node) = newBrkStmt((yystack_[1].value.node));
            }
#line 989 "parser.cpp"
    break;

  case 56: // brk_stmt: BRK
#line 523 "bnf.y"
            {
                (yylhs.value.node) = newBrkStmt(NULL);
            }
#line 997 "parser.cpp"
    break;

  case 57: // if_stmt: IF LPAREN expr RPAREN code_block_or_stmt opt_else
#line 530 "bnf.y"
            {
                (yylhs.value.node) = newIfStmt((yystack_[3].value.node), (yystack_[1].value.node), (yystack_[0].value.node));
            }
#line 1005 "parser.cpp"
    break;

  case 58: // opt_else: ELSE code_block_or_stmt
#line 537 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1013 "parser.cpp"
    break;

  case 59: // opt_else: %empty
#line 541 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1021 "parser.cpp"
    break;

  case 60: // toss_stmt: TOSS expr
#line 548 "bnf.y"
            {
                (yylhs.value.node) = newTossStmt((yystack_[0].value.node));
            }
#line 1029 "parser.cpp"
    break;

  case 61: // op_assign: EQUALS
#line 556 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1037 "parser.cpp"
    break;

  case 62: // op_assign: ASSIGN_ADD
#line 560 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1045 "parser.cpp"
    break;

  case 63: // op_assign: ASSIGN_SUB
#line 564 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1053 "parser.cpp"
    break;

  case 64: // op_assign: ASSIGN_DIV
#line 568 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1061 "parser.cpp"
    break;

  case 65: // op_assign: ASSIGN_MUL
#line 572 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1069 "parser.cpp"
    break;

  case 66: // op_assign: ASSIGN_MOD
#line 576 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1077 "parser.cpp"
    break;

  case 67: // op_assign: ASSIGN_XOR
#line 580 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1085 "parser.cpp"
    break;

  case 68: // op_assign: ASSIGN_SHIFTL
#line 584 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1093 "parser.cpp"
    break;

  case 69: // op_assign: ASSIGN_SHIFTR
#line 588 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1101 "parser.cpp"
    break;

  case 70: // op_assign: ASSIGN_AND
#line 592 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1109 "parser.cpp"
    break;

  case 71: // op_assign: ASSIGN_OR
#line 596 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1117 "parser.cpp"
    break;

  case 72: // op_logic: LOGIC_AND
#line 603 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1125 "parser.cpp"
    break;

  case 73: // op_logic: LOGIC_OR
#line 607 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1133 "parser.cpp"
    break;

  case 74: // op_cond: COND_GT
#line 614 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1141 "parser.cpp"
    break;

  case 75: // op_cond: COND_GEQ
#line 618 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1149 "parser.cpp"
    break;

  case 76: // op_cond: COND_LT
#line 622 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1157 "parser.cpp"
    break;

  case 77: // op_cond: COND_LEQ
#line 626 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1165 "parser.cpp"
    break;

  case 78: // op_cond: COND_EQ
#line 630 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1173 "parser.cpp"
    break;

  case 79: // op_cond: COND_NEQ
#line 634 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1181 "parser.cpp"
    break;

  case 80: // op_bitwise_xor: BIT_XOR
#line 641 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1189 "parser.cpp"
    break;

  case 81: // op_bitwise_or: BIT_OR
#line 648 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1197 "parser.cpp"
    break;

  case 82: // op_bitwise_and: BIT_AND
#line 655 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1205 "parser.cpp"
    break;

  case 83: // op_bitwise_shift: BIT_SHIFTL
#line 662 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1213 "parser.cpp"
    break;

  case 84: // op_bitwise_shift: BIT_SHIFTR
#line 666 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1221 "parser.cpp"
    break;

  case 85: // op_arithm_add: PLUS
#line 673 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1229 "parser.cpp"
    break;

  case 86: // op_arithm_add: MINUS
#line 677 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1237 "parser.cpp"
    break;

  case 87: // op_arithm_mul: STAR
#line 684 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1245 "parser.cpp"
    break;

  case 88: // op_arithm_mul: SLASH
#line 688 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1253 "parser.cpp"
    break;

  case 89: // op_unary_neg: MINUS
#line 695 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1261 "parser.cpp"
    break;

  case 90: // op_unary_logic_neg: BANG
#line 702 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1269 "parser.cpp"
    break;

  case 91: // op_unary_inc: UNARY_DEC
#line 709 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1277 "parser.cpp"
    break;

  case 92: // op_unary_inc: UNARY_INC
#line 713 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1285 "parser.cpp"
    break;

  case 93: // expr: lvl_logic
#line 721 "bnf.y"
            { 
                (yylhs.value.node) = (yystack_[0].value.node); 
            }
#line 1293 "parser.cpp"
    break;

  case 94: // lvl_logic: lvl_logic op_logic lvl_cond
#line 728 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1301 "parser.cpp"
    break;

  case 95: // lvl_logic: lvl_cond
#line 732 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1309 "parser.cpp"
    break;

  case 96: // lvl_cond: lvl_cond op_cond lvl_bitwise_xor
#line 739 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1317 "parser.cpp"
    break;

  case 97: // lvl_cond: lvl_bitwise_xor
#line 743 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1325 "parser.cpp"
    break;

  case 98: // lvl_bitwise_xor: lvl_bitwise_xor op_bitwise_xor lvl_bitwise_or
#line 750 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1333 "parser.cpp"
    break;

  case 99: // lvl_bitwise_xor: lvl_bitwise_or
#line 754 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1341 "parser.cpp"
    break;

  case 100: // lvl_bitwise_or: lvl_bitwise_or op_bitwise_or lvl_bitwise_and
#line 761 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1349 "parser.cpp"
    break;

  case 101: // lvl_bitwise_or: lvl_bitwise_and
#line 765 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1357 "parser.cpp"
    break;

  case 102: // lvl_bitwise_and: lvl_bitwise_and op_bitwise_and lvl_bitwise_shift
#line 772 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1365 "parser.cpp"
    break;

  case 103: // lvl_bitwise_and: lvl_bitwise_shift
#line 776 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1373 "parser.cpp"
    break;

  case 104: // lvl_bitwise_shift: lvl_bitwise_shift op_bitwise_shift lvl_arithm_add
#line 783 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1381 "parser.cpp"
    break;

  case 105: // lvl_bitwise_shift: lvl_arithm_add
#line 787 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1389 "parser.cpp"
    break;

  case 106: // lvl_arithm_add: lvl_arithm_add op_arithm_add lvl_arithm_mul
#line 794 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1397 "parser.cpp"
    break;

  case 107: // lvl_arithm_add: lvl_arithm_mul
#line 798 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1405 "parser.cpp"
    break;

  case 108: // lvl_arithm_mul: lvl_arithm_mul op_arithm_mul lvl_unary_neg
#line 805 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1413 "parser.cpp"
    break;

  case 109: // lvl_arithm_mul: lvl_unary_neg
#line 809 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1421 "parser.cpp"
    break;

  case 110: // lvl_unary_neg: op_unary_neg lvl_unary_inc
#line 816 "bnf.y"
            {
                (yylhs.value.node) = newUnaryPre((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1429 "parser.cpp"
    break;

  case 111: // lvl_unary_neg: op_unary_logic_neg lvl_unary_inc
#line 820 "bnf.y"
            {
                (yylhs.value.node) = newUnaryPre((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1437 "parser.cpp"
    break;

  case 112: // lvl_unary_neg: lvl_unary_inc
#line 824 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1445 "parser.cpp"
    break;

  case 113: // lvl_unary_inc: op_unary_inc lvl_postfix
#line 831 "bnf.y"
            {
                (yylhs.value.node) = newUnaryPre((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1453 "parser.cpp"
    break;

  case 114: // lvl_unary_inc: lvl_postfix
#line 835 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1461 "parser.cpp"
    break;

  case 115: // lvl_postfix: lvl_postfix DOT nameNode
#line 842 "bnf.y"
            {   
                (yylhs.value.node) = newMemberAccess((yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1469 "parser.cpp"
    break;

  case 116: // lvl_postfix: lvl_postfix LPAREN opt_expr_list RPAREN
#line 846 "bnf.y"
            {   
                (yylhs.value.node) = newFnCall((yystack_[3].value.node), (yystack_[1].value.node));   
            }
#line 1477 "parser.cpp"
    break;

  case 117: // lvl_postfix: lvl_postfix LSQUARE opt_expr RSQUARE
#line 850 "bnf.y"
            {   
                (yylhs.value.node) = newArrayAccess((yystack_[3].value.node), (yystack_[1].value.node));   
            }
#line 1485 "parser.cpp"
    break;

  case 118: // lvl_postfix: lvl_postfix op_unary_inc
#line 854 "bnf.y"
            {   
                (yylhs.value.node) = newUnaryPost((yystack_[0].value.opType), (yystack_[1].value.node)); 
            }
#line 1493 "parser.cpp"
    break;

  case 119: // lvl_postfix: primary
#line 858 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1501 "parser.cpp"
    break;

  case 120: // primary: literal
#line 865 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1509 "parser.cpp"
    break;

  case 121: // primary: nameNode
#line 869 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1517 "parser.cpp"
    break;

  case 122: // primary: LPAREN expr RPAREN
#line 873 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 1525 "parser.cpp"
    break;

  case 123: // opt_expr: expr
#line 880 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1533 "parser.cpp"
    break;

  case 124: // opt_expr: %empty
#line 884 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1541 "parser.cpp"
    break;

  case 125: // opt_expr_list: expr_list
#line 891 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1549 "parser.cpp"
    break;

  case 126: // opt_expr_list: %empty
#line 895 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1557 "parser.cpp"
    break;

  case 127: // expr_list: expr_list COMMA expr
#line 902 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1566 "parser.cpp"
    break;

  case 128: // expr_list: expr
#line 907 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1574 "parser.cpp"
    break;

  case 129: // literal: TRUE
#line 914 "bnf.y"
            { 
                (yylhs.value.node) = newBoolValue(true); 
            }
#line 1582 "parser.cpp"
    break;

  case 130: // literal: FALSE
#line 918 "bnf.y"
            {
                (yylhs.value.node) = newBoolValue(false);
            }
#line 1590 "parser.cpp"
    break;

  case 131: // literal: NUMBER
#line 922 "bnf.y"
            {
                (yylhs.value.node) = newNumberValue((yystack_[0].value.numberValue));
            }
#line 1598 "parser.cpp"
    break;

  case 132: // literal: arr_initializer
#line 926 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1606 "parser.cpp"
    break;

  case 133: // literal: STRING
#line 930 "bnf.y"
            {
                (yylhs.value.node) = newStringValue((yystack_[0].value.stringValue));
            }
#line 1614 "parser.cpp"
    break;

  case 134: // arr_initializer: LSQUARE RSQUARE
#line 938 "bnf.y"
            {
                (yylhs.value.node) = newArray(NULL);
            }
#line 1622 "parser.cpp"
    break;

  case 135: // arr_initializer: LSQUARE expr_list RSQUARE
#line 942 "bnf.y"
            {
                (yylhs.value.node) = newArray((yystack_[1].value.node));
            }
#line 1630 "parser.cpp"
    break;

  case 136: // nameNode: NAME
#line 950 "bnf.y"
            {
                (yylhs.value.node) = newName((yystack_[0].value.stringValue));
            }
#line 1638 "parser.cpp"
    break;

  case 137: // enum_stmt: ENUM nameNode LBRACE enum_opt_elements RBRACE
#line 958 "bnf.y"
            {
                (yylhs.value.node) = newEnumDef((yystack_[3].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1646 "parser.cpp"
    break;

  case 138: // enum_element: nameNode
#line 965 "bnf.y"
            {
                (yylhs.value.node) = newEnumElement((yystack_[0].value.node)->data.stringValue, NULL);
            }
#line 1654 "parser.cpp"
    break;

  case 139: // enum_element: nameNode EQUALS expr
#line 969 "bnf.y"
            {
                (yylhs.value.node) = newEnumElement((yystack_[2].value.node)->data.stringValue, (yystack_[0].value.node));
            }
#line 1662 "parser.cpp"
    break;

  case 140: // enum_opt_elements: enum_elements
#line 976 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1670 "parser.cpp"
    break;

  case 141: // enum_opt_elements: %empty
#line 980 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1678 "parser.cpp"
    break;

  case 142: // enum_elements: enum_elements COMMA enum_element
#line 987 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1687 "parser.cpp"
    break;

  case 143: // enum_elements: enum_element
#line 992 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1695 "parser.cpp"
    break;

  case 144: // fn_parameter: nameNode nameNode
#line 1000 "bnf.y"
            {
                (yylhs.value.node) = newParamInfo((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1703 "parser.cpp"
    break;

  case 145: // fn_parameter: var_type nameNode
#line 1004 "bnf.y"
            {
                (yylhs.value.node) = newParamInfo((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1711 "parser.cpp"
    break;

  case 146: // fn_opt_params_list: fn_params_list
#line 1011 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1719 "parser.cpp"
    break;

  case 147: // fn_opt_params_list: %empty
#line 1015 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1727 "parser.cpp"
    break;

  case 148: // fn_params_list: fn_params_list COMMA fn_parameter
#line 1022 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1736 "parser.cpp"
    break;

  case 149: // fn_params_list: fn_parameter
#line 1027 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1744 "parser.cpp"
    break;

  case 150: // fn_def: FN nameNode LPAREN fn_opt_params_list RPAREN code_block
#line 1034 "bnf.y"
            {
                (yylhs.value.node) = newFnDef((yystack_[4].value.node)->data.stringValue, (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 1752 "parser.cpp"
    break;

  case 151: // var_decl: VAR var_decl_assign_list
#line 1042 "bnf.y"
            {
                (yylhs.value.node) = newVarDeclBatch((yystack_[0].value.node));
            }
#line 1760 "parser.cpp"
    break;

  case 152: // var_decl_assign_list: var_decl_assign_list COMMA var_decl_assign
#line 1049 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1769 "parser.cpp"
    break;

  case 153: // var_decl_assign_list: var_decl_assign
#line 1054 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1777 "parser.cpp"
    break;

  case 154: // var_decl_assign: var_decl_modifiers nameNode EQUALS expr
#line 1061 "bnf.y"
            {
                (yylhs.value.node) = newVarDecl((yystack_[2].value.node)->data.stringValue, (yystack_[3].value.modifier), (yystack_[0].value.node));
            }
#line 1785 "parser.cpp"
    break;

  case 155: // var_decl_assign: var_decl_modifiers nameNode
#line 1065 "bnf.y"
            {
                (yylhs.value.node) = newVarDecl((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.modifier), NULL);
            }
#line 1793 "parser.cpp"
    break;

  case 156: // var_decl_modifiers: var_decl_modifiers var_decl_modifier
#line 1072 "bnf.y"
            {
                (yylhs.value.modifier) = static_cast<Modifier>(to_underlying((yystack_[1].value.modifier)) | to_underlying((yystack_[0].value.modifier)));
            }
#line 1801 "parser.cpp"
    break;

  case 157: // var_decl_modifiers: %empty
#line 1076 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::None;
            }
#line 1809 "parser.cpp"
    break;

  case 158: // var_decl_modifier: CONST
#line 1083 "bnf.y"
            {
                (yylhs.value.modifier) = (yystack_[0].value.modifier);
            }
#line 1817 "parser.cpp"
    break;

  case 159: // var_decl_modifier: LOCAL
#line 1087 "bnf.y"
            {
                (yylhs.value.modifier) = (yystack_[0].value.modifier);
            }
#line 1825 "parser.cpp"
    break;

  case 160: // var_type: VAR
#line 1094 "bnf.y"
            {
                (yylhs.value.node) = newName("var");
            }
#line 1833 "parser.cpp"
    break;

  case 161: // assign: expr op_assign assign_rhs
#line 1102 "bnf.y"
            {
                (yylhs.value.node) = newVarAssign((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 1841 "parser.cpp"
    break;

  case 162: // assign_rhs: expr
#line 1109 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1849 "parser.cpp"
    break;

  case 163: // class_def: CLASS nameNode opt_delimiter_list LBRACE opt_class_member_list RBRACE
#line 1156 "bnf.y"
            {
                (yylhs.value.node) = newClassDef((yystack_[4].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1857 "parser.cpp"
    break;

  case 164: // opt_class_member_list: class_member_list
#line 1163 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1865 "parser.cpp"
    break;

  case 165: // opt_class_member_list: opt_delimiter_list
#line 1167 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1873 "parser.cpp"
    break;

  case 166: // class_member_list: class_member class_member_list_tail
#line 1173 "bnf.y"
            {
                (yylhs.value.node) = astNodeLLConcat(newNodeList((yystack_[1].value.node)), (yystack_[0].value.node));
            }
#line 1881 "parser.cpp"
    break;

  case 167: // class_member_list_tail: delimiter_list class_member class_member_list_tail
#line 1180 "bnf.y"
            {
                (yylhs.value.node) = astNodeLLPrepend((yystack_[0].value.node), (yystack_[1].value.node));
            }
#line 1889 "parser.cpp"
    break;

  case 168: // class_member_list_tail: %empty
#line 1184 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1897 "parser.cpp"
    break;

  case 169: // class_member: class_member_mod_list var_type nameNode
#line 1191 "bnf.y"
            {
                (yylhs.value.node) = newClassFieldDef((yystack_[0].value.node)->data.stringValue, (yystack_[2].value.modifier), (yystack_[1].value.node));
            }
#line 1905 "parser.cpp"
    break;

  case 170: // class_member: class_member_mod_list fn_def
#line 1195 "bnf.y"
            {
                (yylhs.value.node) = newClassMethodDef((yystack_[1].value.modifier), (yystack_[0].value.node));
            }
#line 1913 "parser.cpp"
    break;

  case 171: // class_member_mod_list: class_member_mod_list class_member_mod
#line 1237 "bnf.y"
            {
                (yylhs.value.modifier) = static_cast<Modifier>(static_cast<uint16_t>((yystack_[1].value.modifier)) | static_cast<uint16_t>((yystack_[0].value.modifier)));
            }
#line 1921 "parser.cpp"
    break;

  case 172: // class_member_mod_list: %empty
#line 1241 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::None;
            }
#line 1929 "parser.cpp"
    break;

  case 173: // class_member_mod: HIDDEN
#line 1248 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Hidden;
            }
#line 1937 "parser.cpp"
    break;

  case 174: // class_member_mod: CONST
#line 1252 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Const;
            }
#line 1945 "parser.cpp"
    break;

  case 175: // class_member_mod: STATIC
#line 1256 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Static;
            }
#line 1953 "parser.cpp"
    break;


#line 1957 "parser.cpp"

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









  const short parser::yypact_ninf_ = -198;

  const signed char parser::yytable_ninf_ = -5;

  const short
  parser::yypact_[] =
  {
     -39,  -198,    17,   204,   -25,  -198,  -198,  -198,  -198,  -198,
    -198,  -198,  -198,  -198,  -198,   412,   397,    30,   412,    -2,
       3,    12,    14,    27,    30,   412,   412,    30,  -198,  -198,
    -198,   -39,  -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,
    -198,  -198,  -198,  -198,   436,   436,    18,   444,    -9,    73,
      42,    53,    60,    39,    63,    70,  -198,  -198,     5,  -198,
    -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,    69,
    -198,  -198,   -22,    57,  -198,   412,   412,   412,   412,   412,
     -39,  -198,  -198,    67,    52,  -198,     9,    89,  -198,  -198,
    -198,     5,  -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,
    -198,  -198,  -198,   412,  -198,  -198,   412,  -198,  -198,  -198,
    -198,  -198,  -198,   412,  -198,   412,  -198,   412,  -198,   412,
    -198,  -198,   412,  -198,  -198,   412,  -198,  -198,   412,    30,
     412,   412,  -198,  -198,  -198,   412,     0,    71,    72,    74,
    -198,    68,    77,    79,    80,    30,  -198,  -198,  -198,   110,
    -198,   -39,  -198,  -198,    73,    42,    53,    60,    39,    63,
      70,  -198,  -198,    85,  -198,    81,  -198,  -198,    30,  -198,
      86,    82,    30,   -39,   -39,   412,    90,  -198,    -5,   115,
    -198,    88,    87,  -198,   412,  -198,  -198,  -198,  -198,   -39,
       0,  -198,   232,  -198,    83,  -198,    37,    -9,   384,  -198,
      92,  -198,   -39,   -14,   412,  -198,    30,  -198,    94,  -198,
    -198,   300,  -198,   -39,  -198,   412,   412,    93,   100,   -39,
    -198,   102,  -198,   -25,  -198,  -198,  -198,  -198,  -198,    30,
    -198,  -198,  -198,  -198,   -39,   111,  -198,  -198,   116,  -198,
      -9,   -39,  -198,    28,   -39,   -39,  -198,   316,  -198,  -198,
     -39,  -198,  -198,  -198,  -198,   -39,  -198,  -198
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,     6,     0,     8,     3,     1,   133,   131,   129,   130,
     136,    89,    90,    91,    92,     0,     0,     0,    46,     0,
       0,     0,     0,    56,     0,     0,     0,     0,   157,     2,
       7,    12,    14,    13,    18,    21,    20,    33,    34,    36,
      35,    37,    38,    39,     0,     0,     0,    23,    93,    95,
      97,    99,   101,   103,   105,   107,   109,   112,   114,   119,
     120,   132,   121,    17,    16,    19,    22,    15,     5,     0,
     134,   128,     0,     0,    47,     0,     0,   126,     0,     0,
       4,    60,    32,     0,   151,   153,     0,    11,     9,   110,
     111,   113,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,     0,    72,    73,     0,    74,    75,    76,
      77,    78,    79,     0,    80,     0,    81,     0,    82,     0,
      83,    84,     0,    86,    85,     0,    87,    88,     0,     0,
     126,   124,   118,   122,   135,     0,   147,     0,     0,     0,
      42,   125,     0,     0,     0,   141,   157,   158,   159,   155,
     156,    12,   162,   161,    94,    96,    98,   100,   102,   104,
     106,   108,   115,     0,   123,     0,   127,   160,     0,   149,
       0,   146,     0,     4,     4,     0,     0,    55,   172,   138,
     143,     0,   140,   152,     0,    10,   116,   117,   144,     4,
       0,   145,     0,    30,    59,    40,     0,    44,    50,   165,
       0,   164,   168,     0,     0,   137,     0,   154,     0,   150,
     148,     4,    31,     4,    57,   126,     0,     0,     0,    49,
      52,     0,   163,   172,   166,   174,   175,   173,   170,     0,
     171,   139,   142,    26,    29,     0,    25,    58,     0,    45,
      43,     4,    48,     0,     4,   168,   169,     0,    27,    24,
       4,    54,    51,    53,   167,    29,    41,    28
  };

  const short
  parser::yypgoto_[] =
  {
    -198,  -198,     4,   -29,  -198,  -198,    10,    75,  -198,   -28,
    -197,   -26,  -198,  -198,   -90,  -165,  -198,  -198,  -198,  -198,
    -198,  -198,  -198,  -198,  -198,  -198,  -198,   -77,  -198,  -198,
    -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,  -198,
    -198,  -198,  -198,   -40,   -15,  -167,    61,    55,    59,    54,
      51,    50,    62,    47,    56,   130,  -198,  -198,  -124,   161,
    -198,  -198,   -12,  -198,   -27,  -198,  -198,   -10,  -198,  -198,
     -19,  -198,  -198,    35,  -198,  -198,   -18,  -198,  -198,  -198,
    -198,  -198,   -59,   -35,  -198,  -198
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     2,   192,     4,    29,    30,    88,    31,    32,    33,
      34,   193,   235,   236,   248,   194,    35,    36,    37,    38,
     139,   196,   238,    39,    40,   218,   219,   220,    41,    42,
     214,    43,   103,   106,   113,   115,   117,   119,   122,   125,
     128,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,   165,   140,   141,
      60,    61,    62,    63,   180,   181,   182,   169,   170,   171,
      64,    65,    84,    85,    86,   150,   172,    66,   153,    67,
     200,   201,   224,   202,   203,   230
  };

  const short
  parser::yytable_[] =
  {
      69,    71,    87,    74,     3,    73,   163,    10,   197,   195,
      81,    82,    80,     1,   234,    83,    10,     5,   132,    13,
      14,     6,     7,     8,     9,    10,   134,    68,   135,   104,
     105,     6,     7,     8,     9,    10,    11,    10,    12,    75,
      17,    -4,    13,    14,    76,   129,   130,     1,   237,   240,
     255,   132,   131,    77,   225,    78,   167,   226,   227,    15,
     137,   138,    71,   142,   143,    16,   120,   121,    79,    15,
     167,   123,   124,   114,   149,    16,   251,   147,   148,   253,
      68,   126,   127,   116,   144,   256,   215,   216,   152,   118,
     217,   239,     6,     7,     8,     9,    10,    11,   136,    12,
      89,    90,   146,    13,    14,   107,   108,   109,   110,   111,
     112,   133,   145,   173,   174,    71,   164,   162,   135,   176,
     166,   177,    87,   175,   168,   178,   184,   186,   189,   187,
      15,   204,   190,   179,   205,   198,    16,   206,   222,   211,
     213,    68,   241,    17,    18,    19,   242,    20,    21,    22,
      23,   244,    24,    25,    26,    27,   188,   249,   250,    28,
     191,   185,   151,   209,   212,   257,   252,   154,   155,   207,
     158,   157,   159,   223,   156,   161,    91,    72,   168,   232,
     210,   183,   199,   221,   228,   229,   254,   160,   245,   231,
     243,     0,     0,   208,   179,     0,     0,     0,     0,     0,
      71,     0,     0,     0,     0,   247,     0,     6,     7,     8,
       9,    10,    11,     0,    12,   233,   223,   246,    13,    14,
       0,     0,     0,     0,     0,     0,   247,     0,   221,     0,
       0,     0,     0,     0,     0,     6,     7,     8,     9,    10,
      11,     0,    12,     0,     0,    15,    13,    14,     0,     0,
       0,    16,     0,     0,     0,     0,     0,     0,    17,    18,
      19,     0,    20,    21,    22,    23,     0,    24,    25,    26,
      27,     0,     0,    15,    28,     0,     0,   211,     0,    16,
       0,     0,     0,     0,     0,     0,     0,    18,    19,     0,
      20,    21,    22,    23,     0,     0,    25,    26,     0,     0,
       0,     0,    28,     6,     7,     8,     9,    10,    11,     0,
      12,     0,     0,     0,    13,    14,     0,     0,     0,     6,
       7,     8,     9,    10,    11,     0,    12,     0,     0,     0,
      13,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    15,     0,     0,     0,     0,     0,    16,     0,     0,
       0,     0,     1,     0,     0,    18,    19,    15,    20,    21,
      22,    23,     0,    16,    25,    26,     0,     0,    68,     0,
      28,    18,    19,     0,    20,    21,    22,    23,     0,     0,
      25,    26,     0,     0,     0,     0,    28,     6,     7,     8,
       9,    10,    11,     0,    12,     0,     0,     0,    13,    14,
       6,     7,     8,     9,    10,    11,     0,    12,     0,     0,
       0,    13,    14,     0,     0,     6,     7,     8,     9,    10,
      11,     0,    12,     0,     0,    15,    13,    14,     0,     0,
       0,    16,     0,     0,     0,     0,     0,     0,    15,     6,
       7,     8,     9,    10,    16,    70,   217,     0,     0,     0,
      13,    14,     0,    15,     0,     0,     0,     0,     0,    16,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,     0,     0,     0,     0,     0,     0,    15,     0,     0,
       0,     0,     0,    16
  };

  const short
  parser::yycheck_[] =
  {
      15,    16,    31,    18,     0,    17,   130,     7,   175,   174,
      25,    26,    24,    52,   211,    27,     7,     0,    58,    14,
      15,     3,     4,     5,     6,     7,    48,    52,    50,    38,
      39,     3,     4,     5,     6,     7,     8,     7,    10,    41,
      54,    46,    14,    15,    41,    40,    41,    52,   213,   216,
     247,    91,    47,    41,    68,    41,    70,    71,    72,    41,
      75,    76,    77,    78,    79,    47,    27,    28,    41,    41,
      70,     8,     9,    31,    86,    47,   241,    68,    69,   244,
      52,    11,    12,    30,    80,   250,    49,    50,   103,    29,
      62,   215,     3,     4,     5,     6,     7,     8,    41,    10,
      44,    45,    50,    14,    15,    32,    33,    34,    35,    36,
      37,    42,    45,    42,    42,   130,   131,   129,    50,    42,
     135,    42,   151,    49,   136,    45,    16,    42,    42,    48,
      41,    16,    50,   145,    46,    45,    47,    50,    46,    45,
      57,    52,    49,    54,    55,    56,    46,    58,    59,    60,
      61,    49,    63,    64,    65,    66,   168,    46,    42,    70,
     172,   151,    87,   189,   192,   255,   243,   106,   113,   184,
     119,   117,   122,   202,   115,   128,    46,    16,   190,   206,
     190,   146,   178,   198,   203,   203,   245,   125,   223,   204,
     219,    -1,    -1,   189,   206,    -1,    -1,    -1,    -1,    -1,
     215,    -1,    -1,    -1,    -1,   234,    -1,     3,     4,     5,
       6,     7,     8,    -1,    10,   211,   245,   229,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,   255,    -1,   243,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    -1,    41,    14,    15,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    -1,    58,    59,    60,    61,    -1,    63,    64,    65,
      66,    -1,    -1,    41,    70,    -1,    -1,    45,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    -1,
      58,    59,    60,    61,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    70,     3,     4,     5,     6,     7,     8,    -1,
      10,    -1,    -1,    -1,    14,    15,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    -1,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    52,    -1,    -1,    55,    56,    41,    58,    59,
      60,    61,    -1,    47,    64,    65,    -1,    -1,    52,    -1,
      70,    55,    56,    -1,    58,    59,    60,    61,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    70,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    -1,    -1,    14,    15,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    -1,
      -1,    14,    15,    -1,    -1,     3,     4,     5,     6,     7,
       8,    -1,    10,    -1,    -1,    41,    14,    15,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    41,     3,
       4,     5,     6,     7,    47,    48,    62,    -1,    -1,    -1,
      14,    15,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    47
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    52,    76,    77,    78,     0,     3,     4,     5,     6,
       7,     8,    10,    14,    15,    41,    47,    54,    55,    56,
      58,    59,    60,    61,    63,    64,    65,    66,    70,    79,
      80,    82,    83,    84,    85,    91,    92,    93,    94,    98,
      99,   103,   104,   106,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     135,   136,   137,   138,   145,   146,   152,   154,    52,   119,
      48,   119,   134,   137,   119,    41,    41,    41,    41,    41,
     137,   119,   119,   137,   147,   148,   149,    78,    81,   129,
     129,   130,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   107,    38,    39,   108,    32,    33,    34,
      35,    36,    37,   109,    31,   110,    30,   111,    29,   112,
      27,    28,   113,     8,     9,   114,    11,    12,   115,    40,
      41,    47,   118,    42,    48,    50,    41,   119,   119,    95,
     133,   134,   119,   119,    77,    45,    50,    68,    69,   137,
     150,    82,   119,   153,   121,   122,   123,   124,   125,   126,
     127,   128,   137,   133,   119,   132,   119,    70,   137,   142,
     143,   144,   151,    42,    42,    49,    42,    42,    45,   137,
     139,   140,   141,   148,    16,    81,    42,    48,   137,    42,
      50,   137,    77,    86,    90,    90,    96,   120,    45,    77,
     155,   156,   158,   159,    16,    46,    50,   119,    77,    86,
     142,    45,    84,    57,   105,    49,    50,    62,   100,   101,
     102,   119,    46,    78,   157,    68,    71,    72,   145,   151,
     160,   119,   139,    77,    85,    87,    88,    90,    97,   133,
     120,    49,    46,    78,    49,   158,   137,    78,    89,    46,
      42,    90,   102,    90,   157,    85,    90,    89
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    75,    76,    77,    77,    78,    78,    79,    79,    80,
      81,    81,    81,    82,    82,    83,    83,    83,    84,    85,
      85,    85,    85,    85,    86,    87,    87,    88,    89,    89,
      90,    90,    91,    92,    92,    92,    92,    92,    92,    92,
      93,    94,    95,    96,    96,    97,    98,    98,    99,   100,
     100,   101,   101,   102,   102,   103,   103,   104,   105,   105,
     106,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   108,   108,   109,   109,   109,   109,   109,   109,
     110,   111,   112,   113,   113,   114,   114,   115,   115,   116,
     117,   118,   118,   119,   120,   120,   121,   121,   122,   122,
     123,   123,   124,   124,   125,   125,   126,   126,   127,   127,
     128,   128,   128,   129,   129,   130,   130,   130,   130,   130,
     131,   131,   131,   132,   132,   133,   133,   134,   134,   135,
     135,   135,   135,   135,   136,   136,   137,   138,   139,   139,
     140,   140,   141,   141,   142,   142,   143,   143,   144,   144,
     145,   146,   147,   147,   148,   148,   149,   149,   150,   150,
     151,   152,   153,   154,   155,   155,   156,   157,   157,   158,
     158,   159,   159,   160,   160,   160
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     1,     0,     2,     1,     1,     0,     2,
       3,     1,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     1,     2,     3,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       5,     9,     1,     3,     1,     1,     1,     2,     7,     1,
       0,     3,     1,     3,     3,     4,     1,     6,     2,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       2,     2,     1,     2,     1,     3,     4,     4,     2,     1,
       1,     1,     3,     1,     0,     1,     0,     3,     1,     1,
       1,     1,     1,     1,     2,     3,     1,     5,     1,     3,
       1,     0,     3,     1,     2,     2,     1,     0,     3,     1,
       6,     2,     3,     1,     4,     2,     2,     0,     1,     1,
       1,     3,     1,     6,     1,     1,     2,     3,     0,     3,
       2,     2,     0,     1,     1,     1
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
  "program_stmt_list_tail", "program_stmt", "global_stmt", "stmt",
  "stmt_stmt", "code_block", "opt_stmt_list", "stmt_list",
  "stmt_list_tail", "code_block_or_stmt", "del_stmt", "cntrl_cnstrct",
  "loop_whl", "loop_for", "loop_for_init", "loop_for_cond",
  "loop_for_post_iter", "ret_stmt", "switch_stmt", "opt_switch_case_list",
  "switch_case_list", "switch_case", "brk_stmt", "if_stmt", "opt_else",
  "toss_stmt", "op_assign", "op_logic", "op_cond", "op_bitwise_xor",
  "op_bitwise_or", "op_bitwise_and", "op_bitwise_shift", "op_arithm_add",
  "op_arithm_mul", "op_unary_neg", "op_unary_logic_neg", "op_unary_inc",
  "expr", "lvl_logic", "lvl_cond", "lvl_bitwise_xor", "lvl_bitwise_or",
  "lvl_bitwise_and", "lvl_bitwise_shift", "lvl_arithm_add",
  "lvl_arithm_mul", "lvl_unary_neg", "lvl_unary_inc", "lvl_postfix",
  "primary", "opt_expr", "opt_expr_list", "expr_list", "literal",
  "arr_initializer", "nameNode", "enum_stmt", "enum_element",
  "enum_opt_elements", "enum_elements", "fn_parameter",
  "fn_opt_params_list", "fn_params_list", "fn_def", "var_decl",
  "var_decl_assign_list", "var_decl_assign", "var_decl_modifiers",
  "var_decl_modifier", "var_type", "assign", "assign_rhs", "class_def",
  "opt_class_member_list", "class_member_list", "class_member_list_tail",
  "class_member", "class_member_mod_list", "class_member_mod", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   230,   230,   237,   238,   242,   243,   247,   252,   258,
     265,   269,   274,   280,   284,   292,   296,   300,   308,   315,
     319,   323,   327,   331,   339,   346,   350,   357,   364,   369,
     375,   379,   387,   395,   399,   403,   407,   411,   415,   419,
     426,   433,   440,   447,   452,   459,   466,   470,   477,   484,
     489,   495,   500,   507,   511,   518,   522,   529,   536,   540,
     547,   555,   559,   563,   567,   571,   575,   579,   583,   587,
     591,   595,   602,   606,   613,   617,   621,   625,   629,   633,
     640,   647,   654,   661,   665,   672,   676,   683,   687,   694,
     701,   708,   712,   720,   727,   731,   738,   742,   749,   753,
     760,   764,   771,   775,   782,   786,   793,   797,   804,   808,
     815,   819,   823,   830,   834,   841,   845,   849,   853,   857,
     864,   868,   872,   879,   884,   890,   895,   901,   906,   913,
     917,   921,   925,   929,   937,   941,   949,   957,   964,   968,
     975,   980,   986,   991,   999,  1003,  1010,  1015,  1021,  1026,
    1033,  1041,  1048,  1053,  1060,  1064,  1071,  1076,  1082,  1086,
    1093,  1101,  1108,  1155,  1162,  1166,  1172,  1179,  1184,  1190,
    1194,  1236,  1241,  1247,  1251,  1255
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
#line 2575 "parser.cpp"

#line 1261 "bnf.y"
