/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "sqlgram.y" /* yacc.c:339  */

#include <stdio.h>
#include "sqlparser.h"
#include "sqlscanner.h"
#include "node.h"
#include "dictionary.h"

void yyerror(yyscan_t yyscaninfo, const char *msg);

#line 76 "sqlgram.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "sqlgram.h".  */
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
#line 15 "sqlgram.y" /* yacc.c:355  */

    char *sval;
    int  ival;
    float fval;
    Node *node;

#line 159 "sqlgram.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (yyscan_t yyscaninfo);

#endif /* !YY_YY_SQLGRAM_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 175 "sqlgram.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  18
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   177

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  86
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  145

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   290

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    42,     2,     2,
      44,    45,    40,    38,    48,    39,    46,    41,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    47,
      35,    36,    34,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    37,
      43
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    73,    73,    80,    84,    88,    92,    97,   106,   114,
     124,   129,   136,   139,   143,   148,   152,   157,   161,   167,
     170,   174,   180,   183,   188,   192,   198,   201,   206,   210,
     214,   218,   222,   226,   230,   235,   239,   243,   247,   251,
     255,   259,   263,   267,   273,   277,   282,   286,   291,   295,
     300,   304,   309,   313,   317,   324,   327,   332,   336,   342,
     345,   351,   354,   358,   364,   367,   372,   376,   382,   385,
     390,   397,   405,   409,   414,   419,   423,   428,   432,   437,
     445,   450,   455,   460,   465,   473,   481
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CREATE", "TABLE", "DROP", "INSERT",
  "INTO", "VALUES", "SELECT", "FROM", "WHERE", "ALL", "DISTINCT", "AS",
  "JOIN", "INNER", "CROSS", "LEFT", "RIGHT", "OUTER", "NATURAL", "ON",
  "USING", "AND", "OR", "NOT", "LESS_EQ", "GREATER_EQ", "NOT_EQ", "IDENT",
  "STRING", "INTNUMBER", "FLOATNUMBER", "'>'", "'<'", "'='", "Op", "'+'",
  "'-'", "'*'", "'/'", "'%'", "UMINUS", "'('", "')'", "'.'", "';'", "','",
  "$accept", "top_stmt", "stmt", "create_stmt", "drop_stmt", "insert_stmt",
  "select_stmt", "select_clause", "target_opts", "target_list",
  "target_element", "alias_opt", "from_clause", "from_list",
  "where_clause", "a_expr", "c_expr", "column_name", "table_ref",
  "table_info", "table_join", "left_or_right_opt", "left_or_right",
  "outer_opt", "inner_cross_join_opt", "join_condition_opt",
  "join_condition", "attr_name_list_opt", "attr_name_list",
  "multi_values_list", "values_list", "row_values", "columndef_list",
  "column_def", "tablename", "attr_name", "expr_name", "attr_type",
  "constValues", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,    62,    60,    61,   289,    43,    45,
      42,    47,    37,   290,    40,    41,    46,    59,    44
};
# endif

#define YYPACT_NINF -112

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-112)))

