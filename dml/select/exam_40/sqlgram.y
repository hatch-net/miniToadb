%{
#include <stdio.h>
#include "sqlparser.h"
#include "sqlscanner.h"
#include "node.h"
#include "dictionary.h"

void yyerror(yyscan_t yyscaninfo, const char *msg);
%}

%pure-parser
%parse-param { yyscan_t yyscaninfo }
%lex-param { yyscan_t yyscaninfo }

%union {
    char *sval;
    int  ival;
    float fval;
    Node *node;
}

%token CREATE
%token TABLE
%token DROP
%token INSERT
%token INTO
%token VALUES
%token SELECT
%token FROM
%token WHERE
%token ALL
%token DISTINCT
%token AS
%token JOIN
%token INNER
%token CROSS
%token LEFT
%token RIGHT
%token OUTER         
%token NATURAL
%token ON
%token USING


%token <sval> IDENT
%token <sval> STRING
%token <ival> INTNUMBER
%token <fval> FLOATNUMBER

%type <sval> tablename attr_type attr_name

%type <node> top_stmt stmt create_stmt drop_stmt columndef_list column_def insert_stmt attr_name_list_opt attr_name_list multi_values_list values_list row_values constValues

%start top_stmt


%%
top_stmt :  stmt ';'
                    {
                        ParserTree* parserTree = (ParserTree*)yyget_extra(yyscaninfo);
                        parserTree->root = $1;
                        parserTree->result = 0;
                    }
        ;
stmt :      create_stmt
                    {
                        $$ = $1;
                    }
           | drop_stmt
                    {
                        $$ = $1;
                    }
           | insert_stmt
                    {
                        $$ = $1;
                    }
        ;
create_stmt:    CREATE TABLE tablename '(' columndef_list ')' 
                    {                       
                        CreateStmt* createStmt = NewNode(CreateStmt);
                        createStmt->tableName = $3;
                        createStmt->colList = $5;

                        $$ = (Node*)createStmt;
                    }
        ;
drop_stmt:      DROP TABLE tablename 
                    {
                        DropStmt* dropStmt = NewNode(DropStmt);
                        dropStmt->tableName = $3;

                        $$ = (Node*)dropStmt;
                    }
       ;
insert_stmt:    INSERT INTO tablename attr_name_list_opt VALUES multi_values_list
                    {
                        InsertStmt* insertStmt = NewNode(InsertStmt);
                        insertStmt->tableName = $3;
                        insertStmt->attrNameList = $4;
                        insertStmt->valuesList = $6;

                        $$ = (Node*)insertStmt;
                    }
        ;
attr_name_list_opt : /* empty */
                    {
                        $$ = NULL;
                    }
                | '(' attr_name_list ')' 
                    {
                        $$ = $2;
                    }
        ;
attr_name_list: attr_name
                    {
                        AttrName* node = NewNode(AttrName);
                        node->attrName = $1;

                        $$ = AppendNewList((Node*)node);
                    }
                | attr_name_list ',' attr_name
                    {
                        AttrName* node = NewNode(AttrName);
                        node->attrName = $3;

                        $$ = AppendList($1, (Node*)node);
                    }
        ;
multi_values_list: values_list
                    {
                        $$ = AppendNewList($1);
                    }
                | multi_values_list ',' values_list
                    {
                        $$ = AppendList($1, $3);
                    } 
        ;
values_list : '(' row_values ')'
                    {
                        $$ = $2;
                    }
        ;
row_values:    constValues
                    {
                        $$ = AppendNewList($1);
                    }
                | row_values ',' constValues
                    {
                        $$ = AppendList($1, $3);
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
expr_name :     IDENT
                {
                    
                }
    ;        
attr_type:      IDENT
                    {
                        $$ = $1;
                    }
        ;
constValues :  STRING
                    {
                        ValuesData* val = NewNode(ValuesData);
                        val->dataVal.pData = $1;
                        val->dataType = DT_VARCHAR;

                        $$ = (Node*)val;
                    }
            | INTNUMBER 
                    {
                        ValuesData* val = NewNode(ValuesData);
                        val->dataVal.iData = $1;
                        val->dataType = DT_INTEGER;

                        $$ = (Node*)val;
                    }
            | FLOATNUMBER
                    {
                        ValuesData* val = NewNode(ValuesData);
                        val->dataVal.fData = $1;
                        val->dataType = DT_FLOAT;

                        $$ = (Node*)val;
                    }
        ;        
%%

void yyerror(yyscan_t yyscaninfo, const char *msg)
{
    fprintf(stderr, "error: %s\n",msg);
}