%{
#include <stdio.h>
#include "sqlparser.h"
#include "sqlscanner.h"

void yyerror(yyscan_t yyscaninfo, const char *msg);
%}
%pure-parser
%parse-param { yyscan_t yyscaninfo }
%lex-param { yyscan_t yyscaninfo }

%union {
    char *sval;
}

%token CREATE
%token TABLE
%token <sval> IDENT

%type <sval> tablename attr_type attr_name

%type <sval> columndef_list create_stmt column_def 

%%
create_stmt:    CREATE TABLE tablename '(' columndef_list ')' ';'
                    {
                        printf("create_stmt \n");
                    }
        ;
columndef_list: column_def
                    {
                        printf("columndef_list \n");
                    }
                |    columndef_list ',' column_def
                    {
                        printf("columndef_list1 \n");
                    }
        ;
column_def:   attr_name attr_type
                    {
                        printf("column_def \n");
                    }
        ;
tablename:      IDENT
                    {
                        printf("tablename %s \n", $1);
                    }
        ;
attr_name:      IDENT
                    {
                        printf("attr_name %s \n", $1);
                    }
        ;
attr_type:      IDENT
                    {
                        printf("attr_type %s \n", $1);
                    }
        ;
%%

void yyerror(yyscan_t yyscaninfo, const char *msg)
{
    fprintf(stderr, "error: %s\n",msg);
}