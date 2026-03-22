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
#line 22 "bnf.y"

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
#line 224 "bnf.y"
            {
                astRoot = (yystack_[0].value.node);
            }
#line 586 "parser.cpp"
    break;

  case 7: // opt_program_stmt_list: program_stmt_list opt_delimiter_list
#line 241 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 594 "parser.cpp"
    break;

  case 8: // opt_program_stmt_list: opt_delimiter_list
#line 245 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 602 "parser.cpp"
    break;

  case 9: // program_stmt_list: program_stmt_list delimiter_list program_stmt
#line 252 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 611 "parser.cpp"
    break;

  case 10: // program_stmt_list: program_stmt
#line 257 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 619 "parser.cpp"
    break;

  case 11: // program_stmt: stmt
#line 264 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 627 "parser.cpp"
    break;

  case 12: // program_stmt: global_stmt
#line 268 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 635 "parser.cpp"
    break;

  case 13: // global_stmt: global_stmt_stmt
#line 276 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 643 "parser.cpp"
    break;

  case 14: // global_stmt_stmt: class_def
#line 283 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 651 "parser.cpp"
    break;

  case 15: // global_stmt_stmt: fn_def
#line 287 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 659 "parser.cpp"
    break;

  case 16: // global_stmt_stmt: enum_stmt
#line 291 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 667 "parser.cpp"
    break;

  case 17: // stmt: stmt_stmt
#line 299 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 675 "parser.cpp"
    break;

  case 18: // stmt_stmt: var_decl
#line 306 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 683 "parser.cpp"
    break;

  case 19: // stmt_stmt: cntrl_cnstrct
#line 310 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 691 "parser.cpp"
    break;

  case 20: // stmt_stmt: del_stmt
#line 314 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 699 "parser.cpp"
    break;

  case 21: // stmt_stmt: assign
#line 318 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 707 "parser.cpp"
    break;

  case 22: // stmt_stmt: expr
#line 322 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 715 "parser.cpp"
    break;

  case 23: // code_block: opt_delimiter_list LBRACE opt_stmt_list RBRACE
#line 330 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 723 "parser.cpp"
    break;

  case 24: // opt_stmt_list: stmt_list
#line 337 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 731 "parser.cpp"
    break;

  case 25: // opt_stmt_list: %empty
#line 341 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 739 "parser.cpp"
    break;

  case 26: // $@1: %empty
#line 348 "bnf.y"
            {
                ctx.nodeLLStack.push(newNodeList((yystack_[0].value.node)));
            }
#line 747 "parser.cpp"
    break;

  case 27: // stmt_list: opt_delimiter_list stmt_stmt $@1 stmt_list_tail
#line 352 "bnf.y"
            {
              (yylhs.value.node) = ctx.nodeLLStack.top();
              ctx.nodeLLStack.pop();
            }
#line 756 "parser.cpp"
    break;

  case 28: // stmt_list_tail: delimiter_list stmt_stmt stmt_list_tail
#line 360 "bnf.y"
            {
                astNodeLLAppend(ctx.nodeLLStack.top(), (yystack_[1].value.node));
            }
#line 764 "parser.cpp"
    break;

  case 31: // code_block_or_stmt: code_block
#line 369 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 772 "parser.cpp"
    break;

  case 32: // code_block_or_stmt: opt_delimiter_list stmt
#line 373 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 780 "parser.cpp"
    break;

  case 33: // del_stmt: DEL expr
#line 381 "bnf.y"
            {
                (yylhs.value.node) = newDelStmt((yystack_[0].value.node));
            }
#line 788 "parser.cpp"
    break;

  case 34: // cntrl_cnstrct: loop_whl
#line 389 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 796 "parser.cpp"
    break;

  case 35: // cntrl_cnstrct: loop_for
#line 393 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 804 "parser.cpp"
    break;

  case 36: // cntrl_cnstrct: switch_stmt
#line 397 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 812 "parser.cpp"
    break;

  case 37: // cntrl_cnstrct: ret_stmt
#line 401 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 820 "parser.cpp"
    break;

  case 38: // cntrl_cnstrct: brk_stmt
#line 405 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 828 "parser.cpp"
    break;

  case 39: // cntrl_cnstrct: if_stmt
#line 409 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 836 "parser.cpp"
    break;

  case 40: // cntrl_cnstrct: toss_stmt
#line 413 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 844 "parser.cpp"
    break;

  case 41: // loop_whl: WHL LPAREN expr RPAREN code_block_or_stmt
