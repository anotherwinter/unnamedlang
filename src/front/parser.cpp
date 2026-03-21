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
#line 14 "bnf.y"

#include <stdio.h>
#include "bison_wrapper.h"

ASTNode* astRoot = NULL;

#line 48 "parser.cpp"


#include "parser.hpp"




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
  parser::parser (Lexer& lexer_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg)
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
#line 217 "bnf.y"
            {
                astRoot = (yystack_[0].value.node);
            }
#line 585 "parser.cpp"
    break;

  case 7: // opt_program_stmt_list: program_stmt_list opt_delimiter_list
#line 234 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 593 "parser.cpp"
    break;

  case 8: // opt_program_stmt_list: opt_delimiter_list
#line 238 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 601 "parser.cpp"
    break;

  case 9: // program_stmt_list: program_stmt_list delimiter_list program_stmt
#line 245 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node)->data.nodeList.list, (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 610 "parser.cpp"
    break;

  case 10: // program_stmt_list: program_stmt
#line 250 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 618 "parser.cpp"
    break;

  case 11: // program_stmt: stmt
#line 257 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 626 "parser.cpp"
    break;

  case 12: // program_stmt: global_stmt
#line 261 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 634 "parser.cpp"
    break;

  case 13: // global_stmt: global_stmt_stmt
#line 269 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 642 "parser.cpp"
    break;

  case 14: // global_stmt_stmt: class_def
#line 276 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 650 "parser.cpp"
    break;

  case 15: // global_stmt_stmt: fn_def
#line 280 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 658 "parser.cpp"
    break;

  case 16: // global_stmt_stmt: enum_stmt
#line 284 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 666 "parser.cpp"
    break;

  case 17: // stmt: stmt_stmt
#line 292 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 674 "parser.cpp"
    break;

  case 18: // stmt_stmt: var_decl
#line 299 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 682 "parser.cpp"
    break;

  case 19: // stmt_stmt: cntrl_cnstrct
#line 303 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 690 "parser.cpp"
    break;

  case 20: // stmt_stmt: del_stmt
#line 307 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 698 "parser.cpp"
    break;

  case 21: // stmt_stmt: assign
#line 311 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 706 "parser.cpp"
    break;

  case 22: // stmt_stmt: expr
#line 315 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 714 "parser.cpp"
    break;

  case 23: // code_block: opt_delimiter_list LBRACE opt_delimiter_list opt_stmt_list RBRACE
#line 323 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 722 "parser.cpp"
    break;

  case 24: // opt_stmt_list: stmt_list
#line 330 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 730 "parser.cpp"
    break;

  case 25: // opt_stmt_list: %empty
#line 334 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 738 "parser.cpp"
    break;

  case 26: // stmt_list: stmt_list delimiter_list stmt_stmt
#line 341 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node)->data.nodeList.list, (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 747 "parser.cpp"
    break;

  case 27: // stmt_list: stmt_stmt
#line 346 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 755 "parser.cpp"
    break;

  case 28: // code_block_or_stmt: code_block
#line 353 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 763 "parser.cpp"
    break;

  case 29: // code_block_or_stmt: opt_delimiter_list stmt
#line 357 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 771 "parser.cpp"
    break;

  case 30: // del_stmt: DEL expr
#line 365 "bnf.y"
            {
                (yylhs.value.node) = newDelStmt((yystack_[0].value.node));
            }
#line 779 "parser.cpp"
    break;

  case 31: // cntrl_cnstrct: loop_whl
#line 373 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 787 "parser.cpp"
    break;

  case 32: // cntrl_cnstrct: loop_for
#line 377 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 795 "parser.cpp"
    break;

  case 33: // cntrl_cnstrct: switch_stmt
#line 381 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 803 "parser.cpp"
    break;

  case 34: // cntrl_cnstrct: ret_stmt
#line 385 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 811 "parser.cpp"
    break;

  case 35: // cntrl_cnstrct: brk_stmt
#line 389 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 819 "parser.cpp"
    break;

  case 36: // cntrl_cnstrct: if_stmt
#line 393 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 827 "parser.cpp"
    break;

  case 37: // cntrl_cnstrct: toss_stmt
#line 397 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 835 "parser.cpp"
    break;

  case 38: // loop_whl: WHL LPAREN expr RPAREN code_block_or_stmt
