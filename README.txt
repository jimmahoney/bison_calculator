
This is from 

  https://en.wikipedia.org/wiki/GNU_bison

an algebraic calculator definition and compilation example
which includes the creation and evaluation of a syntax tree - 
not just a numeric value passed along the way.

The files are

  expression.h         header file for syntax tree data structure
  expression.c         implementation of syntax tree
  lexer.l              Flex token specs
  parser.y             Bison language syntax specs
  main.c               call the Flex scanner and Bison parser
                       to make a syntax tree, then evaluate the tree
  makefile             compile everything

bison, flex, c versions 

  I'm using the Oct 2015 version of flex and g++ that
  are part of Mac OS 10.10.5. However, the default bison 2.3
  doesn't understand the directives (i.e. "%code") from 
  the wikipedia example, so I manually installed 3.0.5 
  (wget .../bison-3.0.4.tar.gz; tar zxf ...; ./configure; make; make install)
  to /usr/local/bin .

  $ /usr/local/bin/bison --version
  bison (GNU Bison) 3.0.4

  $ flex --version
  flex 2.5.35 Apple(flex-31)

  $ g++ --version
  Apple LLVM version 7.0.0 (clang-700.1.76)
  Target: x86_64-apple-darwin14.5.0

modifications

  I've also made some minor modifications to the code:
    * adjusted C styling to suit my preferences
    * removed yyerror() declration to avoid a compiler 
      "duplicate symbol" error (apparently bison generates
      one of these too, and I didn't track down how 
      to keep the new one instead)
    * added "-x c " compiler flag to avoid compiler C++ warnings

running it

  $ make
  