#define YYTABLE_NINF -62

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-62)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     147,     2,     6,    11,    15,    55,    16,  -112,  -112,  -112,
    -112,    39,    39,    39,  -112,  -112,    64,   -18,  -112,  -112,
    -112,    38,  -112,    45,   -19,    82,   109,  -112,  -112,  -112,
    -112,   109,  -112,    51,  -112,    37,  -112,  -112,    67,  -112,
      71,    71,   115,   120,   111,   128,  -112,  -112,   -11,   109,
    -112,    18,  -112,   -18,    94,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,  -112,  -112,
      94,  -112,    77,  -112,   131,   106,  -112,   118,   119,   -19,
    -112,  -112,  -112,  -112,    25,   143,   150,  -112,    56,  -112,
    -112,    18,    76,    92,    92,    92,    92,    92,    92,     8,
       8,  -112,  -112,  -112,  -112,  -112,    71,  -112,  -112,  -112,
      71,    75,   121,  -112,   -11,   128,   151,  -112,  -112,   152,
     -19,  -112,  -112,   110,  -112,   118,  -112,   -19,   -19,    65,
    -112,    75,  -112,  -112,    65,   109,   124,  -112,  -112,  -112,
    -112,    56,    71,   112,  -112
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,    12,     0,     0,     3,     4,     5,
       6,     0,     0,     0,    13,    14,    22,     0,     1,     2,
      80,     0,     8,    68,     0,    26,     0,    82,    84,    85,
      86,     0,    18,    11,    15,    19,    28,    44,    46,    45,
       0,     0,     0,     0,    23,    24,    48,    49,    19,     0,
      10,    43,    29,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    17,    21,
       0,    81,     0,    77,     0,     0,    70,     0,     0,     0,
      62,    63,    57,    58,    55,    59,     0,    50,    27,    16,
      20,    41,    42,    38,    39,    40,    35,    36,    37,    30,
      31,    32,    33,    34,    47,     7,     0,    83,    79,    69,
       0,     0,     9,    72,    19,    25,     0,    56,    60,     0,
       0,    78,    71,     0,    75,     0,    51,     0,     0,    64,
      74,     0,    73,    54,     0,     0,     0,    52,    65,    76,
      53,    66,     0,     0,    67
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -112,  -112,  -112,  -112,  -112,  -112,   127,  -112,  -112,  -112,
     122,   -46,  -112,  -112,  -112,   -26,  -112,  -112,    93,  -111,
    -112,  -112,    87,  -112,  -112,  -112,    40,  -112,    31,  -112,
      52,  -112,  -112,    70,   125,   -40,   -28,  -112,  -107
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,     9,    10,    16,    17,    33,
      34,    68,    25,    44,    50,    35,    36,    37,    45,    46,
      47,   116,    85,   119,    86,   137,   138,    42,    75,   112,
     113,   123,    72,    73,    48,    74,    38,   108,    39
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      51,    76,    87,    54,   124,    52,    11,    69,    26,   129,
      12,    20,    27,    28,    29,    30,   133,   134,    13,    27,
      69,    31,    32,    88,   139,    43,    90,    14,    15,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,    82,    83,    57,    58,    59,    65,    66,
      67,    54,    60,    61,    62,    18,    63,    64,    65,    66,
      67,    55,    56,    19,    57,    58,    59,    27,   126,    20,
     122,    60,    61,    62,    24,    63,    64,    65,    66,    67,
      55,    56,    40,    57,    58,    59,    69,   135,   136,    41,
      60,    61,    62,    49,    63,    64,    65,    66,    67,    53,
      55,    71,    76,    57,    58,    59,    28,    29,    30,   141,
      60,    61,    62,    70,    63,    64,    65,    66,    67,   -62,
     -62,   -62,   105,    77,    27,   106,   -62,   -62,   -62,     4,
      63,    64,    65,    66,    67,    26,    21,    22,    23,    27,
      28,    29,    30,   -61,    80,    81,    82,    83,    31,    84,
       1,   109,     2,     3,   110,   130,     4,   144,   131,    79,
     110,   107,   111,   118,   114,   120,   127,   128,   142,   125,
      78,   117,   115,   143,   140,    89,   121,   132
};

