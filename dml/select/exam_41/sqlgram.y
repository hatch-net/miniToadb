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
%token AND
%token OR
%token NOT
%token LESS_EQ
%token GREATER_EQ
%token NOT_EQ

%token <sval> IDENT
%token <sval> STRING
%token <ival> INTNUMBER
%token <fval> FLOATNUMBER

%type <sval> tablename attr_type attr_name

%type <node> top_stmt stmt create_stmt drop_stmt columndef_list column_def insert_stmt attr_name_list_opt attr_name_list multi_values_list values_list row_values constValues

%start top_stmt

%left		OR
%left		AND
%right		NOT
%nonassoc   '>' '<' '='  LESS_EQ GREATER_EQ NOT_EQ
%left		Op 
%left       '+' '-' 
%left       '*' '/' '%'
%right		UMINUS
%left		'(' ')'
%left		'.'

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
           | select_stmt
                    {

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
select_stmt:    SELECT select_clause from_clause where_clause
                    {

                    }
        ;        
select_clause: target_opts target_list
                    {

                    }
        ;
target_opts:                    
            /* empty */
                    {

                    }
            | ALL
                    {

                    }
            | DISTINCT
                    {

                    }
        ;
target_list: target_element
                    {

                    }
            | target_list ',' target_element 
                    {

                    }
        ;
target_element: a_expr alias_opt
                {

                }
            | '*'
                {

                }
        ;
alias_opt: /* empty */
                {
                    
                }
            | AS expr_name
                {

                }   
            | expr_name
                {

                }             
        ;        
from_clause: /* empty */ 
                {
                   
                }
            | FROM from_list
                {

                }
        ;
from_list: table_ref 
                {

                }
            | from_list ',' table_ref
                {

                }
        ;
where_clause: /* empty */
                {
                    
                }
            | WHERE a_expr
                {

                }
        ;  
a_expr: c_expr
                    {
                        
                    }
        | '-' a_expr %prec UMINUS
                    {
 
                    }
        | a_expr '+' a_expr
                    {

                    }
        | a_expr '-' a_expr
                   {

                    }
        | a_expr '*' a_expr
                    {

                    }
        | a_expr '/' a_expr  
                    {

                    }
        | a_expr '%' a_expr  
                    {
 
                    }
        /* logical operators */
        | a_expr '>' a_expr
                    {

                    }
        | a_expr '<' a_expr     
                    {

                    }  
        | a_expr '=' a_expr
                    {
 
                    }
        | a_expr LESS_EQ a_expr
                    {

                    }
        | a_expr GREATER_EQ a_expr   
                    {

                    }    
        | a_expr NOT_EQ a_expr
                    {

                    }
        | a_expr AND a_expr       
                    {
 
                    }
        | a_expr OR a_expr
                    {

                    }
        | NOT a_expr
                    {

                    }
        ;
    
c_expr : column_name
        {
            
        }
        |  constValues
        {
                       
        }         
    ; 
column_name : expr_name
        {
            
        }
        | expr_name '.' expr_name
        {
            
        }
    ;   
table_ref: table_info 
                {

                }
        | table_join 
                {

                }
    ;
table_info: tablename alias_opt
                {

                }
        | '(' select_stmt ')' alias_opt    
                {

                }
    ;
table_join: table_ref inner_cross_join_opt JOIN table_info join_condition_opt
            {
                
            }
        | table_ref left_or_right outer_opt JOIN table_info join_condition
            {

            }
        | table_ref NATURAL left_or_right_opt JOIN table_info 
            {

            }
    ;

left_or_right_opt: /* empty */
            {

            }
        | left_or_right
            {

            }
    ;
left_or_right: LEFT 
            {

            }   
        | RIGHT
            {

            }
    ; 
outer_opt: /* empty */
            {

            }
        | OUTER 
            {

            }
    ;
inner_cross_join_opt: /* empty */
            {

            }
        | INNER 
            {

            }
        | CROSS 
            {

            }
    ;    
join_condition_opt: /* empty */
            {

            }
        | join_condition
            {

            }
    ;
join_condition: ON a_expr
            {

            }
        | USING '(' attr_name_list ')'
            {

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