%{
#include <stdio.h>

%}

%union {
    char *sval;
}

%token IDENT

%type <sval> tablename attr_type attr_name

%type <sval> columndef_list create_stmt column_def 

%%
create_stmt:    CREATE TABLE tablename '(' columndef_list ')' ';'
                    {

                    }
        ;
columndef_list: column_def
                    {

                    }
                |    columndef_list ',' column_def
                    {

                    }
        ;
column_def:   attr_name attr_type
                    {

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
int main(int argc, char *argv[])
{
	yyparser();
}

void yyerror(char *s)
{
	printf("error: %s\n", s);
}