static const yytype_uint8 yycheck[] =
{
      26,    41,    48,    14,   111,    31,     4,    35,    26,   120,
       4,    30,    30,    31,    32,    33,   127,   128,     7,    30,
      48,    39,    40,    49,   131,    44,    54,    12,    13,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    70,    18,    19,    27,    28,    29,    40,    41,
      42,    14,    34,    35,    36,     0,    38,    39,    40,    41,
      42,    24,    25,    47,    27,    28,    29,    30,   114,    30,
     110,    34,    35,    36,    10,    38,    39,    40,    41,    42,
      24,    25,    44,    27,    28,    29,   114,    22,    23,    44,
      34,    35,    36,    11,    38,    39,    40,    41,    42,    48,
      24,    30,   142,    27,    28,    29,    31,    32,    33,   135,
      34,    35,    36,    46,    38,    39,    40,    41,    42,    27,
      28,    29,    45,     8,    30,    48,    34,    35,    36,     9,
      38,    39,    40,    41,    42,    26,    11,    12,    13,    30,
      31,    32,    33,    15,    16,    17,    18,    19,    39,    21,
       3,    45,     5,     6,    48,    45,     9,    45,    48,    48,
      48,    30,    44,    20,    45,    15,    15,    15,    44,    48,
      43,    84,    79,   142,   134,    53,   106,   125
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     5,     6,     9,    50,    51,    52,    53,    54,
      55,     4,     4,     7,    12,    13,    56,    57,     0,    47,
      30,    83,    83,    83,    10,    61,    26,    30,    31,    32,
      33,    39,    40,    58,    59,    64,    65,    66,    85,    87,
      44,    44,    76,    44,    62,    67,    68,    69,    83,    11,
      63,    64,    64,    48,    14,    24,    25,    27,    28,    29,
      34,    35,    36,    38,    39,    40,    41,    42,    60,    85,
      46,    30,    81,    82,    84,    77,    84,     8,    55,    48,
      16,    17,    18,    19,    21,    71,    73,    60,    64,    59,
      85,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    85,    45,    48,    30,    86,    45,
      48,    44,    78,    79,    45,    67,    70,    71,    20,    72,
      15,    82,    84,    80,    87,    48,    60,    15,    15,    68,
      45,    48,    79,    68,    68,    22,    23,    74,    75,    87,
      75,    64,    44,    77,    45
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    51,    51,    51,    51,    52,    53,    54,
      55,    56,    57,    57,    57,    58,    58,    59,    59,    60,
      60,    60,    61,    61,    62,    62,    63,    63,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    65,    65,    66,    66,    67,    67,
      68,    68,    69,    69,    69,    70,    70,    71,    71,    72,
      72,    73,    73,    73,    74,    74,    75,    75,    76,    76,
      77,    77,    78,    78,    79,    80,    80,    81,    81,    82,
      83,    84,    85,    86,    87,    87,    87
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     6,     3,     6,
       4,     2,     0,     1,     1,     1,     3,     2,     1,     0,
       2,     1,     0,     2,     1,     3,     0,     2,     1,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     1,     3,     1,     1,
       2,     4,     5,     6,     5,     0,     1,     1,     1,     0,
       1,     0,     1,     1,     0,     1,     2,     4,     0,     3,
       1,     3,     1,     3,     3,     1,     3,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (yyscaninfo, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, yyscaninfo); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, yyscan_t yyscaninfo)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yyscaninfo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, yyscan_t yyscaninfo)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yyscaninfo);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, yyscan_t yyscaninfo)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              , yyscaninfo);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, yyscaninfo); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, yyscan_t yyscaninfo)
{
  YYUSE (yyvaluep);
  YYUSE (yyscaninfo);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t yyscaninfo)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, yyscaninfo);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 74 "sqlgram.y" /* yacc.c:1646  */
    {
                        ParserTree* parserTree = (ParserTree*)yyget_extra(yyscaninfo);
                        parserTree->root = (yyvsp[-1].node);
                        parserTree->result = 0;
                    }
#line 1378 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 3:
#line 81 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = (yyvsp[0].node);
                    }
#line 1386 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 4:
#line 85 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = (yyvsp[0].node);
                    }
#line 1394 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 5:
#line 89 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = (yyvsp[0].node);
                    }
#line 1402 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 6:
#line 93 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1410 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 7:
#line 98 "sqlgram.y" /* yacc.c:1646  */
    {                       
                        CreateStmt* createStmt = NewNode(CreateStmt);
                        createStmt->tableName = (yyvsp[-3].sval);
                        createStmt->colList = (yyvsp[-1].node);

                        (yyval.node) = (Node*)createStmt;
                    }
