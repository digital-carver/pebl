/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PEBL_AND = 258,
     PEBL_ADD = 259,
     PEBL_ARGLIST = 260,
     PEBL_ASSIGN = 261,
     PEBL_BREAK = 262,
     PEBL_COMMA = 263,
     PEBL_DEFINE = 264,
     PEBL_DIVIDE = 265,
     PEBL_DOT = 266,
     PEBL_ELSE = 267,
     PEBL_ELSEIF = 268,
     PEBL_END = 269,
     PEBL_EOF = 270,
     PEBL_EQ = 271,
     PEBL_FUNCTION = 272,
     PEBL_FUNCTIONS = 273,
     PEBL_GE = 274,
     PEBL_GT = 275,
     PEBL_IF = 276,
     PEBL_IFELSE = 277,
     PEBL_LAMBDAFUNCTION = 278,
     PEBL_LBRACE = 279,
     PEBL_LBRACKET = 280,
     PEBL_LE = 281,
     PEBL_LIBRARYFUNCTION = 282,
     PEBL_LISTHEAD = 283,
     PEBL_LISTITEM = 284,
     PEBL_LOOP = 285,
     PEBL_LPAREN = 286,
     PEBL_LT = 287,
     PEBL_MULTIPLY = 288,
     PEBL_NE = 289,
     PEBL_NEWLINE = 290,
     PEBL_NOT = 291,
     PEBL_OR = 292,
     PEBL_POWER = 293,
     PEBL_RETURN = 294,
     PEBL_RBRACE = 295,
     PEBL_RBRACKET = 296,
     PEBL_RPAREN = 297,
     PEBL_SUBTRACT = 298,
     PEBL_SEMI = 299,
     PEBL_STATEMENTS = 300,
     PEBL_START = 301,
     PEBL_VARIABLEDATUM = 302,
     PEBL_VARLIST = 303,
     PEBL_WHILE = 304,
     PEBL_FLOAT = 305,
     PEBL_INTEGER = 306,
     PEBL_STRING = 307,
     PEBL_SYMBOL = 308,
     PEBL_LOCALVAR = 309,
     PEBL_GLOBALVAR = 310,
     PEBL_FUNCTIONNAME = 311,
     PEBL_UMINUS = 312
   };
#endif
#define PEBL_AND 258
#define PEBL_ADD 259
#define PEBL_ARGLIST 260
#define PEBL_ASSIGN 261
#define PEBL_BREAK 262
#define PEBL_COMMA 263
#define PEBL_DEFINE 264
#define PEBL_DIVIDE 265
#define PEBL_DOT 266
#define PEBL_ELSE 267
#define PEBL_ELSEIF 268
#define PEBL_END 269
#define PEBL_EOF 270
#define PEBL_EQ 271
#define PEBL_FUNCTION 272
#define PEBL_FUNCTIONS 273
#define PEBL_GE 274
#define PEBL_GT 275
#define PEBL_IF 276
#define PEBL_IFELSE 277
#define PEBL_LAMBDAFUNCTION 278
#define PEBL_LBRACE 279
#define PEBL_LBRACKET 280
#define PEBL_LE 281
#define PEBL_LIBRARYFUNCTION 282
#define PEBL_LISTHEAD 283
#define PEBL_LISTITEM 284
#define PEBL_LOOP 285
#define PEBL_LPAREN 286
#define PEBL_LT 287
#define PEBL_MULTIPLY 288
#define PEBL_NE 289
#define PEBL_NEWLINE 290
#define PEBL_NOT 291
#define PEBL_OR 292
#define PEBL_POWER 293
#define PEBL_RETURN 294
#define PEBL_RBRACE 295
#define PEBL_RBRACKET 296
#define PEBL_RPAREN 297
#define PEBL_SUBTRACT 298
#define PEBL_SEMI 299
#define PEBL_STATEMENTS 300
#define PEBL_START 301
#define PEBL_VARIABLEDATUM 302
#define PEBL_VARLIST 303
#define PEBL_WHILE 304
#define PEBL_FLOAT 305
#define PEBL_INTEGER 306
#define PEBL_STRING 307
#define PEBL_SYMBOL 308
#define PEBL_LOCALVAR 309
#define PEBL_GLOBALVAR 310
#define PEBL_FUNCTIONNAME 311
#define PEBL_UMINUS 312




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 30 "src/base/grammar.y"
typedef union YYSTYPE {
  
  long int    iValue;  /* For the lexical analyser. NUMBER tokens */
  long double fValue;
  char        *strValue; 
  PNode       *exp;    /* For expressions. */
  char        *symbol; /* The name of a variable*/
  char        *function; 
} YYSTYPE;
/* Line 1318 of yacc.c.  */
#line 161 "src/base/grammar.tab.hpp"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



