/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SQLGRAM_H_INCLUDED
# define YY_YY_SQLGRAM_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CREATE = 258,
    TABLE = 259,
    DROP = 260,
    INSERT = 261,
    INTO = 262,
    VALUES = 263,
    SELECT = 264,
    FROM = 265,
    WHERE = 266,
    ALL = 267,
    DISTINCT = 268,
    AS = 269,
    JOIN = 270,
    INNER = 271,
    CROSS = 272,
    LEFT = 273,
    RIGHT = 274,
    OUTER = 275,
    NATURAL = 276,
    ON = 277,
    USING = 278,
    AND = 279,
    OR = 280,
    NOT = 281,
    LESS_EQ = 282,
    GREATER_EQ = 283,
    NOT_EQ = 284,
    IDENT = 285,
    STRING = 286,
    INTNUMBER = 287,
    FLOATNUMBER = 288,
    Op = 289,
    UMINUS = 290
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 15 "sqlgram.y" /* yacc.c:1909  */

    char *sval;
    int  ival;
    float fval;
    Node *node;

#line 97 "sqlgram.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (yyscan_t yyscaninfo);

#endif /* !YY_YY_SQLGRAM_H_INCLUDED  */