#line 1422 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 8:
#line 107 "sqlgram.y" /* yacc.c:1646  */
    {
                        DropStmt* dropStmt = NewNode(DropStmt);
                        dropStmt->tableName = (yyvsp[0].sval);

                        (yyval.node) = (Node*)dropStmt;
                    }
#line 1433 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 9:
#line 115 "sqlgram.y" /* yacc.c:1646  */
    {
                        InsertStmt* insertStmt = NewNode(InsertStmt);
                        insertStmt->tableName = (yyvsp[-3].sval);
                        insertStmt->attrNameList = (yyvsp[-2].node);
                        insertStmt->valuesList = (yyvsp[0].node);

                        (yyval.node) = (Node*)insertStmt;
                    }
#line 1446 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 10:
#line 125 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1454 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 11:
#line 130 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1462 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 12:
#line 136 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1470 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 13:
#line 140 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1478 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 14:
#line 144 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1486 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 15:
#line 149 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1494 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 16:
#line 153 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1502 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 17:
#line 158 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1510 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 18:
#line 162 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1518 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 19:
#line 167 "sqlgram.y" /* yacc.c:1646  */
    {
                    
                }
#line 1526 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 20:
#line 171 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1534 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 21:
#line 175 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1542 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 22:
#line 180 "sqlgram.y" /* yacc.c:1646  */
    {
                   
                }
#line 1550 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 23:
#line 184 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1558 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 24:
#line 189 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1566 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 25:
#line 193 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1574 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 26:
#line 198 "sqlgram.y" /* yacc.c:1646  */
    {
                    
                }
#line 1582 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 27:
#line 202 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1590 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 28:
#line 207 "sqlgram.y" /* yacc.c:1646  */
    {
                        
                    }
#line 1598 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 29:
#line 211 "sqlgram.y" /* yacc.c:1646  */
    {
 
                    }
#line 1606 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 30:
#line 215 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1614 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 31:
#line 219 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1622 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 32:
#line 223 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1630 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 33:
#line 227 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1638 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 34:
#line 231 "sqlgram.y" /* yacc.c:1646  */
    {
 
                    }
#line 1646 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 35:
#line 236 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1654 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 36:
#line 240 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1662 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 37:
#line 244 "sqlgram.y" /* yacc.c:1646  */
    {
 
                    }
#line 1670 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 38:
#line 248 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1678 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 39:
#line 252 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1686 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 40:
#line 256 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1694 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 41:
#line 260 "sqlgram.y" /* yacc.c:1646  */
    {
 
                    }
#line 1702 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 42:
#line 264 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1710 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 43:
#line 268 "sqlgram.y" /* yacc.c:1646  */
    {

                    }
#line 1718 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 44:
#line 274 "sqlgram.y" /* yacc.c:1646  */
    {
            
        }
#line 1726 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 45:
#line 278 "sqlgram.y" /* yacc.c:1646  */
    {
                       
        }
#line 1734 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 46:
#line 283 "sqlgram.y" /* yacc.c:1646  */
    {
            
        }
#line 1742 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 47:
#line 287 "sqlgram.y" /* yacc.c:1646  */
    {
            
        }
#line 1750 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 48:
#line 292 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1758 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 49:
#line 296 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1766 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 50:
#line 301 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1774 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 51:
#line 305 "sqlgram.y" /* yacc.c:1646  */
    {

                }
#line 1782 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 52:
#line 310 "sqlgram.y" /* yacc.c:1646  */
    {
                
            }
#line 1790 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 53:
#line 314 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1798 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 54:
#line 318 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1806 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 55:
#line 324 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1814 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 56:
#line 328 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1822 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 57:
#line 333 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1830 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 58:
#line 337 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1838 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 59:
#line 342 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1846 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 60:
#line 346 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1854 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 61:
#line 351 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1862 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 62:
#line 355 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1870 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 63:
#line 359 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1878 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 64:
#line 364 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1886 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 65:
#line 368 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1894 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 66:
#line 373 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1902 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 67:
#line 377 "sqlgram.y" /* yacc.c:1646  */
    {

            }
