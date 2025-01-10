#ifndef HAT_SQLPARSER_H
#define HAT_SQLPARSER_H

typedef struct Node Node;

typedef union YYVAL_TYPE
{
    char *sval;
}YYVAL_TYPE;
#define YYSTYPE YYVAL_TYPE



#endif