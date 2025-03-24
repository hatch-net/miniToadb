#ifndef HAT_SQLPARSER_H
#define HAT_SQLPARSER_H

#pragma pack(push, 1)

typedef struct Node Node;

typedef union YYVAL_TYPE
{
    char *sval;
    int   ival;
    float fval;
    Node *node;
}YYVAL_TYPE;
#define YYSTYPE YYVAL_TYPE

typedef struct ParserTree
{
    Node *root;      
    int result;
}ParserTree, *PParserTree;
#define YY_EXTRA_TYPE PParserTree

#pragma pack(pop)

#endif