#line 1910 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 68:
#line 382 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = NULL;
                    }
#line 1918 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 69:
#line 386 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = (yyvsp[-1].node);
                    }
#line 1926 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 70:
#line 391 "sqlgram.y" /* yacc.c:1646  */
    {
                        AttrName* node = NewNode(AttrName);
                        node->attrName = (yyvsp[0].sval);

                        (yyval.node) = AppendNewList((Node*)node);
                    }
#line 1937 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 71:
#line 398 "sqlgram.y" /* yacc.c:1646  */
    {
                        AttrName* node = NewNode(AttrName);
                        node->attrName = (yyvsp[0].sval);

                        (yyval.node) = AppendList((yyvsp[-2].node), (Node*)node);
                    }
#line 1948 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 72:
#line 406 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = AppendNewList((yyvsp[0].node));
                    }
#line 1956 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 73:
#line 410 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = AppendList((yyvsp[-2].node), (yyvsp[0].node));
                    }
#line 1964 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 74:
#line 415 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = (yyvsp[-1].node);
                    }
#line 1972 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 75:
#line 420 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = AppendNewList((yyvsp[0].node));
                    }
#line 1980 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 76:
#line 424 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = AppendList((yyvsp[-2].node), (yyvsp[0].node));
                    }
#line 1988 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 77:
#line 429 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = AppendNewList((yyvsp[0].node));
                    }
#line 1996 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 78:
#line 433 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.node) = AppendList((yyvsp[-2].node), (yyvsp[0].node));
                    }
#line 2004 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 79:
#line 438 "sqlgram.y" /* yacc.c:1646  */
    {
                        ColumnDef* coldef = NewNode(ColumnDef);
                        coldef->colName = (yyvsp[-1].sval);
                        coldef->colType = (yyvsp[0].sval);
                        (yyval.node) = (Node*)coldef;
                    }
#line 2015 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 80:
#line 446 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.sval) = (yyvsp[0].sval);
                    }
#line 2023 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 81:
#line 451 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.sval) = (yyvsp[0].sval);
                    }
#line 2031 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 82:
#line 456 "sqlgram.y" /* yacc.c:1646  */
    {
                    
                }
#line 2039 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 83:
#line 461 "sqlgram.y" /* yacc.c:1646  */
    {
                        (yyval.sval) = (yyvsp[0].sval);
                    }
#line 2047 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 84:
#line 466 "sqlgram.y" /* yacc.c:1646  */
    {
                        ValuesData* val = NewNode(ValuesData);
                        val->dataVal.pData = (yyvsp[0].sval);
                        val->dataType = DT_VARCHAR;

                        (yyval.node) = (Node*)val;
                    }
#line 2059 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 85:
#line 474 "sqlgram.y" /* yacc.c:1646  */
    {
                        ValuesData* val = NewNode(ValuesData);
                        val->dataVal.iData = (yyvsp[0].ival);
                        val->dataType = DT_INTEGER;

                        (yyval.node) = (Node*)val;
                    }
#line 2071 "sqlgram.c" /* yacc.c:1646  */
    break;

  case 86:
#line 482 "sqlgram.y" /* yacc.c:1646  */
    {
                        ValuesData* val = NewNode(ValuesData);
                        val->dataVal.fData = (yyvsp[0].fval);
                        val->dataType = DT_FLOAT;

                        (yyval.node) = (Node*)val;
                    }
#line 2083 "sqlgram.c" /* yacc.c:1646  */
    break;


#line 2087 "sqlgram.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (yyscaninfo, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yyscaninfo, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, yyscaninfo);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yyscaninfo);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (yyscaninfo, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, yyscaninfo);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yyscaninfo);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 490 "sqlgram.y" /* yacc.c:1906  */


void yyerror(yyscan_t yyscaninfo, const char *msg)
{
    fprintf(stderr, "error: %s\n",msg);
}
