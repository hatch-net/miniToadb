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

%type <sval> tablename attr_type attr_name expr_name alias_opt

%type <ival> target_opts left_or_right_opt left_or_right outer_opt inner_cross_join_opt

%type <node> top_stmt stmt create_stmt drop_stmt columndef_list column_def insert_stmt attr_name_list_opt attr_name_list multi_values_list values_list row_values constValues
%type <node> select_stmt select_clause from_clause target_list target_element  
%type <node> from_list table_ref table_info table_join join_condition_opt join_condition  
%type <node> where_clause a_expr c_expr ref_name

%start top_stmt

%left		OR
%left		AND
%right		NOT
%nonassoc   '>' '<' '='  LESS_EQ GREATER_EQ NOT_EQ
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
select_stmt:    SELECT select_clause from_clause where_clause
                {
                    SelectStmt* node = NewNode(SelectStmt);
                    node->targetNode = $2;
                    node->fromList = $3;
                    node->whereList = $4;

                    $$ = (Node*)node;
                }
        ;       
select_clause: target_opts target_list
                {
                    TargetNode* node = NewNode(TargetNode);
                    node->targetOption = $1;
                    node->targetList = $2;  

                    $$ = (Node*)node;
                }
        ;
target_opts:                    
            /* empty */
            {
                $$ = 0;
            }
            | ALL
            {
                $$ = 0x01;
            }
            | DISTINCT
            {
                $$ = 0x02;
            }
        ;
target_list: target_element
            {
                $$ = AppendNewList($1);
            }
            | target_list ',' target_element 
            {
                $$ = AppendList($1, $3);
            }
        ;
target_element: expr_name '.' '*'
                {
                    TargetElement* node = NewNode(TargetElement);

                     ColumnName* subnode = NewNode(ColumnName);
                    subnode->parent_name = $1;
                    subnode->sub_name = NULL;

                    node->alias = NULL;
                    node->all = 1;
                    node->val = (Node*)subnode;
                    node->isTop = TOP_TARGET;

                    $$ = (Node*)node;
                }
                | a_expr alias_opt
                {
                    TargetElement* node = NewNode(TargetElement);
                    node->alias = $2;
                    node->all = 0;
                    node->val = $1;
                    node->isTop = TOP_TARGET;

                    $$ = (Node*)node;
                }
            | '*'
                {
                    TargetElement* node = NewNode(TargetElement);
                    node->alias = NULL;
                    node->all = 1;
                    node->val = NULL;
                    node->isTop = TOP_TARGET;

                    $$ = (Node*)node;
                }
        ;
alias_opt: /* empty */
                {
                    $$ = NULL;
                }
            | AS expr_name
                {
                    $$ = $2;
                }   
            | expr_name
                {
                    $$ = $1;
                }             
        ;        
from_clause: /* empty */ 
                {
                   $$ = NULL;
                }
            | FROM from_list
                {
                    $$ = $2;
                }
        ;
from_list: table_ref 
            {
                $$ = AppendNewList($1);
            }
            | from_list ',' table_ref
            {
                $$ = AppendList($1, $3);
            }
        ;
where_clause: /* empty */
            {
                $$ = NULL;
            }
            | WHERE a_expr
            {
                $$ = $2;
            }
        ;  
a_expr: c_expr
        {
            $$ = $1;  
        }
        | '-' a_expr %prec UMINUS
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_NEGATIVE;
            node->lexpr = $2;
            node->rexpr = NULL;

            $$ = (Node*)node;
        }
        | a_expr '+' a_expr
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_PLUS;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        | a_expr '-' a_expr
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_MINUS;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        | a_expr '*' a_expr
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_MULTIPLE;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        | a_expr '/' a_expr  
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_DIVISIION;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        | a_expr '%' a_expr  
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_MOD;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        /* logical operators */
        | a_expr '>' a_expr
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_GREATER;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        | a_expr '<' a_expr     
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_LESS;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        } 
        | a_expr '=' a_expr
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_EQUAL;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        | a_expr LESS_EQ a_expr
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_LESS_EQUAL;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        | a_expr GREATER_EQ a_expr   
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_GREATER_EQUAL;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }  
        | a_expr NOT_EQ a_expr
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_NOT_EQUAL;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        | a_expr AND a_expr       
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_AND;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        | a_expr OR a_expr
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_OR;
            node->lexpr = $1;
            node->rexpr = $3;

            $$ = (Node*)node;
        }
        | NOT a_expr
        {
            Expr* node = NewNode(Expr);
            node->exprOp = EXPR_OP_NOT;
            node->lexpr = $2;
            node->rexpr = NULL;

            $$ = (Node*)node;
        }
    ;    
