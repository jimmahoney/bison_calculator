# bison_calculator

A sample bison compiler based on https://en.wikipedia.org/wiki/GNU_bison - 
an algebraic calculator definition and compilation example
including the creation and evaluation of a syntax tree.

## files

```
  expression.h         header file for syntax tree data structure
  expression.c         implementation of syntax tree
  lexer.l              Flex token specs
  parser.y             Bison language syntax specs
  main.c               call the Flex scanner and Bison parser
                       to make a syntax tree, then evaluate the tree
  makefile             compile everything
  README.md            you're looking at it
  LICENSE
```

## software requirements

This works with the versions of flex and g++ which
are part of Mac OS 10.10.5. However, the default bison 2.3
doesn't understand the directives (i.e. "%code") from 
the wikipedia example, so I manually installed 3.0.5 
(wget .../bison-3.0.4.tar.gz; tar zxf ...; ./configure; make; make install)
to /usr/local/bin .

```
  $ /usr/local/bin/bison --version
  bison (GNU Bison) 3.0.4

  $ flex --version
  flex 2.5.35 Apple(flex-31)

  $ g++ --version
  Apple LLVM version 7.0.0 (clang-700.1.76)
  Target: x86_64-apple-darwin14.5.0
```

## change history

* modified makefile to use bison in /usr/local/bin/bison
* adjusted C styling to suit my preferences
* removed yyerror() declration to avoid a compiler "duplicate symbol" error (apparently bison generates one of these too, and I didn't track down how to keep the new one instead)
* added "-x c " compiler flag to avoid compiler C++ warnings
# graphviz representation of the parse tree is now output to tree.dot in main.c

## running it

```
    $ make
    flex lexer.l
    /usr/local/bin/bison parser.y
    g++ -g -x c -ansi lexer.c parser.c expression.c main.c -o test

    $ ./test  # also creates tree.dot
    Result of ' 4 + 2*10 + 3*( 5 + 1 ) ' is 42

    # generate parse tree image from tree.dot
    $ dot -Tpng < tree.dot > tree.png

```

  