#line 404 "bnf.y"
            {
                (yylhs.value.node) = newLoopWhl((yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 843 "parser.cpp"
    break;

  case 39: // loop_for: FOR LPAREN loop_for_init COLON loop_for_cond COLON loop_for_post_iter RPAREN code_block_or_stmt
#line 411 "bnf.y"
            {
                (yylhs.value.node) = newLoopFor((yystack_[6].value.node), (yystack_[4].value.node), (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 851 "parser.cpp"
    break;

  case 40: // loop_for_init: opt_expr_list
#line 418 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 859 "parser.cpp"
    break;

  case 41: // loop_for_cond: loop_for_cond COMMA lvl_logic
#line 425 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node)->data.nodeList.list, (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 868 "parser.cpp"
    break;

  case 42: // loop_for_cond: lvl_logic
#line 430 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 876 "parser.cpp"
    break;

  case 43: // loop_for_post_iter: opt_expr_list
#line 437 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 884 "parser.cpp"
    break;

  case 44: // ret_stmt: RET
#line 444 "bnf.y"
            {
                (yylhs.value.node) = newRetStmt(NULL);
            }
#line 892 "parser.cpp"
    break;

  case 45: // ret_stmt: RET expr
#line 448 "bnf.y"
            {
                (yylhs.value.node) = newRetStmt((yystack_[0].value.node));
            }
#line 900 "parser.cpp"
    break;

  case 46: // switch_stmt: SWITCH LPAREN expr RPAREN LBRACE opt_switch_case_list opt_delimiter_list RBRACE
#line 455 "bnf.y"
            {
                (yylhs.value.node) = newSwitchStmt((yystack_[5].value.node), (yystack_[2].value.node));
            }
#line 908 "parser.cpp"
    break;

  case 47: // opt_switch_case_list: switch_case_list
#line 462 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 916 "parser.cpp"
    break;

  case 48: // opt_switch_case_list: %empty
#line 466 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 924 "parser.cpp"
    break;

  case 49: // switch_case_list: switch_case_list delimiter_list switch_case
#line 473 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node)->data.nodeList.list, (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 933 "parser.cpp"
    break;

  case 50: // switch_case_list: switch_case
#line 478 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 941 "parser.cpp"
    break;

  case 51: // switch_case: expr COLON code_block_or_stmt
#line 485 "bnf.y"
            {
                (yylhs.value.node) = newSwitchCase((yystack_[2].value.node), (yystack_[0].value.node), false);
            }
#line 949 "parser.cpp"
    break;

  case 52: // switch_case: DEFAULT COLON code_block_or_stmt
#line 489 "bnf.y"
            {
                (yylhs.value.node) = newSwitchCase(NULL, (yystack_[0].value.node), true);
            }
#line 957 "parser.cpp"
    break;

  case 53: // brk_stmt: BRK LPAREN expr RPAREN
#line 496 "bnf.y"
            {
                (yylhs.value.node) = newBrkStmt((yystack_[1].value.node));
            }
#line 965 "parser.cpp"
    break;

  case 54: // brk_stmt: BRK
#line 500 "bnf.y"
            {
                (yylhs.value.node) = newBrkStmt(NULL);
            }
#line 973 "parser.cpp"
    break;

  case 55: // if_stmt: IF LPAREN expr RPAREN code_block_or_stmt opt_else
#line 507 "bnf.y"
            {
                (yylhs.value.node) = newIfStmt((yystack_[3].value.node), (yystack_[1].value.node), (yystack_[0].value.node));
            }
#line 981 "parser.cpp"
    break;

  case 56: // opt_else: ELSE code_block_or_stmt
#line 514 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 989 "parser.cpp"
    break;

  case 57: // opt_else: %empty
#line 518 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 997 "parser.cpp"
    break;

  case 58: // toss_stmt: TOSS expr
#line 525 "bnf.y"
            {
                (yylhs.value.node) = newTossStmt((yystack_[0].value.node));
            }
#line 1005 "parser.cpp"
    break;

  case 59: // op_assign: EQUALS
#line 533 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1013 "parser.cpp"
    break;

  case 60: // op_assign: ASSIGN_ADD
#line 537 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1021 "parser.cpp"
    break;

  case 61: // op_assign: ASSIGN_SUB
#line 541 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1029 "parser.cpp"
    break;

  case 62: // op_assign: ASSIGN_DIV
#line 545 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1037 "parser.cpp"
    break;

  case 63: // op_assign: ASSIGN_MUL
#line 549 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1045 "parser.cpp"
    break;

  case 64: // op_assign: ASSIGN_MOD
#line 553 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1053 "parser.cpp"
    break;

  case 65: // op_assign: ASSIGN_XOR
#line 557 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1061 "parser.cpp"
    break;

  case 66: // op_assign: ASSIGN_SHIFTL
#line 561 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1069 "parser.cpp"
    break;

  case 67: // op_assign: ASSIGN_SHIFTR
#line 565 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1077 "parser.cpp"
    break;

  case 68: // op_assign: ASSIGN_AND
#line 569 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1085 "parser.cpp"
    break;

  case 69: // op_assign: ASSIGN_OR
#line 573 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1093 "parser.cpp"
    break;

  case 70: // op_logic: LOGIC_AND
#line 580 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1101 "parser.cpp"
    break;

  case 71: // op_logic: LOGIC_OR
#line 584 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1109 "parser.cpp"
    break;

  case 72: // op_cond: COND_GT
#line 591 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1117 "parser.cpp"
    break;

  case 73: // op_cond: COND_GEQ
#line 595 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1125 "parser.cpp"
    break;

  case 74: // op_cond: COND_LT
#line 599 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1133 "parser.cpp"
    break;

  case 75: // op_cond: COND_LEQ
#line 603 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1141 "parser.cpp"
    break;

  case 76: // op_cond: COND_EQ
#line 607 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1149 "parser.cpp"
    break;

  case 77: // op_cond: COND_NEQ
#line 611 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1157 "parser.cpp"
    break;

  case 78: // op_bitwise_xor: BIT_XOR
#line 618 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1165 "parser.cpp"
    break;

  case 79: // op_bitwise_or: BIT_OR
#line 625 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1173 "parser.cpp"
    break;

  case 80: // op_bitwise_and: BIT_AND
#line 632 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1181 "parser.cpp"
    break;

  case 81: // op_bitwise_shift: BIT_SHIFTL
#line 639 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1189 "parser.cpp"
    break;

  case 82: // op_bitwise_shift: BIT_SHIFTR
#line 643 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1197 "parser.cpp"
    break;

  case 83: // op_arithm_add: PLUS
#line 650 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1205 "parser.cpp"
    break;

  case 84: // op_arithm_add: MINUS
#line 654 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1213 "parser.cpp"
    break;

  case 85: // op_arithm_mul: STAR
#line 661 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1221 "parser.cpp"
    break;

  case 86: // op_arithm_mul: SLASH
#line 665 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1229 "parser.cpp"
    break;

  case 87: // op_unary_neg: MINUS
#line 672 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1237 "parser.cpp"
    break;

  case 88: // op_unary_logic_neg: BANG
#line 679 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1245 "parser.cpp"
    break;

  case 89: // op_unary_inc: UNARY_DEC
#line 686 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1253 "parser.cpp"
    break;

  case 90: // op_unary_inc: UNARY_INC
#line 690 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1261 "parser.cpp"
    break;

  case 91: // expr: lvl_logic
#line 698 "bnf.y"
            { 
                (yylhs.value.node) = (yystack_[0].value.node); 
            }
#line 1269 "parser.cpp"
    break;

  case 92: // lvl_logic: lvl_logic op_logic lvl_cond
#line 705 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1277 "parser.cpp"
    break;

  case 93: // lvl_logic: lvl_cond
#line 709 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1285 "parser.cpp"
    break;

  case 94: // lvl_cond: lvl_cond op_cond lvl_bitwise_xor
#line 716 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1293 "parser.cpp"
    break;

  case 95: // lvl_cond: lvl_bitwise_xor
#line 720 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1301 "parser.cpp"
    break;

  case 96: // lvl_bitwise_xor: lvl_bitwise_xor op_bitwise_xor lvl_bitwise_or
#line 727 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1309 "parser.cpp"
    break;

  case 97: // lvl_bitwise_xor: lvl_bitwise_or
#line 731 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1317 "parser.cpp"
    break;

  case 98: // lvl_bitwise_or: lvl_bitwise_or op_bitwise_or lvl_bitwise_and
#line 738 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1325 "parser.cpp"
    break;

  case 99: // lvl_bitwise_or: lvl_bitwise_and
#line 742 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1333 "parser.cpp"
    break;

  case 100: // lvl_bitwise_and: lvl_bitwise_and op_bitwise_and lvl_bitwise_shift
#line 749 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1341 "parser.cpp"
    break;

  case 101: // lvl_bitwise_and: lvl_bitwise_shift
#line 753 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1349 "parser.cpp"
    break;

  case 102: // lvl_bitwise_shift: lvl_bitwise_shift op_bitwise_shift lvl_arithm_add
#line 760 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1357 "parser.cpp"
    break;

  case 103: // lvl_bitwise_shift: lvl_arithm_add
#line 764 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1365 "parser.cpp"
    break;

  case 104: // lvl_arithm_add: lvl_arithm_add op_arithm_add lvl_arithm_mul
#line 771 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1373 "parser.cpp"
    break;

  case 105: // lvl_arithm_add: lvl_arithm_mul
#line 775 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1381 "parser.cpp"
    break;

  case 106: // lvl_arithm_mul: lvl_arithm_mul op_arithm_mul lvl_unary_neg
#line 782 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1389 "parser.cpp"
    break;

  case 107: // lvl_arithm_mul: lvl_unary_neg
#line 786 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1397 "parser.cpp"
    break;

  case 108: // lvl_unary_neg: op_unary_neg lvl_unary_inc
#line 793 "bnf.y"
            {
                (yylhs.value.node) = newUnaryOp((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1405 "parser.cpp"
    break;

  case 109: // lvl_unary_neg: op_unary_logic_neg lvl_unary_inc
#line 797 "bnf.y"
            {
                (yylhs.value.node) = newUnaryOp((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1413 "parser.cpp"
    break;

  case 110: // lvl_unary_neg: lvl_unary_inc
#line 801 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1421 "parser.cpp"
    break;

  case 111: // lvl_unary_inc: op_unary_inc lvl_postfix
#line 808 "bnf.y"
            {
                (yylhs.value.node) = newUnaryOp((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1429 "parser.cpp"
    break;

  case 112: // lvl_unary_inc: lvl_postfix
#line 812 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1437 "parser.cpp"
    break;

  case 113: // lvl_postfix: lvl_postfix DOT nameNode
#line 819 "bnf.y"
            {   
                (yylhs.value.node) = newMemberAccess((yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1445 "parser.cpp"
    break;

  case 114: // lvl_postfix: lvl_postfix LPAREN opt_expr_list RPAREN
#line 823 "bnf.y"
            {   
                (yylhs.value.node) = newFnCall((yystack_[3].value.node), (yystack_[1].value.node));   
            }
#line 1453 "parser.cpp"
    break;

  case 115: // lvl_postfix: lvl_postfix LSQUARE opt_expr RSQUARE
#line 827 "bnf.y"
            {   
                (yylhs.value.node) = newArrayAccess((yystack_[3].value.node), (yystack_[1].value.node));   
            }
#line 1461 "parser.cpp"
    break;

  case 116: // lvl_postfix: primary
#line 831 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1469 "parser.cpp"
    break;

  case 117: // primary: literal
#line 838 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1477 "parser.cpp"
    break;

  case 118: // primary: nameNode
#line 842 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1485 "parser.cpp"
    break;

  case 119: // primary: LPAREN expr RPAREN
#line 846 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 1493 "parser.cpp"
    break;

  case 120: // opt_expr: expr
#line 853 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1501 "parser.cpp"
    break;

  case 121: // opt_expr: %empty
#line 857 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1509 "parser.cpp"
    break;

  case 122: // opt_expr_list: expr_list
#line 864 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1517 "parser.cpp"
    break;

  case 123: // opt_expr_list: %empty
#line 868 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1525 "parser.cpp"
    break;

  case 124: // expr_list: expr_list COMMA expr
#line 875 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node)->data.nodeList.list, (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1534 "parser.cpp"
    break;

  case 125: // expr_list: expr
#line 880 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1542 "parser.cpp"
    break;

  case 126: // literal: TRUE
#line 887 "bnf.y"
            { 
                (yylhs.value.node) = newBoolValue(true); 
            }
#line 1550 "parser.cpp"
    break;

  case 127: // literal: FALSE
#line 891 "bnf.y"
            {
                (yylhs.value.node) = newBoolValue(false);
            }
#line 1558 "parser.cpp"
    break;

  case 128: // literal: NUMBER
#line 895 "bnf.y"
            {
                (yylhs.value.node) = newNumberValue((yystack_[0].value.numberValue));
            }
#line 1566 "parser.cpp"
    break;

  case 129: // literal: arr_initializer
#line 899 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1574 "parser.cpp"
    break;

  case 130: // literal: STRING
#line 903 "bnf.y"
            {
                (yylhs.value.node) = newStringValue((yystack_[0].value.stringValue));
            }
#line 1582 "parser.cpp"
    break;

  case 131: // arr_initializer: LSQUARE RSQUARE
#line 911 "bnf.y"
            {
                (yylhs.value.node) = newArray(NULL);
            }
#line 1590 "parser.cpp"
    break;

  case 132: // arr_initializer: LSQUARE expr_list RSQUARE
#line 915 "bnf.y"
            {
                (yylhs.value.node) = newArray((yystack_[1].value.node));
            }
#line 1598 "parser.cpp"
    break;

  case 133: // nameNode: NAME
#line 923 "bnf.y"
            {
                (yylhs.value.node) = newName((yystack_[0].value.stringValue));
            }
#line 1606 "parser.cpp"
    break;

  case 134: // enum_stmt: ENUM nameNode LBRACE enum_opt_elements RBRACE
#line 931 "bnf.y"
            {
                (yylhs.value.node) = newEnumDef((yystack_[3].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1614 "parser.cpp"
    break;

  case 135: // enum_element: delimiter_list nameNode
#line 938 "bnf.y"
            {
                (yylhs.value.node) = newEnumElement((yystack_[0].value.node)->data.stringValue, NULL);
            }
#line 1622 "parser.cpp"
    break;

  case 136: // enum_element: delimiter_list nameNode EQUALS expr
#line 942 "bnf.y"
            {
                (yylhs.value.node) = newEnumElement((yystack_[2].value.node)->data.stringValue, (yystack_[0].value.node));
            }
#line 1630 "parser.cpp"
    break;

  case 137: // enum_opt_elements: enum_elements
#line 949 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1638 "parser.cpp"
    break;

  case 138: // enum_opt_elements: %empty
#line 953 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1646 "parser.cpp"
    break;

  case 139: // enum_elements: enum_elements COMMA enum_element
#line 960 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node)->data.nodeList.list, (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1655 "parser.cpp"
    break;

  case 140: // enum_elements: enum_element
#line 965 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1663 "parser.cpp"
    break;

  case 141: // fn_parameter: nameNode nameNode
#line 973 "bnf.y"
            {
                (yylhs.value.node) = newParamInfo((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1671 "parser.cpp"
    break;

  case 142: // fn_parameter: var_type nameNode
#line 977 "bnf.y"
            {
                (yylhs.value.node) = newParamInfo((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1679 "parser.cpp"
    break;

  case 143: // fn_opt_params_list: fn_params_list
#line 984 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1687 "parser.cpp"
    break;

  case 144: // fn_opt_params_list: %empty
#line 988 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1695 "parser.cpp"
    break;

  case 145: // fn_params_list: fn_params_list COMMA fn_parameter
#line 995 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node)->data.nodeList.list, (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1704 "parser.cpp"
    break;

  case 146: // fn_params_list: fn_parameter
#line 1000 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1712 "parser.cpp"
    break;

  case 147: // fn_def: FN nameNode LPAREN fn_opt_params_list RPAREN code_block
#line 1007 "bnf.y"
            {
                (yylhs.value.node) = newFnDef((yystack_[4].value.node)->data.stringValue, (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 1720 "parser.cpp"
    break;

  case 148: // var_decl: VAR var_decl_assign_list
#line 1015 "bnf.y"
            {
                (yylhs.value.node) = newVarDeclBatch((yystack_[0].value.node));
            }
#line 1728 "parser.cpp"
    break;

  case 149: // var_decl_assign_list: var_decl_assign_list COMMA var_decl_assign
#line 1022 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node)->data.nodeList.list, (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1737 "parser.cpp"
    break;

  case 150: // var_decl_assign_list: var_decl_assign
#line 1027 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1745 "parser.cpp"
    break;

  case 151: // var_decl_assign: var_decl_modifiers nameNode EQUALS expr
#line 1034 "bnf.y"
            {
                (yylhs.value.node) = newVarDecl((yystack_[2].value.node)->data.stringValue, (yystack_[3].value.modifier), (yystack_[0].value.node));
            }
#line 1753 "parser.cpp"
    break;

  case 152: // var_decl_assign: var_decl_modifiers nameNode
#line 1038 "bnf.y"
            {
                (yylhs.value.node) = newVarDecl((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.modifier), NULL);
            }
#line 1761 "parser.cpp"
    break;

  case 153: // var_decl_modifiers: var_decl_modifiers var_decl_modifier
#line 1045 "bnf.y"
            {
                (yylhs.value.modifier) = static_cast<Modifier>(to_underlying((yystack_[1].value.modifier)) | to_underlying((yystack_[0].value.modifier)));
            }
#line 1769 "parser.cpp"
    break;

  case 154: // var_decl_modifiers: %empty
#line 1049 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::None;
            }
#line 1777 "parser.cpp"
    break;

  case 155: // var_decl_modifier: CONST
#line 1056 "bnf.y"
            {
                (yylhs.value.modifier) = (yystack_[0].value.modifier);
            }
#line 1785 "parser.cpp"
    break;

  case 156: // var_decl_modifier: LOCAL
#line 1060 "bnf.y"
            {
                (yylhs.value.modifier) = (yystack_[0].value.modifier);
            }
#line 1793 "parser.cpp"
    break;

  case 157: // var_type: VAR
#line 1067 "bnf.y"
            {
                (yylhs.value.node) = newName("var");
            }
#line 1801 "parser.cpp"
    break;

  case 158: // assign: expr op_assign assign_rhs
#line 1075 "bnf.y"
            {
                (yylhs.value.node) = newVarAssign((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 1809 "parser.cpp"
    break;

  case 159: // assign_rhs: expr
#line 1082 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1817 "parser.cpp"
    break;

  case 160: // class_def: CLASS nameNode opt_delimiter_list LBRACE opt_class_member_list opt_delimiter_list RBRACE
#line 1129 "bnf.y"
            {
                (yylhs.value.node) = newClassDef((yystack_[5].value.node)->data.stringValue, (yystack_[2].value.node));
            }
#line 1825 "parser.cpp"
    break;

  case 161: // opt_class_member_list: class_member_list
#line 1136 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1833 "parser.cpp"
    break;

  case 162: // opt_class_member_list: %empty
#line 1140 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1841 "parser.cpp"
    break;

  case 163: // class_member_list: class_member_list delimiter_list class_member
#line 1147 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node)->data.nodeList.list, (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1850 "parser.cpp"
    break;

  case 164: // class_member_list: class_member
#line 1152 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1858 "parser.cpp"
    break;

  case 165: // class_member: class_member_mod_list var_type nameNode
#line 1159 "bnf.y"
            {
                (yylhs.value.node) = newClassFieldDef((yystack_[0].value.node)->data.stringValue, (yystack_[2].value.modifier), (yystack_[1].value.node));
            }
#line 1866 "parser.cpp"
    break;

  case 166: // class_member: class_member_mod_list fn_def
#line 1163 "bnf.y"
            {
                (yylhs.value.node) = newClassMethodDef((yystack_[1].value.modifier), (yystack_[0].value.node));
            }
#line 1874 "parser.cpp"
    break;

  case 167: // class_member_mod_list: class_member_mod_list class_member_mod
#line 1205 "bnf.y"
            {
                (yylhs.value.modifier) = static_cast<Modifier>(static_cast<uint16_t>((yystack_[1].value.modifier)) | static_cast<uint16_t>((yystack_[0].value.modifier)));
            }
#line 1882 "parser.cpp"
    break;

  case 168: // class_member_mod_list: %empty
#line 1209 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::None;
            }
#line 1890 "parser.cpp"
    break;

  case 169: // class_member_mod: HIDDEN
#line 1216 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Hidden;
            }
#line 1898 "parser.cpp"
    break;

  case 170: // class_member_mod: CONST
#line 1220 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Const;
            }
#line 1906 "parser.cpp"
    break;

  case 171: // class_member_mod: STATIC
#line 1224 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Static;
            }
#line 1914 "parser.cpp"
    break;


#line 1918 "parser.cpp"

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









  const short parser::yypact_ninf_ = -225;

  const short parser::yytable_ninf_ = -163;

  const short
  parser::yypact_[] =
  {
      44,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
     394,   381,  -225,     7,   394,   -31,   -19,    -2,     2,    34,
       7,   394,   394,     7,  -225,    18,  -225,    -6,  -225,    11,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,   409,   409,    30,   441,   -13,    83,
      39,    53,    59,    29,    22,    50,  -225,  -225,   -20,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,    47,  -225,
    -225,     5,    49,  -225,   394,   394,   394,   394,   394,    11,
    -225,  -225,    48,    45,  -225,    10,  -225,  -225,  -225,   201,
    -225,  -225,   -20,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,   394,  -225,  -225,   394,  -225,  -225,
    -225,  -225,  -225,  -225,   394,  -225,   394,  -225,   394,  -225,
     394,  -225,  -225,   394,  -225,  -225,   394,  -225,  -225,   394,
       7,   394,   394,  -225,  -225,   394,     6,    55,    64,    43,
    -225,    51,    69,    70,    68,    11,  -225,  -225,  -225,   108,
    -225,  -225,  -225,  -225,    83,    39,    53,    59,    29,    22,
      50,  -225,  -225,    85,  -225,    80,  -225,  -225,     7,  -225,
      87,    81,     7,    11,    11,   394,    88,  -225,   -23,     8,
    -225,    84,    90,  -225,   394,  -225,  -225,  -225,    11,     6,
    -225,   229,  -225,    86,  -225,    24,   -13,   151,    11,    11,
    -225,   -30,   126,  -225,    11,  -225,    99,  -225,  -225,    11,
    -225,    11,  -225,   394,   394,    98,    11,    11,  -225,   100,
     102,    -6,  -225,  -225,  -225,  -225,     7,  -225,   394,  -225,
     313,  -225,   109,  -225,   -13,    11,   104,   131,    11,  -225,
    -225,  -225,  -225,  -225,   106,    11,    11,  -225,  -225,  -225,
    -225,  -225,   297,  -225,  -225
  };

  const unsigned char
  parser::yydefact_[] =
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
      40,   122,     0,     0,     0,   138,   154,   155,   156,   152,
     153,     9,   159,   158,    92,    94,    96,    98,   100,   102,
     104,   106,   113,     0,   120,     0,   124,   157,     0,   146,
       0,   143,     0,     4,     4,     0,     0,    53,   168,     0,
     140,     0,   137,   149,     0,   114,   115,   141,     4,     0,
     142,     0,    28,    57,    38,     0,    42,    48,     4,   161,
     164,     0,   135,   134,     0,   151,     0,   147,   145,     4,
      29,     4,    55,   123,     0,     0,     4,    47,    50,     0,
       0,   168,   170,   171,   169,   166,     0,   167,     0,   139,
      25,    56,     0,    43,    41,     4,     0,     0,     4,   160,
     163,   165,   136,    27,     0,    24,     4,    52,    46,    49,
      51,    23,     0,    39,    26
  };

  const short
  parser::yypgoto_[] =
  {
    -225,  -225,     3,   -22,  -225,  -225,    71,  -225,  -225,   -38,
    -224,   -26,  -225,  -225,  -166,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,   -74,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,   -10,  -170,    60,    56,    57,    58,    61,
      46,    54,    65,    42,   125,  -225,  -225,  -129,   168,  -225,
    -225,    -4,  -225,   -18,  -225,  -225,    -5,  -225,  -225,   -12,
    -225,  -225,    74,  -225,  -225,   -11,  -225,  -225,  -225,  -225,
    -225,   -33,  -225,  -225
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,    25,   191,    27,    28,    29,    30,    31,    32,    33,
      34,   192,   244,   245,   193,    35,    36,    37,    38,   139,
     195,   232,    39,    40,   216,   217,   218,    41,    42,   212,
      43,   104,   107,   114,   116,   118,   120,   123,   126,   129,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,   165,   140,   141,    60,
      61,    62,    63,   180,   181,   182,   169,   170,   171,    64,
      65,    83,    84,    85,   150,   172,    66,   153,    67,   198,
     199,   200,   201,   227
  };

  const short
  parser::yytable_[] =
  {
      68,    70,   163,    26,    73,   196,   243,    89,   194,    72,
      74,    80,    81,     5,     5,     5,    79,     5,    86,    82,
     130,   131,    75,  -162,    13,   105,   106,   132,   254,  -162,
     124,   125,    88,     1,     2,     3,     4,     5,   222,    76,
     167,   223,   224,    77,   234,   231,    87,     1,     2,     3,
       4,     5,     6,   134,     7,   135,   121,   122,     8,     9,
      87,   127,   128,    12,   137,   138,    70,   142,   143,   247,
     115,    10,   250,   213,   214,    78,   167,    11,   147,   148,
     253,   149,   144,   117,   233,    10,    90,    91,   119,   133,
     136,    11,   175,   145,   152,   146,    12,   173,    13,    14,
      15,   135,    16,    17,    18,    19,   174,    20,    21,    22,
      23,   176,   177,   178,    24,   108,   109,   110,   111,   112,
     113,    70,   164,   179,   184,   166,   162,   185,   186,   188,
     203,   189,   168,   197,     1,     2,     3,     4,     5,     6,
     204,     7,   228,   211,   209,     8,     9,   235,   239,   238,
     248,   246,   251,   210,     1,     2,     3,     4,     5,     6,
     151,     7,   207,   249,   187,     8,     9,   154,   190,   159,
     155,    92,    10,   156,   205,   202,   157,   221,    11,    71,
     160,   158,   179,    87,   208,   168,   229,   219,   240,   225,
     226,   206,    10,   215,   161,   237,     0,     0,    11,     0,
       0,   220,     0,    70,     1,     2,     3,     4,     5,     6,
       0,     7,   230,   215,     0,     8,     9,     0,   242,   236,
     183,     0,   241,   252,     0,     0,     0,   219,     0,     0,
       0,     0,     1,     2,     3,     4,     5,     6,     0,     7,
       0,     0,    10,     8,     9,     0,     0,     0,    11,     0,
       0,     0,     0,    87,     0,    13,    14,    15,     0,    16,
      17,    18,    19,     0,    20,    21,    22,    23,     0,     0,
      10,    24,     0,     0,   209,     0,    11,     0,     0,     0,
       0,     0,     0,     0,    14,    15,     0,    16,    17,    18,
      19,     0,     0,    21,    22,     0,     0,     0,     0,    24,
       1,     2,     3,     4,     5,     6,     0,     7,     0,     0,
       0,     8,     9,     0,     0,     0,     1,     2,     3,     4,
       5,     6,     0,     7,     0,     0,     0,     8,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    10,     0,
       0,     0,     0,     0,    11,     0,     0,     0,     0,    87,
       0,     0,    14,    15,    10,    16,    17,    18,    19,     0,
      11,    21,    22,     0,     0,     0,     0,    24,    14,    15,
       0,    16,    17,    18,    19,     0,     0,    21,    22,     0,
       0,     0,     0,    24,     1,     2,     3,     4,     5,     6,
       0,     7,     0,     0,     0,     8,     9,     1,     2,     3,
       4,     5,     6,     0,     7,     0,     0,     0,     8,     9,
       0,     0,     1,     2,     3,     4,     5,     0,     0,     0,
       0,     0,    10,     8,     9,     0,     0,     0,    11,    69,
       0,     0,     0,     0,     0,    10,     0,     0,     0,     0,
       0,    11,     0,     0,     0,     0,     0,     0,     0,     0,
      10,     0,     0,     0,     0,     0,    11,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103
  };

  const short
  parser::yycheck_[] =
  {
      10,    11,   131,     0,    14,   175,   230,    29,   174,    13,
      41,    21,    22,     7,     7,     7,    20,     7,     0,    23,
      40,    41,    41,    46,    54,    38,    39,    47,   252,    52,
       8,     9,    29,     3,     4,     5,     6,     7,    68,    41,
      70,    71,    72,    41,   214,   211,    52,     3,     4,     5,
       6,     7,     8,    48,    10,    50,    27,    28,    14,    15,
      52,    11,    12,    52,    74,    75,    76,    77,    78,   235,
      31,    41,   238,    49,    50,    41,    70,    47,    68,    69,
     246,    85,    79,    30,   213,    41,    44,    45,    29,    42,
      41,    47,    49,    45,   104,    50,    52,    42,    54,    55,
      56,    50,    58,    59,    60,    61,    42,    63,    64,    65,
      66,    42,    42,    45,    70,    32,    33,    34,    35,    36,
      37,   131,   132,   145,    16,   135,   130,    42,    48,    42,
      46,    50,   136,    45,     3,     4,     5,     6,     7,     8,
      50,    10,    16,    57,    45,    14,    15,    49,    46,    49,
      46,    42,    46,   191,     3,     4,     5,     6,     7,     8,
      89,    10,   188,   237,   168,    14,    15,   107,   172,   123,
     114,    46,    41,   116,   184,   179,   118,   199,    47,    11,
     126,   120,   204,    52,   189,   189,   204,   197,   221,   201,
     201,   188,    41,    62,   129,   217,    -1,    -1,    47,    -1,
      -1,   198,    -1,   213,     3,     4,     5,     6,     7,     8,
      -1,    10,   209,    62,    -1,    14,    15,    -1,   228,   216,
     146,    -1,   226,   245,    -1,    -1,    -1,   237,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,     8,    -1,    10,
      -1,    -1,    41,    14,    15,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    52,    -1,    54,    55,    56,    -1,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    -1,    -1,
      41,    70,    -1,    -1,    45,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,
      61,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
       3,     4,     5,     6,     7,     8,    -1,    10,    -1,    -1,
      -1,    14,    15,    -1,    -1,    -1,     3,     4,     5,     6,
       7,     8,    -1,    10,    -1,    -1,    -1,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    55,    56,    41,    58,    59,    60,    61,    -1,
      47,    64,    65,    -1,    -1,    -1,    -1,    70,    55,    56,
      -1,    58,    59,    60,    61,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,     3,     4,     5,     6,     7,     8,
      -1,    10,    -1,    -1,    -1,    14,    15,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    -1,    -1,    14,    15,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      -1,    -1,    41,    14,    15,    -1,    -1,    -1,    47,    48,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    47,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     3,     4,     5,     6,     7,     8,    10,    14,    15,
      41,    47,    52,    54,    55,    56,    58,    59,    60,    61,
      63,    64,    65,    66,    70,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    90,    91,    92,    93,    97,
      98,   102,   103,   105,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     134,   135,   136,   137,   144,   145,   151,   153,   118,    48,
     118,   133,   136,   118,    41,    41,    41,    41,    41,   136,
     118,   118,   136,   146,   147,   148,     0,    52,    77,    78,
     128,   128,   129,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   106,    38,    39,   107,    32,    33,
      34,    35,    36,    37,   108,    31,   109,    30,   110,    29,
     111,    27,    28,   112,     8,     9,   113,    11,    12,   114,
      40,    41,    47,    42,    48,    50,    41,   118,   118,    94,
     132,   133,   118,   118,    77,    45,    50,    68,    69,   136,
     149,    81,   118,   152,   120,   121,   122,   123,   124,   125,
     126,   127,   136,   132,   118,   131,   118,    70,   136,   141,
     142,   143,   150,    42,    42,    49,    42,    42,    45,    78,
     138,   139,   140,   147,    16,    42,    48,   136,    42,    50,
     136,    77,    86,    89,    89,    95,   119,    45,   154,   155,
     156,   157,   136,    46,    50,   118,    77,    86,   141,    45,
      84,    57,   104,    49,    50,    62,    99,   100,   101,   118,
      77,    78,    68,    71,    72,   144,   150,   158,    16,   138,
      77,    89,    96,   132,   119,    49,    77,    78,    49,    46,
     156,   136,   118,    85,    87,    88,    42,    89,    46,   101,
      89,    46,    78,    89,    85
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    75,    76,    77,    77,    78,    78,    79,    79,    80,
      80,    81,    81,    82,    83,    83,    83,    84,    85,    85,
      85,    85,    85,    86,    87,    87,    88,    88,    89,    89,
      90,    91,    91,    91,    91,    91,    91,    91,    92,    93,
      94,    95,    95,    96,    97,    97,    98,    99,    99,   100,
     100,   101,   101,   102,   102,   103,   104,   104,   105,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     107,   107,   108,   108,   108,   108,   108,   108,   109,   110,
     111,   112,   112,   113,   113,   114,   114,   115,   116,   117,
     117,   118,   119,   119,   120,   120,   121,   121,   122,   122,
     123,   123,   124,   124,   125,   125,   126,   126,   127,   127,
     127,   128,   128,   129,   129,   129,   129,   130,   130,   130,
     131,   131,   132,   132,   133,   133,   134,   134,   134,   134,
     134,   135,   135,   136,   137,   138,   138,   139,   139,   140,
     140,   141,   141,   142,   142,   143,   143,   144,   145,   146,
     146,   147,   147,   148,   148,   149,   149,   150,   151,   152,
     153,   154,   154,   155,   155,   156,   156,   157,   157,   158,
     158,   158
  };

  const signed char
  parser::yyr2_[] =
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
       1,     4,     2,     2,     0,     1,     1,     1,     3,     1,
       7,     1,     0,     3,     1,     3,     2,     2,     0,     1,
       1,     1
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
  "var_decl_modifier", "var_type", "assign", "assign_rhs", "class_def",
  "opt_class_member_list", "class_member_list", "class_member",
  "class_member_mod_list", "class_member_mod", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   216,   216,   223,   224,   228,   229,   233,   237,   244,
     249,   256,   260,   268,   275,   279,   283,   291,   298,   302,
     306,   310,   314,   322,   329,   334,   340,   345,   352,   356,
     364,   372,   376,   380,   384,   388,   392,   396,   403,   410,
     417,   424,   429,   436,   443,   447,   454,   461,   466,   472,
     477,   484,   488,   495,   499,   506,   513,   517,   524,   532,
     536,   540,   544,   548,   552,   556,   560,   564,   568,   572,
     579,   583,   590,   594,   598,   602,   606,   610,   617,   624,
     631,   638,   642,   649,   653,   660,   664,   671,   678,   685,
     689,   697,   704,   708,   715,   719,   726,   730,   737,   741,
     748,   752,   759,   763,   770,   774,   781,   785,   792,   796,
     800,   807,   811,   818,   822,   826,   830,   837,   841,   845,
     852,   857,   863,   868,   874,   879,   886,   890,   894,   898,
     902,   910,   914,   922,   930,   937,   941,   948,   953,   959,
     964,   972,   976,   983,   988,   994,   999,  1006,  1014,  1021,
    1026,  1033,  1037,  1044,  1049,  1055,  1059,  1066,  1074,  1081,
    1128,  1135,  1140,  1146,  1151,  1158,  1162,  1204,  1209,  1215,
    1219,  1223
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
#line 2531 "parser.cpp"

#line 1229 "bnf.y"
