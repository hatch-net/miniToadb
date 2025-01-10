%{
#include <stdio.h>
#include "sqlparser.h"
#include "sqlscanner.h"
#include "node.h"

void yyerror(yyscan_t yyscaninfo, const char *msg);
%}
%pure-parser
%parse-param { yyscan_t yyscaninfo }
%lex-param { yyscan_t yyscaninfo }

%union {
    char *sval;
    Node *node;
}

%token CREATE
%token TABLE
%token <sval> IDENT

%type <sval> tablename attr_type attr_name

%type <node> columndef_list create_stmt column_def 

%%
create_stmt:    CREATE TABLE tablename '(' columndef_list ')' ';'
                    {
                        ParserTree* parserTree = (ParserTree*)yyget_extra(yyscaninfo);
                        CreateStmt* createStmt = NewNode(CreateStmt);
                        createStmt->tableName = $3;
                        createStmt->colList = $5;

                        parserTree->root = (Node*)createStmt;
                        parserTree->result = 0;
                    }
        ;
columndef_list: column_def
                    {
                        $$ = AppendNewList($1);
                    }
                |    columndef_list ',' column_def
                    {
                        $$ = AppendList($1, $3);
                    }
        ;
column_def:     attr_name attr_type
                    {
                        ColumnDef* coldef = NewNode(ColumnDef);
                        coldef->colName = $1;
                        coldef->colType = $2;
                        $$ = (Node*)coldef;
                    }
        ;
tablename:      IDENT
                    {
                        $$ = $1;
                    }
        ;
attr_name:      IDENT
                    {
                        $$ = $1;
                    }
        ;
attr_type:      IDENT
                    {
                        $$ = $1;
                    }
        ;
%%

void yyerror(yyscan_t yyscaninfo, const char *msg)
{
    fprintf(stderr, "error: %s\n",msg);
}