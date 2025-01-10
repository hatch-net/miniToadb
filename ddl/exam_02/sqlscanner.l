/* 
 * flex 
*/
%{
#include <stdio.h>
#include <string.h>

%}

%option case-insensitive
%option noyywrap

space       [ \t\n\r\f]
nonewline   [^\n\r]
comment     ("--"{nonewline}*)
whitespace	({space}+|{comment})

identify    [a-zA-Z][a-zA-Z0-9_]*

flagself	[,().;\+\-\*\/\%\<\>\=]

%% 
CREATE          {
                    printf("%s\n", yytext); 
                }
TABLE	        {
		            printf("%s\n", yytext); 
	            }
{identify}      {
                    printf("%s\n", yytext);
                }
{flagself}      {
                    printf("%s\n", yytext); 
                }
{whitespace}    {
                    ; /* ignore */
                }

%%
int main(int argc, char *argv[])
{
    printf("please intput SQL.\n");
	yylex();
}

