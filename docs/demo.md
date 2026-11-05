# Demo

## Lexer
Lexer test is a REPL-like program where for a valid program statement test displays every token of it. For example, consider simple declaration of variables:
```
var a = 3, b = 4, c = 5
```

The token stream for given statement will be like this, as taken from test output:
<details>
<summary>Token stream</summary>

```
token var, type keyword
token a, type name
token (null), type =
token 3, type number
token (null), type ,
token b, type name
token (null), type =
token 4, type number
token (null), type ,
token c, type name
token (null), type =
token 5, type number
token (null), type delimiter
token (null), type end of tokens
```
</details>

## Parser
Parser test is also REPL-like, but outputs AST of input valid statement. The AST is not typed, just tree-like representation of code, with minimal work on it. Consider following example statement:
```
if (x) var hello = (1 - x)*call("hello")
```

The AST for given statement, as taken from test output:
<details>
<summary>AST</summary>

```
NODE_LIST:
  NODE_IF
    NODE_NAME 'x'
    NODE_LIST:
      NODE_VAR_DECL name='hello' modifier=0x0
        NODE_BINARYOP op='*'
          NODE_BINARYOP op='-'
            NODE_NUMBER value=1
            NODE_NAME 'x'
          NODE_FN_CALL
            NODE_NAME 'call'
            NODE_LIST:
              NODE_STRING 'hello'
```
</details>

## Typed AST
Consider following code (available [here](examples/example.fls)):
<details>
<summary>Source code</summary>

```unnamedlang
var x = 5
whl (x > 0)
{
    print(x)
    x = x - 1
}

fn print(var x) {
    x = 3
    ret x
}

~ example of a class definition
class Example {
    fn change(Number val) {
        var x = 3
        if (x > 0)
            a = val
        b = x
        3+5

        ret a
    }
    
    var a
}
```
</details>
<br>

Typed AST test will be able to transform this source code through pipeline's front-end. As we know, it outputs Typed AST, and this test will pretty-print it:
<details>
<summary>Typed AST</summary>

```
NodeList
  NodeList
    VarDecl
      name:
        x
      value:
        NumVal
          5.000000
      type:
        Number
  LoopWhl
    cond:
      BinaryExpr
        op: >
        lhs:
          NameExpr
            x
        type:
          dynamic
        rhs:
          NumVal
            0.000000
        type:
          Number
    body:
      NodeList
        CallExpr
          callee:
            NameExpr
              print
          args:
            NameExpr
              x
        VarAssign
          op: =
          lhs:
            NameExpr
              x
          type:
            dynamic
          rhs:
            BinaryExpr
              op: -
              lhs:
                NameExpr
                  x
              type:
                dynamic
              rhs:
                NumVal
                  1.000000
              type:
                Number
          type:
            dynamic
  FnDef
(id: 0)
    NodeList
      VarAssign
        op: =
        lhs:
          NameExpr
            x
        type:
          dynamic
        rhs:
          NumVal
            3.000000
        type:
          Number
      StmtRet
        expr:
          NameExpr
            x
  ClassDef
(id: 5)
    methods:
      FnDef
(id: 1)
        NodeList
          NodeList
            VarDecl
              name:
                x
              value:
                NumVal
                  3.000000
              type:
                Number
          StmtIf
            cond:
              BinaryExpr
                op: >
                lhs:
                  NameExpr
                    x
                type:
                  dynamic
                rhs:
                  NumVal
                    0.000000
                type:
                  Number
            body:
              VarAssign
                op: =
                lhs:
                  NameExpr
                    a
                type:
                  dynamic
                rhs:
                  NameExpr
                    val
                type:
                  dynamic
          VarAssign
            op: =
            lhs:
              NameExpr
                b
            type:
              dynamic
            rhs:
              NameExpr
                x
            type:
              dynamic
          BinaryExpr
            op: +
            lhs:
              NumVal
                3.000000
            type:
              Number
            rhs:
              NumVal
                5.000000
            type:
              Number
          StmtRet
            expr:
              NameExpr
                a
    fields:
      a
```
</details>

## TAC test
TAC test utilizes pipeline fully (as of now) and runs source code from its original form to optimized TAC MIR. Test displays both unoptimized and optimized (with static optimizations) forms. Running test with input code from previous test (also available [here](examples/example.fls)) will show two MIR's:
<details>
<summary>TAC MIR <b>before</b> optimizations</summary>

```
block 0:
@0 = load 5.000000
store @0, x0_0
jmp blk1

block 1:
@19 = phi blk0, x0_0, blk2, x0_1
store @19, x0_6
@1 = load @19
@2 = gt @1, 0.000000
condjmp @2, blk2, blk3

block 2:
@3 = load x0_6
param @3
@4 = call fn0
@5 = load x0_6
@6 = sub @5, 1.000000
store @6, x0_1
jmp blk1

block 3:
exit
```

</details>
<br>

<details>
<summary>TAC MIR <b>after</b> optimizations</summary>

```
block 0:
dead
dead
jmp blk1

block 1:
@19 = phi blk0, 5.000000, blk2, @6
dead
dead
@2 = gt @19, 0.000000
condjmp @2, blk2, blk3

block 2:
dead
param @19
@4 = call fn0
dead
@6 = sub @19, 1.000000
dead
jmp blk1

block 3:
exit
```
</details>
<br>
While this pretty print may look ugly, it corresponds to the correct optimized CFG of the program. It is important to note that this is just an optimized MIR - without any preparation for backend or runtime.