c_expr : ref_name
        {
            $$ = $1;
        }
        |  constValues
        {
            $$ = $1;            
        }         
    ; 
ref_name : expr_name
        {
            ColumnName* node = NewNode(ColumnName);
            node->parent_name = NULL;
            node->sub_name = $1;

            $$ = (Node*)node;
        }
        | expr_name '.' expr_name
        {
            ColumnName* node = NewNode(ColumnName);
            node->parent_name = $1;
            node->sub_name = $3;

            $$ = (Node*)node;
        }
    ;  
table_ref: table_info 
            {
                $$ = $1;
            }
        | table_join 
            {
                $$ = $1;
            }
    ;
table_info: ref_name alias_opt
            {                
                TableRefInfo* node = NewNode(TableRefInfo);
                node->alias = $2;
                node->val = $1;
                node->rtype = RT_SIMPLE;

                $$ = (Node*)node;
            }
        | '(' select_stmt ')' alias_opt    
            {
                TableRefInfo* node = NewNode(TableRefInfo);
                node->alias = $4;
                node->val = $2;
                node->rtype = RT_SUBQUERY;

                $$ = (Node*)node;
            }
    ;
table_join: table_ref inner_cross_join_opt JOIN table_info join_condition_opt
            {                
                JoinTblInfo* node = NewNode(JoinTblInfo);
                node->joinType = $2;
                node->leftTblInfo = $1;
                node->rightTblInfo = $4;
                node->joinExpr = $5;

                $$ = (Node*)node;	
            }
        | table_ref left_or_right outer_opt JOIN table_info join_condition
            {
                JoinTblInfo* node = NewNode(JoinTblInfo);
                node->joinType = $2;
                node->leftTblInfo = $1;
                node->rightTblInfo = $5;
                node->joinExpr = $6;

                $$ = (Node*)node;	
            }
        | table_ref NATURAL left_or_right_opt JOIN table_info 
            {
                JoinTblInfo* node = NewNode(JoinTblInfo);
                node->joinType = JOIN_NATURAL | $3;
                node->leftTblInfo = $1;
                node->rightTblInfo = $5;
                node->joinExpr = NULL;

                $$ = (Node*)node;	
            }
    ;
left_or_right_opt: /* empty */
            {
                $$ = 0;
            }
        | left_or_right
            {
                $$ = $1;
            }
    ;
left_or_right: LEFT 
            {
                $$ = JOIN_LEFT;
            }   
        | RIGHT
            {
                $$ = JOIN_RIGHT;
            }
    ; 
outer_opt: /* empty */
            {
                $$ = 0;
            }
        | OUTER 
            {
                $$ = 1;
            }
    ;
inner_cross_join_opt: /* empty */
            {
                $$ = JOIN_INNER;
            }
        | INNER 
            {
                $$ = JOIN_INNER;
            }
        | CROSS 
            {
                $$ = JOIN_CROSS;
            }
    ;    
join_condition_opt: /* empty */
            {
                $$ = NULL;
            }
        | join_condition
            {
                $$ = $1;
            }
    ;
join_condition: ON a_expr
            {
                $$ = $2;
            }
        | USING '(' attr_name_list ')'
            {
                $$ = $3;
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
                    $$ = $1;
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
    ParserTree* parserTree = (ParserTree*)yyget_extra(yyscaninfo);
    parserTree->result = -1;
    fprintf(stderr, "error: %s\n",msg);
}