# Syntax

This file describes the syntax of Piper using BNF (or at least a version of it, I may not be doing it 100% strictly correctly). If you have never read BNF before, this probably isn't the right document for you. Whenever I write friendlier documentation I should link that here.

## Identifier

identifier ::= [a-zA-Z_][a-zA-Z0-9_]*

Identifiers make up valid variable names, namespace fragments, etc. They are used quite a lot.

## File

file ::= <import-statement>* <file-level-annotation>* ( <struct-file> | <function-file> )

struct-file ::= <struct-level-annotation>* <struct>

function-file ::= <function-level-annotations>? <function>

A file has a very strict order, enforced for purposes of readability and consistency. While most parts are optional, when they exist they must be in a specific order.

A file can only define a single thing, and it must be the same name as the file itself. TODO: Should I make the top-level objects name implied? Is that too implicit?

### Import Statements

import-statement ::= 'import' <namespace> ( 'as' <identifier> )?

namespace ::= <identifier> ( '.' <identifier> )*

The import statement will load the top-level symbol in the given namespace and makes it available in this file. It does not transitively load namespaces that are loaded by the other file. If you give an 'as' identifier, the top-level symbol will be available under the new name rather than its actual name.

### File-Level Annotations

file-level-annotation ::= '@' ( <namespace-annotation> )

File level annotations are annotations that affect the entire file. Currently there is only one, but I expect more to be added.

#### Namespace

namespace-annotation ::= 'namespace' '(' <namespace> ')'

The namespace annotation sets what namespace this file is considered to be under. A single file can only be in a single namespace. The namespace **should** be reflective of the file path, though the exact mechanism and how far it will be enforced is still up in the air.

### Struct-Level Annotations

struct-level-annotation ::= '@' ( <implements-annotation> | <mixin-annotation> | <testclass-annotation> )

Struct level annotations are annotations that apply to the struct at a data level. They are currently ill-defined but I hope to flesh out the precise mechanics soon.

#### Implements

implements-annotation ::= 'implements' '(' namespace ')'

This should annotate that this struct is meant to be an implementation of an interface. This is tricky to fully define in a separate-code-and-data world.

#### Mixin

mixin-annotation ::= 'mixin' '(' namespace ')'

The resulting type has all of the data members of this struct as well as the data members of the "mixin" struct. If names conflict, this is currently an error (I plan on adding a mechanism to disambiguate soon).

#### Test Class

testclass-annotation ::= 'testclass' '(' namespace ')'

Annotate what class is responsible for testing this one. This gets trickier in a separate-code-and-data world, so it needs to be revisited once that is more fully fleshed out.

### Struct

struct-declaration := 'struct' <identifier> <struct-body>

struct-body := '{' <struct-body-declaration>+ '}'

struct-body-declaration := <identifier> ':' <type> ';'

Structures are data-only.

### Function

function-declaration := <function-header> <block>

function-header := 'function' <identifier> '(' <formal-parameter-list> ')' ':' <type>

formal-parameter-list := <formal-parameter>*

formal-parameter := <identifier> ':' <type>

### Statements

statement := <return-statement>
    | <expression-statement>
    | <block>
    | <if-then-statement>
    | <if-then-else-statement>
    | <if-then-elseif-statement>
    | <if-then-elseif-else-statement>
    | <switch-statement>
    | <do-statement>
    | <break-statement>
    | <continue-statement>
    | <for-statement>

#### Return Statement

return-statement := 'return' <expression> ';'

#### Expression Statement

expression-statement := <expression> ';'

#### Block

block := '{' <statement>* '}'

#### If Statements

if-then-statement := 'if' '(' <expression> ')' <statement>

if-then-else-statement := 'if' '(' <expression> ')' <statement> 'else' <statement>

if-then-elseif-statement := 'if' '(' <expression> ')' <statement> ( 'else if' '(' <expression> ')' <statement> )+

if-then-elseif-else-statement := 'if' '(' <expression> ')' <statement> ( 'else if' '(' <expression> ')' <statment> )+ 'else' <statement>

#### Switch Statement

switch-statement := 'switch' '(' <expression> ')' '{' <switch-case>+ '}'

switch-case := 'case' <expression> ':' <statement>
    | 'default' ':' <statement>

#### Do Statement

do-statement := 'do' <statement> 'while' '(' <expression> ')' ';'

#### Break Statement

break-statement := 'break' ';'

#### Continue Statement

continue-statement := 'continue' ';'

#### For Statement

for-statement := 'for' '(' <expression>? ';' <expression> ';' <expression> ')' <statement>

#### While Statement

while-statement := 'while' '(' <expression> ')' <statement>

### Expressions

#### Expression

expression := <assignment-expression>

#### Assignment Expression

assignment-expression := <conditional-expression> | <assignment>

assignment := <left-hand-side> <assignment-operator> <assignment-expression>

left-hand-side := <expression-name> | <field-access> | <array-access>

assignment-operator := '=' | '*=' | '/=' | '%=' | '+=' | '-=' | '<<=' | '>>=' | '>>>=' | '&=' | '^=' | '|='

#### Conditional Expression

conditional-expression := <conditional-or-expression>
    | <conditional-or-expression> '?' <expression> ':' <conditional-expression>

conditional-or-expression := <conditional-and-expression>
    | <conditional-or-expression> '||' <conditional-and-expression>

conditional-and-expression := <incusive-or-expression>
    | <conditional-and-expression>
    | <exclusive-or-experssion>

inclusive-or-expression := <exclusive-or-expression>
    | <inclusive-or-expression>
    | <exclusive-or-expression>

exclusive-or-expression := <and-expression>
    | <exclusive-or-expression> '^' <and-expression>

and-expression := <equality-expression>
    | <and-expression> '&' <equality-expression>

equality-expression := <relational-expression>
    | <equality-expression> '==' <relational-expression>
    | <equality-expression> '!=' <relational-expression>

relational-expression := <shift-expression>
    | <relational-expression> '<' <shift-expression>
    | <relational-expression> '>' <shift-expression>
    | <relational-expression> '<=' <shift-expression>
    | <relational-expression> '>=' <shift-expression>
    | <relational-expression> 'instanceof' <reference-type>

shift-expression := <additive-expression>
    | <shift-expression> '<<' <additive-expression>
    | <shift-expression> '>>' <additive-expression>
    | <shift-expression> '>>>' <additive-expression>

additive-expression

### Misc

local-variable-declaration-statement := <local-variable-declaration> ';'

local-variable-declaration := <type> <variable-declarators>

statement-expression := <assignment>
    | <preincrement-expression>
    | <postincrement-expression>
    | <predecrement-expression>
    | <postdecrement-expression>
    | <method-invocation>
    | <class-instance-creation-expression>

conditional-and-expression := <inclusive-or-expression> | <conditional-and-expression> | <exclusive-or-expression>



### Function