#line 420 "bnf.y"
            {
                (yylhs.value.node) = newLoopWhl((yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 852 "parser.cpp"
    break;

  case 42: // loop_for: FOR LPAREN loop_for_init COLON loop_for_cond COLON loop_for_post_iter RPAREN code_block_or_stmt
#line 427 "bnf.y"
            {
                (yylhs.value.node) = newLoopFor((yystack_[6].value.node), (yystack_[4].value.node), (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 860 "parser.cpp"
    break;

  case 43: // loop_for_init: opt_expr_list
#line 434 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 868 "parser.cpp"
    break;

  case 44: // loop_for_cond: loop_for_cond COMMA lvl_logic
#line 441 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 877 "parser.cpp"
    break;

  case 45: // loop_for_cond: lvl_logic
#line 446 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 885 "parser.cpp"
    break;

  case 46: // loop_for_post_iter: opt_expr_list
#line 453 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 893 "parser.cpp"
    break;

  case 47: // ret_stmt: RET
#line 460 "bnf.y"
            {
                (yylhs.value.node) = newRetStmt(NULL);
            }
#line 901 "parser.cpp"
    break;

  case 48: // ret_stmt: RET expr
#line 464 "bnf.y"
            {
                (yylhs.value.node) = newRetStmt((yystack_[0].value.node));
            }
#line 909 "parser.cpp"
    break;

  case 49: // switch_stmt: SWITCH LPAREN expr RPAREN LBRACE opt_switch_case_list opt_delimiter_list RBRACE
#line 471 "bnf.y"
            {
                (yylhs.value.node) = newSwitchStmt((yystack_[5].value.node), (yystack_[2].value.node));
            }
#line 917 "parser.cpp"
    break;

  case 50: // opt_switch_case_list: switch_case_list
#line 478 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 925 "parser.cpp"
    break;

  case 51: // opt_switch_case_list: %empty
#line 482 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 933 "parser.cpp"
    break;

  case 52: // switch_case_list: switch_case_list delimiter_list switch_case
#line 489 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 942 "parser.cpp"
    break;

  case 53: // switch_case_list: switch_case
#line 494 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 950 "parser.cpp"
    break;

  case 54: // switch_case: expr COLON code_block_or_stmt
#line 501 "bnf.y"
            {
                (yylhs.value.node) = newSwitchCase((yystack_[2].value.node), (yystack_[0].value.node), false);
            }
#line 958 "parser.cpp"
    break;

  case 55: // switch_case: DEFAULT COLON code_block_or_stmt
#line 505 "bnf.y"
            {
                (yylhs.value.node) = newSwitchCase(NULL, (yystack_[0].value.node), true);
            }
#line 966 "parser.cpp"
    break;

  case 56: // brk_stmt: BRK LPAREN expr RPAREN
#line 512 "bnf.y"
            {
                (yylhs.value.node) = newBrkStmt((yystack_[1].value.node));
            }
#line 974 "parser.cpp"
    break;

  case 57: // brk_stmt: BRK
#line 516 "bnf.y"
            {
                (yylhs.value.node) = newBrkStmt(NULL);
            }
#line 982 "parser.cpp"
    break;

  case 58: // if_stmt: IF LPAREN expr RPAREN code_block_or_stmt opt_else
#line 523 "bnf.y"
            {
                (yylhs.value.node) = newIfStmt((yystack_[3].value.node), (yystack_[1].value.node), (yystack_[0].value.node));
            }
#line 990 "parser.cpp"
    break;

  case 59: // opt_else: ELSE code_block_or_stmt
#line 530 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 998 "parser.cpp"
    break;

  case 60: // opt_else: %empty
#line 534 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1006 "parser.cpp"
    break;

  case 61: // toss_stmt: TOSS expr
#line 541 "bnf.y"
            {
                (yylhs.value.node) = newTossStmt((yystack_[0].value.node));
            }
#line 1014 "parser.cpp"
    break;

  case 62: // op_assign: EQUALS
#line 549 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1022 "parser.cpp"
    break;

  case 63: // op_assign: ASSIGN_ADD
#line 553 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1030 "parser.cpp"
    break;

  case 64: // op_assign: ASSIGN_SUB
#line 557 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1038 "parser.cpp"
    break;

  case 65: // op_assign: ASSIGN_DIV
#line 561 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1046 "parser.cpp"
    break;

  case 66: // op_assign: ASSIGN_MUL
#line 565 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1054 "parser.cpp"
    break;

  case 67: // op_assign: ASSIGN_MOD
#line 569 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1062 "parser.cpp"
    break;

  case 68: // op_assign: ASSIGN_XOR
#line 573 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1070 "parser.cpp"
    break;

  case 69: // op_assign: ASSIGN_SHIFTL
#line 577 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1078 "parser.cpp"
    break;

  case 70: // op_assign: ASSIGN_SHIFTR
#line 581 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1086 "parser.cpp"
    break;

  case 71: // op_assign: ASSIGN_AND
#line 585 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1094 "parser.cpp"
    break;

  case 72: // op_assign: ASSIGN_OR
#line 589 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1102 "parser.cpp"
    break;

  case 73: // op_logic: LOGIC_AND
#line 596 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1110 "parser.cpp"
    break;

  case 74: // op_logic: LOGIC_OR
#line 600 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1118 "parser.cpp"
    break;

  case 75: // op_cond: COND_GT
#line 607 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1126 "parser.cpp"
    break;

  case 76: // op_cond: COND_GEQ
#line 611 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1134 "parser.cpp"
    break;

  case 77: // op_cond: COND_LT
#line 615 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1142 "parser.cpp"
    break;

  case 78: // op_cond: COND_LEQ
#line 619 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1150 "parser.cpp"
    break;

  case 79: // op_cond: COND_EQ
#line 623 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1158 "parser.cpp"
    break;

  case 80: // op_cond: COND_NEQ
#line 627 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1166 "parser.cpp"
    break;

  case 81: // op_bitwise_xor: BIT_XOR
#line 634 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1174 "parser.cpp"
    break;

  case 82: // op_bitwise_or: BIT_OR
#line 641 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1182 "parser.cpp"
    break;

  case 83: // op_bitwise_and: BIT_AND
#line 648 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1190 "parser.cpp"
    break;

  case 84: // op_bitwise_shift: BIT_SHIFTL
#line 655 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1198 "parser.cpp"
    break;

  case 85: // op_bitwise_shift: BIT_SHIFTR
#line 659 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1206 "parser.cpp"
    break;

  case 86: // op_arithm_add: PLUS
#line 666 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1214 "parser.cpp"
    break;

  case 87: // op_arithm_add: MINUS
#line 670 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1222 "parser.cpp"
    break;

  case 88: // op_arithm_mul: STAR
#line 677 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1230 "parser.cpp"
    break;

  case 89: // op_arithm_mul: SLASH
#line 681 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1238 "parser.cpp"
    break;

  case 90: // op_unary_neg: MINUS
#line 688 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1246 "parser.cpp"
    break;

  case 91: // op_unary_logic_neg: BANG
#line 695 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1254 "parser.cpp"
    break;

  case 92: // op_unary_inc: UNARY_DEC
#line 702 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1262 "parser.cpp"
    break;

  case 93: // op_unary_inc: UNARY_INC
#line 706 "bnf.y"
            {
                (yylhs.value.opType) = (yystack_[0].value.opType);
            }
#line 1270 "parser.cpp"
    break;

  case 94: // expr: lvl_logic
#line 714 "bnf.y"
            { 
                (yylhs.value.node) = (yystack_[0].value.node); 
            }
#line 1278 "parser.cpp"
    break;

  case 95: // lvl_logic: lvl_logic op_logic lvl_cond
#line 721 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1286 "parser.cpp"
    break;

  case 96: // lvl_logic: lvl_cond
#line 725 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1294 "parser.cpp"
    break;

  case 97: // lvl_cond: lvl_cond op_cond lvl_bitwise_xor
#line 732 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1302 "parser.cpp"
    break;

  case 98: // lvl_cond: lvl_bitwise_xor
#line 736 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1310 "parser.cpp"
    break;

  case 99: // lvl_bitwise_xor: lvl_bitwise_xor op_bitwise_xor lvl_bitwise_or
#line 743 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1318 "parser.cpp"
    break;

  case 100: // lvl_bitwise_xor: lvl_bitwise_or
#line 747 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1326 "parser.cpp"
    break;

  case 101: // lvl_bitwise_or: lvl_bitwise_or op_bitwise_or lvl_bitwise_and
#line 754 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1334 "parser.cpp"
    break;

  case 102: // lvl_bitwise_or: lvl_bitwise_and
#line 758 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1342 "parser.cpp"
    break;

  case 103: // lvl_bitwise_and: lvl_bitwise_and op_bitwise_and lvl_bitwise_shift
#line 765 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1350 "parser.cpp"
    break;

  case 104: // lvl_bitwise_and: lvl_bitwise_shift
#line 769 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1358 "parser.cpp"
    break;

  case 105: // lvl_bitwise_shift: lvl_bitwise_shift op_bitwise_shift lvl_arithm_add
#line 776 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1366 "parser.cpp"
    break;

  case 106: // lvl_bitwise_shift: lvl_arithm_add
#line 780 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1374 "parser.cpp"
    break;

  case 107: // lvl_arithm_add: lvl_arithm_add op_arithm_add lvl_arithm_mul
#line 787 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1382 "parser.cpp"
    break;

  case 108: // lvl_arithm_add: lvl_arithm_mul
#line 791 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1390 "parser.cpp"
    break;

  case 109: // lvl_arithm_mul: lvl_arithm_mul op_arithm_mul lvl_unary_neg
#line 798 "bnf.y"
            {   
                (yylhs.value.node) = newBinaryOp((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1398 "parser.cpp"
    break;

  case 110: // lvl_arithm_mul: lvl_unary_neg
#line 802 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1406 "parser.cpp"
    break;

  case 111: // lvl_unary_neg: op_unary_neg lvl_unary_inc
#line 809 "bnf.y"
            {
                (yylhs.value.node) = newUnaryOp((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1414 "parser.cpp"
    break;

  case 112: // lvl_unary_neg: op_unary_logic_neg lvl_unary_inc
#line 813 "bnf.y"
            {
                (yylhs.value.node) = newUnaryOp((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1422 "parser.cpp"
    break;

  case 113: // lvl_unary_neg: lvl_unary_inc
#line 817 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1430 "parser.cpp"
    break;

  case 114: // lvl_unary_inc: op_unary_inc lvl_postfix
#line 824 "bnf.y"
            {
                (yylhs.value.node) = newUnaryOp((yystack_[1].value.opType), (yystack_[0].value.node));
            }
#line 1438 "parser.cpp"
    break;

  case 115: // lvl_unary_inc: lvl_postfix
#line 828 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1446 "parser.cpp"
    break;

  case 116: // lvl_postfix: lvl_postfix DOT nameNode
#line 835 "bnf.y"
            {   
                (yylhs.value.node) = newMemberAccess((yystack_[2].value.node), (yystack_[0].value.node));   
            }
#line 1454 "parser.cpp"
    break;

  case 117: // lvl_postfix: lvl_postfix LPAREN opt_expr_list RPAREN
#line 839 "bnf.y"
            {   
                (yylhs.value.node) = newFnCall((yystack_[3].value.node), (yystack_[1].value.node));   
            }
#line 1462 "parser.cpp"
    break;

  case 118: // lvl_postfix: lvl_postfix LSQUARE opt_expr RSQUARE
#line 843 "bnf.y"
            {   
                (yylhs.value.node) = newArrayAccess((yystack_[3].value.node), (yystack_[1].value.node));   
            }
#line 1470 "parser.cpp"
    break;

  case 119: // lvl_postfix: primary
#line 847 "bnf.y"
            {   
                (yylhs.value.node) = (yystack_[0].value.node);    
            }
#line 1478 "parser.cpp"
    break;

  case 120: // primary: literal
#line 854 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1486 "parser.cpp"
    break;

  case 121: // primary: nameNode
#line 858 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1494 "parser.cpp"
    break;

  case 122: // primary: LPAREN expr RPAREN
#line 862 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[1].value.node);
            }
#line 1502 "parser.cpp"
    break;

  case 123: // opt_expr: expr
#line 869 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1510 "parser.cpp"
    break;

  case 124: // opt_expr: %empty
#line 873 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1518 "parser.cpp"
    break;

  case 125: // opt_expr_list: expr_list
#line 880 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1526 "parser.cpp"
    break;

  case 126: // opt_expr_list: %empty
#line 884 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1534 "parser.cpp"
    break;

  case 127: // expr_list: expr_list COMMA expr
#line 891 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1543 "parser.cpp"
    break;

  case 128: // expr_list: expr
#line 896 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1551 "parser.cpp"
    break;

  case 129: // literal: TRUE
#line 903 "bnf.y"
            { 
                (yylhs.value.node) = newBoolValue(true); 
            }
#line 1559 "parser.cpp"
    break;

  case 130: // literal: FALSE
#line 907 "bnf.y"
            {
                (yylhs.value.node) = newBoolValue(false);
            }
#line 1567 "parser.cpp"
    break;

  case 131: // literal: NUMBER
#line 911 "bnf.y"
            {
                (yylhs.value.node) = newNumberValue((yystack_[0].value.numberValue));
            }
#line 1575 "parser.cpp"
    break;

  case 132: // literal: arr_initializer
#line 915 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1583 "parser.cpp"
    break;

  case 133: // literal: STRING
#line 919 "bnf.y"
            {
                (yylhs.value.node) = newStringValue((yystack_[0].value.stringValue));
            }
#line 1591 "parser.cpp"
    break;

  case 134: // arr_initializer: LSQUARE RSQUARE
#line 927 "bnf.y"
            {
                (yylhs.value.node) = newArray(NULL);
            }
#line 1599 "parser.cpp"
    break;

  case 135: // arr_initializer: LSQUARE expr_list RSQUARE
#line 931 "bnf.y"
            {
                (yylhs.value.node) = newArray((yystack_[1].value.node));
            }
#line 1607 "parser.cpp"
    break;

  case 136: // nameNode: NAME
#line 939 "bnf.y"
            {
                (yylhs.value.node) = newName((yystack_[0].value.stringValue));
            }
#line 1615 "parser.cpp"
    break;

  case 137: // enum_stmt: ENUM nameNode LBRACE enum_opt_elements RBRACE
#line 947 "bnf.y"
            {
                (yylhs.value.node) = newEnumDef((yystack_[3].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1623 "parser.cpp"
    break;

  case 138: // enum_element: delimiter_list nameNode
#line 954 "bnf.y"
            {
                (yylhs.value.node) = newEnumElement((yystack_[0].value.node)->data.stringValue, NULL);
            }
#line 1631 "parser.cpp"
    break;

  case 139: // enum_element: delimiter_list nameNode EQUALS expr
#line 958 "bnf.y"
            {
                (yylhs.value.node) = newEnumElement((yystack_[2].value.node)->data.stringValue, (yystack_[0].value.node));
            }
#line 1639 "parser.cpp"
    break;

  case 140: // enum_opt_elements: enum_elements
#line 965 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1647 "parser.cpp"
    break;

  case 141: // enum_opt_elements: %empty
#line 969 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1655 "parser.cpp"
    break;

  case 142: // enum_elements: enum_elements COMMA enum_element
#line 976 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1664 "parser.cpp"
    break;

  case 143: // enum_elements: enum_element
#line 981 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1672 "parser.cpp"
    break;

  case 144: // fn_parameter: nameNode nameNode
#line 989 "bnf.y"
            {
                (yylhs.value.node) = newParamInfo((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1680 "parser.cpp"
    break;

  case 145: // fn_parameter: var_type nameNode
#line 993 "bnf.y"
            {
                (yylhs.value.node) = newParamInfo((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.node));
            }
#line 1688 "parser.cpp"
    break;

  case 146: // fn_opt_params_list: fn_params_list
#line 1000 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1696 "parser.cpp"
    break;

  case 147: // fn_opt_params_list: %empty
#line 1004 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1704 "parser.cpp"
    break;

  case 148: // fn_params_list: fn_params_list COMMA fn_parameter
#line 1011 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1713 "parser.cpp"
    break;

  case 149: // fn_params_list: fn_parameter
#line 1016 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1721 "parser.cpp"
    break;

  case 150: // fn_def: FN nameNode LPAREN fn_opt_params_list RPAREN code_block
#line 1023 "bnf.y"
            {
                (yylhs.value.node) = newFnDef((yystack_[4].value.node)->data.stringValue, (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 1729 "parser.cpp"
    break;

  case 151: // var_decl: VAR var_decl_assign_list
#line 1031 "bnf.y"
            {
                (yylhs.value.node) = newVarDeclBatch((yystack_[0].value.node));
            }
#line 1737 "parser.cpp"
    break;

  case 152: // var_decl_assign_list: var_decl_assign_list COMMA var_decl_assign
#line 1038 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1746 "parser.cpp"
    break;

  case 153: // var_decl_assign_list: var_decl_assign
#line 1043 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1754 "parser.cpp"
    break;

  case 154: // var_decl_assign: var_decl_modifiers nameNode EQUALS expr
#line 1050 "bnf.y"
            {
                (yylhs.value.node) = newVarDecl((yystack_[2].value.node)->data.stringValue, (yystack_[3].value.modifier), (yystack_[0].value.node));
            }
#line 1762 "parser.cpp"
    break;

  case 155: // var_decl_assign: var_decl_modifiers nameNode
#line 1054 "bnf.y"
            {
                (yylhs.value.node) = newVarDecl((yystack_[0].value.node)->data.stringValue, (yystack_[1].value.modifier), NULL);
            }
#line 1770 "parser.cpp"
    break;

  case 156: // var_decl_modifiers: var_decl_modifiers var_decl_modifier
#line 1061 "bnf.y"
            {
                (yylhs.value.modifier) = static_cast<Modifier>(to_underlying((yystack_[1].value.modifier)) | to_underlying((yystack_[0].value.modifier)));
            }
#line 1778 "parser.cpp"
    break;

  case 157: // var_decl_modifiers: %empty
#line 1065 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::None;
            }
#line 1786 "parser.cpp"
    break;

  case 158: // var_decl_modifier: CONST
#line 1072 "bnf.y"
            {
                (yylhs.value.modifier) = (yystack_[0].value.modifier);
            }
#line 1794 "parser.cpp"
    break;

  case 159: // var_decl_modifier: LOCAL
#line 1076 "bnf.y"
            {
                (yylhs.value.modifier) = (yystack_[0].value.modifier);
            }
#line 1802 "parser.cpp"
    break;

  case 160: // var_type: VAR
#line 1083 "bnf.y"
            {
                (yylhs.value.node) = newName("var");
            }
#line 1810 "parser.cpp"
    break;

  case 161: // assign: expr op_assign assign_rhs
#line 1091 "bnf.y"
            {
                (yylhs.value.node) = newVarAssign((yystack_[1].value.opType), (yystack_[2].value.node), (yystack_[0].value.node));
            }
#line 1818 "parser.cpp"
    break;

  case 162: // assign_rhs: expr
#line 1098 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1826 "parser.cpp"
    break;

  case 163: // class_def: CLASS nameNode opt_delimiter_list LBRACE opt_class_member_list opt_delimiter_list RBRACE
#line 1145 "bnf.y"
            {
                (yylhs.value.node) = newClassDef((yystack_[5].value.node)->data.stringValue, (yystack_[2].value.node));
            }
#line 1834 "parser.cpp"
    break;

  case 164: // opt_class_member_list: class_member_list
#line 1152 "bnf.y"
            {
                (yylhs.value.node) = (yystack_[0].value.node);
            }
#line 1842 "parser.cpp"
    break;

  case 165: // opt_class_member_list: %empty
#line 1156 "bnf.y"
            {
                (yylhs.value.node) = NULL;
            }
#line 1850 "parser.cpp"
    break;

  case 166: // class_member_list: class_member_list delimiter_list class_member
#line 1163 "bnf.y"
            {
                astNodeLLAppend((yystack_[2].value.node), (yystack_[0].value.node));
                (yylhs.value.node) = (yystack_[2].value.node);
            }
#line 1859 "parser.cpp"
    break;

  case 167: // class_member_list: class_member
#line 1168 "bnf.y"
            {
                (yylhs.value.node) = newNodeList((yystack_[0].value.node));
            }
#line 1867 "parser.cpp"
    break;

  case 168: // class_member: class_member_mod_list var_type nameNode
#line 1175 "bnf.y"
            {
                (yylhs.value.node) = newClassFieldDef((yystack_[0].value.node)->data.stringValue, (yystack_[2].value.modifier), (yystack_[1].value.node));
            }
#line 1875 "parser.cpp"
    break;

  case 169: // class_member: class_member_mod_list fn_def
#line 1179 "bnf.y"
            {
                (yylhs.value.node) = newClassMethodDef((yystack_[1].value.modifier), (yystack_[0].value.node));
            }
#line 1883 "parser.cpp"
    break;

  case 170: // class_member_mod_list: class_member_mod_list class_member_mod
#line 1221 "bnf.y"
            {
                (yylhs.value.modifier) = static_cast<Modifier>(static_cast<uint16_t>((yystack_[1].value.modifier)) | static_cast<uint16_t>((yystack_[0].value.modifier)));
            }
#line 1891 "parser.cpp"
    break;

  case 171: // class_member_mod_list: %empty
#line 1225 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::None;
            }
#line 1899 "parser.cpp"
    break;

  case 172: // class_member_mod: HIDDEN
#line 1232 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Hidden;
            }
#line 1907 "parser.cpp"
    break;

  case 173: // class_member_mod: CONST
#line 1236 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Const;
            }
#line 1915 "parser.cpp"
    break;

  case 174: // class_member_mod: STATIC
#line 1240 "bnf.y"
            {
                (yylhs.value.modifier) = Modifier::Static;
            }
#line 1923 "parser.cpp"
    break;


#line 1927 "parser.cpp"

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









  const short parser::yypact_ninf_ = -210;

  const short parser::yytable_ninf_ = -166;

  const short
  parser::yypact_[] =
  {
      45,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
      22,   413,  -210,    -2,    22,    -7,    17,    20,    31,    33,
      -2,    22,    22,    -2,  -210,    38,  -210,    29,  -210,    39,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,   125,   125,    36,   414,   -15,   127,
      62,    68,    73,    19,    70,    76,  -210,  -210,   -25,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,    65,  -210,
    -210,   -17,    71,  -210,    22,    22,    22,    22,    22,    39,
    -210,  -210,    69,    63,  -210,     7,  -210,  -210,  -210,   192,
    -210,  -210,   -25,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,    22,  -210,  -210,    22,  -210,  -210,
    -210,  -210,  -210,  -210,    22,  -210,    22,  -210,    22,  -210,
      22,  -210,  -210,    22,  -210,  -210,    22,  -210,  -210,    22,
      -2,    22,    22,  -210,  -210,    22,     1,    74,    75,    77,
    -210,    85,    81,    82,    96,    39,  -210,  -210,  -210,   102,
    -210,  -210,  -210,  -210,   127,    62,    68,    73,    19,    70,
      76,  -210,  -210,   100,  -210,    95,  -210,  -210,    -2,  -210,
     103,    94,    -2,    39,    39,    22,   107,  -210,   -33,     2,
    -210,   108,   105,  -210,    22,  -210,  -210,  -210,    39,     1,
    -210,   260,  -210,    99,  -210,    40,   -15,   143,    39,    39,
    -210,    66,   151,  -210,    39,  -210,   123,  -210,  -210,    10,
    -210,    39,  -210,    22,    22,   121,    39,    39,  -210,   122,
     129,    29,  -210,  -210,  -210,  -210,    -2,  -210,    22,  -210,
     344,   131,  -210,  -210,   137,  -210,   -15,    39,   134,   360,
      39,  -210,  -210,  -210,  -210,  -210,  -210,    39,  -210,  -210,
    -210,  -210,    39,  -210,   276,  -210,    39,  -210
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,   133,   131,   129,   130,   136,    90,    91,    92,    93,
       0,     0,     6,     0,    47,     0,     0,     0,     0,    57,
       0,     0,     0,     0,   157,     0,     8,     3,     2,     4,
      10,    12,    13,    11,    17,    20,    19,    34,    35,    37,
      36,    38,    39,    40,     0,     0,     0,    22,    94,    96,
      98,   100,   102,   104,   106,   108,   110,   113,   115,   119,
     120,   132,   121,    16,    15,    18,    21,    14,     0,   134,
     128,     0,     0,    48,     0,     0,   126,     0,     0,     4,
      61,    33,     0,   151,   153,     0,     1,     5,     7,     3,
     111,   112,   114,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,     0,    73,    74,     0,    75,    76,
      77,    78,    79,    80,     0,    81,     0,    82,     0,    83,
       0,    84,    85,     0,    87,    86,     0,    88,    89,     0,
       0,   126,   124,   122,   135,     0,   147,     0,     0,     0,
      43,   125,     0,     0,     0,   141,   157,   158,   159,   155,
     156,     9,   162,   161,    95,    97,    99,   101,   103,   105,
     107,   109,   116,     0,   123,     0,   127,   160,     0,   149,
       0,   146,     0,     4,     4,     0,     0,    56,   171,     0,
     143,     0,   140,   152,     0,   117,   118,   144,     4,     0,
     145,     0,    31,    60,    41,     0,    45,    51,     4,   164,
     167,     0,   138,   137,     0,   154,     0,   150,   148,     4,
      32,     4,    58,   126,     0,     0,     4,    50,    53,     0,
       0,   171,   173,   174,   172,   169,     0,   170,     0,   142,
       0,     0,    24,    59,     0,    46,    44,     4,     0,     0,
       4,   163,   166,   168,   139,    26,    23,     4,    55,    49,
      52,    54,    30,    42,    29,    27,    30,    28
  };

  const short
  parser::yypgoto_[] =
  {
    -210,  -210,     6,   -26,  -210,  -210,    92,  -210,  -210,    -9,
    -209,    -5,  -210,  -210,  -210,   -71,  -167,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,   -51,  -210,
    -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,  -210,
    -210,  -210,  -210,  -210,  -210,   -10,  -157,    86,    78,    93,
      83,    88,    87,    90,    60,    51,   165,  -210,  -210,  -129,
     201,  -210,  -210,    -3,  -210,     9,  -210,  -210,    25,  -210,
    -210,    16,  -210,  -210,    79,  -210,  -210,    18,  -210,  -210,
    -210,  -210,  -210,    -1,  -210,  -210
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,    25,   191,    27,    28,    29,    30,    31,    32,    33,
      34,   192,   231,   232,   252,   255,   193,    35,    36,    37,
      38,   139,   195,   234,    39,    40,   216,   217,   218,    41,
      42,   212,    43,   104,   107,   114,   116,   118,   120,   123,
     126,   129,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,   165,   140,
     141,    60,    61,    62,    63,   180,   181,   182,   169,   170,
     171,    64,    65,    83,    84,    85,   150,   172,    66,   153,
      67,   198,   199,   200,   201,   227
  };

  const short
  parser::yytable_[] =
  {
      68,    70,   163,    89,    73,     5,    26,   194,     5,     5,
      72,    80,    81,  -165,     5,   130,   131,    79,   196,  -165,
      82,   245,   132,   105,   106,     1,     2,     3,     4,     5,
       6,   134,     7,   135,    74,    88,     8,     9,    86,     1,
       2,     3,     4,     5,   233,   256,   121,   122,     1,     2,
       3,     4,     5,     6,    87,     7,   -25,   236,    75,     8,
       9,    76,    12,    10,   137,   138,    70,   142,   143,    11,
     248,   167,    77,   251,    78,   147,   148,    10,   124,   125,
     253,    87,   149,    11,   235,   144,    10,   127,   128,   213,
     214,    12,    11,   115,   152,    90,    91,    12,   117,    13,
      14,    15,   119,    16,    17,    18,    19,   133,    20,    21,
      22,    23,   136,   146,   145,    24,   173,   174,   184,   179,
      13,    70,   164,   176,   177,   166,   175,   162,     1,     2,
       3,     4,     5,   168,   222,   135,   167,   223,   224,     8,
       9,   178,   185,   186,   189,   188,     1,     2,     3,     4,
       5,     6,   197,     7,   203,   204,   211,     8,     9,   108,
     109,   110,   111,   112,   113,   187,    10,   228,   209,   190,
     237,   240,    11,   221,   205,   241,   202,   246,   179,   247,
     249,   151,   210,   207,    10,   257,   168,   219,   250,   161,
      11,   239,   155,   154,   206,     1,     2,     3,     4,     5,
       6,   157,     7,    70,   220,   215,     8,     9,   158,   156,
     159,    92,    71,   229,   208,   230,   160,   225,   244,   226,
     242,     0,   238,   243,     0,   183,   254,     0,     0,   219,
     254,     0,     0,    10,     0,     0,     0,     0,     0,    11,
       0,     0,     0,     0,    87,     0,    13,    14,    15,     0,
      16,    17,    18,    19,     0,    20,    21,    22,    23,     0,
       0,     0,    24,     1,     2,     3,     4,     5,     6,     0,
       7,     0,     0,     0,     8,     9,     0,     0,     0,     1,
       2,     3,     4,     5,     6,     0,     7,     0,     0,     0,
       8,     9,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    10,     0,     0,     0,   209,     0,    11,     0,     0,
       0,     0,     0,     0,     0,    14,    15,    10,    16,    17,
      18,    19,     0,    11,    21,    22,     0,     0,    87,     0,
      24,    14,    15,     0,    16,    17,    18,    19,     0,     0,
      21,    22,     0,     0,     0,     0,    24,     1,     2,     3,
       4,     5,     6,     0,     7,     0,     0,     0,     8,     9,
       0,     0,     0,     1,     2,     3,     4,     5,     6,     0,
       7,     0,     0,     0,     8,     9,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    10,     0,     0,     0,     0,
       0,    11,     0,     0,     0,     0,     0,     0,     0,    14,
      15,    10,    16,    17,    18,    19,     0,    11,    21,    22,
       0,     0,    87,     0,    24,     0,     1,     2,     3,     4,
       5,     6,   215,     7,     0,     0,     0,     8,     9,     0,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,     0,     0,     0,     0,     0,
      11,    69
  };

  const short
  parser::yycheck_[] =
  {
      10,    11,   131,    29,    14,     7,     0,   174,     7,     7,
      13,    21,    22,    46,     7,    40,    41,    20,   175,    52,
      23,   230,    47,    38,    39,     3,     4,     5,     6,     7,
       8,    48,    10,    50,    41,    29,    14,    15,     0,     3,
       4,     5,     6,     7,   211,   254,    27,    28,     3,     4,
       5,     6,     7,     8,    52,    10,    46,   214,    41,    14,
      15,    41,    52,    41,    74,    75,    76,    77,    78,    47,
     237,    70,    41,   240,    41,    68,    69,    41,     8,     9,
     247,    52,    85,    47,   213,    79,    41,    11,    12,    49,
      50,    52,    47,    31,   104,    44,    45,    52,    30,    54,
      55,    56,    29,    58,    59,    60,    61,    42,    63,    64,
      65,    66,    41,    50,    45,    70,    42,    42,    16,   145,
      54,   131,   132,    42,    42,   135,    49,   130,     3,     4,
       5,     6,     7,   136,    68,    50,    70,    71,    72,    14,
      15,    45,    42,    48,    50,    42,     3,     4,     5,     6,
       7,     8,    45,    10,    46,    50,    57,    14,    15,    32,
      33,    34,    35,    36,    37,   168,    41,    16,    45,   172,
      49,    49,    47,   199,   184,    46,   179,    46,   204,    42,
      46,    89,   191,   188,    41,   256,   189,   197,   239,   129,
      47,   217,   114,   107,   188,     3,     4,     5,     6,     7,
       8,   118,    10,   213,   198,    62,    14,    15,   120,   116,
     123,    46,    11,   204,   189,   209,   126,   201,   228,   201,
     221,    -1,   216,   226,    -1,   146,   252,    -1,    -1,   239,
     256,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    52,    -1,    54,    55,    56,    -1,
      58,    59,    60,    61,    -1,    63,    64,    65,    66,    -1,
      -1,    -1,    70,     3,     4,     5,     6,     7,     8,    -1,
      10,    -1,    -1,    -1,    14,    15,    -1,    -1,    -1,     3,
       4,     5,     6,     7,     8,    -1,    10,    -1,    -1,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    45,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    41,    58,    59,
      60,    61,    -1,    47,    64,    65,    -1,    -1,    52,    -1,
      70,    55,    56,    -1,    58,    59,    60,    61,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    70,     3,     4,     5,
       6,     7,     8,    -1,    10,    -1,    -1,    -1,    14,    15,
      -1,    -1,    -1,     3,     4,     5,     6,     7,     8,    -1,
      10,    -1,    -1,    -1,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    41,    58,    59,    60,    61,    -1,    47,    64,    65,
      -1,    -1,    52,    -1,    70,    -1,     3,     4,     5,     6,
       7,     8,    62,    10,    -1,    -1,    -1,    14,    15,    -1,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      47,    48
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,     3,     4,     5,     6,     7,     8,    10,    14,    15,
      41,    47,    52,    54,    55,    56,    58,    59,    60,    61,
      63,    64,    65,    66,    70,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    92,    93,    94,    95,    99,
     100,   104,   105,   107,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     136,   137,   138,   139,   146,   147,   153,   155,   120,    48,
     120,   135,   138,   120,    41,    41,    41,    41,    41,   138,
     120,   120,   138,   148,   149,   150,     0,    52,    77,    78,
     130,   130,   131,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   108,    38,    39,   109,    32,    33,
      34,    35,    36,    37,   110,    31,   111,    30,   112,    29,
     113,    27,    28,   114,     8,     9,   115,    11,    12,   116,
      40,    41,    47,    42,    48,    50,    41,   120,   120,    96,
     134,   135,   120,   120,    77,    45,    50,    68,    69,   138,
     151,    81,   120,   154,   122,   123,   124,   125,   126,   127,
     128,   129,   138,   134,   120,   133,   120,    70,   138,   143,
     144,   145,   152,    42,    42,    49,    42,    42,    45,    78,
     140,   141,   142,   149,    16,    42,    48,   138,    42,    50,
     138,    77,    86,    91,    91,    97,   121,    45,   156,   157,
     158,   159,   138,    46,    50,   120,    77,    86,   143,    45,
      84,    57,   106,    49,    50,    62,   101,   102,   103,   120,
      77,    78,    68,    71,    72,   146,   152,   160,    16,   140,
      77,    87,    88,    91,    98,   134,   121,    49,    77,    78,
      49,    46,   158,   138,   120,    85,    46,    42,    91,    46,
     103,    91,    89,    91,    78,    90,    85,    90
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    75,    76,    77,    77,    78,    78,    79,    79,    80,
      80,    81,    81,    82,    83,    83,    83,    84,    85,    85,
      85,    85,    85,    86,    87,    87,    89,    88,    90,    90,
      90,    91,    91,    92,    93,    93,    93,    93,    93,    93,
      93,    94,    95,    96,    97,    97,    98,    99,    99,   100,
     101,   101,   102,   102,   103,   103,   104,   104,   105,   106,
     106,   107,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   109,   109,   110,   110,   110,   110,   110,
     110,   111,   112,   113,   114,   114,   115,   115,   116,   116,
     117,   118,   119,   119,   120,   121,   121,   122,   122,   123,
     123,   124,   124,   125,   125,   126,   126,   127,   127,   128,
     128,   129,   129,   129,   130,   130,   131,   131,   131,   131,
     132,   132,   132,   133,   133,   134,   134,   135,   135,   136,
     136,   136,   136,   136,   137,   137,   138,   139,   140,   140,
     141,   141,   142,   142,   143,   143,   144,   144,   145,   145,
     146,   147,   148,   148,   149,   149,   150,   150,   151,   151,
     152,   153,   154,   155,   156,   156,   157,   157,   158,   158,
     159,   159,   160,   160,   160
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     0,     2,     1,     2,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     0,     0,     4,     3,     1,
       0,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     5,     9,     1,     3,     1,     1,     1,     2,     8,
       1,     0,     3,     1,     3,     3,     4,     1,     6,     2,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     2,     2,     1,     2,     1,     3,     4,     4,     1,
       1,     1,     3,     1,     0,     1,     0,     3,     1,     1,
       1,     1,     1,     1,     2,     3,     1,     5,     2,     4,
       1,     0,     3,     1,     2,     2,     1,     0,     3,     1,
       6,     2,     3,     1,     4,     2,     2,     0,     1,     1,
       1,     3,     1,     7,     1,     0,     3,     1,     3,     2,
       2,     0,     1,     1,     1
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
  "code_block", "opt_stmt_list", "stmt_list", "$@1", "stmt_list_tail",
  "code_block_or_stmt", "del_stmt", "cntrl_cnstrct", "loop_whl",
  "loop_for", "loop_for_init", "loop_for_cond", "loop_for_post_iter",
  "ret_stmt", "switch_stmt", "opt_switch_case_list", "switch_case_list",
  "switch_case", "brk_stmt", "if_stmt", "opt_else", "toss_stmt",
  "op_assign", "op_logic", "op_cond", "op_bitwise_xor", "op_bitwise_or",
  "op_bitwise_and", "op_bitwise_shift", "op_arithm_add", "op_arithm_mul",
  "op_unary_neg", "op_unary_logic_neg", "op_unary_inc", "expr",
  "lvl_logic", "lvl_cond", "lvl_bitwise_xor", "lvl_bitwise_or",
  "lvl_bitwise_and", "lvl_bitwise_shift", "lvl_arithm_add",
  "lvl_arithm_mul", "lvl_unary_neg", "lvl_unary_inc", "lvl_postfix",
  "primary", "opt_expr", "opt_expr_list", "expr_list", "literal",
  "arr_initializer", "nameNode", "enum_stmt", "enum_element",
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
       0,   223,   223,   230,   231,   235,   236,   240,   244,   251,
     256,   263,   267,   275,   282,   286,   290,   298,   305,   309,
     313,   317,   321,   329,   336,   341,   348,   347,   359,   363,
     364,   368,   372,   380,   388,   392,   396,   400,   404,   408,
     412,   419,   426,   433,   440,   445,   452,   459,   463,   470,
     477,   482,   488,   493,   500,   504,   511,   515,   522,   529,
     533,   540,   548,   552,   556,   560,   564,   568,   572,   576,
     580,   584,   588,   595,   599,   606,   610,   614,   618,   622,
     626,   633,   640,   647,   654,   658,   665,   669,   676,   680,
     687,   694,   701,   705,   713,   720,   724,   731,   735,   742,
     746,   753,   757,   764,   768,   775,   779,   786,   790,   797,
     801,   808,   812,   816,   823,   827,   834,   838,   842,   846,
     853,   857,   861,   868,   873,   879,   884,   890,   895,   902,
     906,   910,   914,   918,   926,   930,   938,   946,   953,   957,
     964,   969,   975,   980,   988,   992,   999,  1004,  1010,  1015,
    1022,  1030,  1037,  1042,  1049,  1053,  1060,  1065,  1071,  1075,
    1082,  1090,  1097,  1144,  1151,  1156,  1162,  1167,  1174,  1178,
    1220,  1225,  1231,  1235,  1239
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
#line 2541 "parser.cpp"

#line 1245 "bnf.y"
